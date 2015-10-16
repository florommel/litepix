/*
 * tetris.cpp
 * This file is part of Litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * Litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Litepix. If not, see <http://www.gnu.org/licenses/>.
 */


#include "tetris.hpp"
#include "core/progmem.hpp"
#include "core/random.hpp"
#include "main.hpp"

using Module::Tetris;


static Color get_color(Tetris::TetType type) {
    switch ((uint8_t)type) {
        case 0:  return 0x000000;
        case 1:  return 0x00cccc;
        case 2:  return 0x0000ff;
        case 3:  return 0xff6600;
        case 4:  return 0xccA000;
        case 5:  return 0x00ff00;
        case 6:  return 0xcc00a0;
        default: return 0xff0000;
    }
}


static Bitmap<4,4> get_bricks(Tetris::TetType type, uint8_t rotation) {
    static const uint16_t bricks_progmem[] PROGMEM = {
        0x2222, 0x0f00, 0x2222, 0x0f00,
        0x6440, 0x0e20, 0x44c0, 0x8e00,
        0xc440, 0x2e00, 0x4460, 0x0e80,
        0x0cc0, 0x0cc0, 0x0cc0, 0x0cc0,
        0x8c40, 0x6c00, 0x4620, 0x06c0,
        0x4c40, 0x4e00, 0x4640, 0x0e40,
        0x4c80, 0xc600, 0x2640, 0x0c60
    };
    
    Bitmap<4,4> bitmap;
    if (type == Tetris::TetType::None) return bitmap;
    uint8_t index = ((uint8_t)type-1) * 4 + rotation;
    bitmap.assign(Progmem::read(bricks_progmem, index));
    return bitmap;
}


Tetris::Tetris() {
    Random::seed(6031); // FIXME
    
    transition.set_destination(canvas);
    transition.set_mask(mask);
    Input::set_handler(DELEGATE(this, input_handler));
    is_game_over = false;
    
    new_tetromino();
    
    falling_timer.set_delegate(DELEGATE(this, fall));
    falling_timer.set_interval(1000);
    falling_timer.start();
}


void Tetris::game_over() {
    falling_timer.stop();
    is_game_over = true;
    
    for (uint16_t i = 0; i < Canvas::Pixels; i++) mask.bit_set(i);
    transition.set_source(Color(0, 0, 0));
    transition.roll(Direction::LeftRight, 3000);
}


bool Tetris::detect_collision() {
    Bitmap<4,4> bricks = get_bricks(falling.type, falling.rotation);
    for (int8_t bricks_x = 0; bricks_x < 4; bricks_x++) {
        int8_t xpos = falling.x + bricks_x;
        
        for (int8_t bricks_y = 0; bricks_y < 4; bricks_y++) {
            int8_t ypos = falling.y + bricks_y;
            
            if (bricks.get(bricks_x, bricks_y)) {
                if (xpos >= Canvas::Width)
                    return true;
                if ((ypos < 0) || (ypos >= Canvas::Height))
                    return true;
                if ((xpos > 0) && field.get(xpos, ypos) != TetType::None)
                    return true;
            }
        }
    }
    
    return false;
}


void Tetris::detect_rows_to_remove() {
    bool has_rows_to_remove = false;
    rows_to_remove = BitArray<Canvas::Width>();  // empty rows_to_remove
    
    // iterate through all rows and check if they are complete (full of bricks)
    for (uint8_t x = 0; x < Canvas::Width; x++) {
        uint8_t is_complete = true;
        for (uint8_t y = 0; y < Canvas::Height; y++) {
            if (field.get(x, y) == TetType::None) {
                is_complete = false;
                break;
            }
        }
        if (is_complete) {
            rows_to_remove.bit_set(x);
            has_rows_to_remove = true;
        }
    }
    
    if (has_rows_to_remove) remove_rows_start();
    else new_tetromino();
}


void Tetris::remove_rows_start() {
    // stop falling timer during remove transitions
    falling_timer.stop();
    
    // set transition mask according to rows_to_remove
    for (uint8_t x = 0; x < Canvas::Width; x++) {
        for (uint8_t y = 0; y < Canvas::Height; y++) {
            mask.set(x, y, rows_to_remove[x]);
        }
    }
    
    // start first transition
    transition.set_source(Color(255, 255, 255));
    transition.roll(Direction::BottomTop, 700,
                    DELEGATE(this, remove_rows_start2));
}


void Tetris::remove_rows_start2() {
    // start second transition (mask was already set in remove_rows_start())
    transition.set_source(Color(0, 0, 0));
    transition.fade(900, DELEGATE(this, remove_rows_finish));
}


void Tetris::remove_rows_finish() {
    // let bricks fall
    for (uint8_t i = 0; i < Canvas::Width; i++) {
        if (rows_to_remove[i]) {
            for (int8_t x = i-1; x >= 0; x--) {
                for (uint8_t y = 0; y < Canvas::Height; y++) {
                    field.set(x+1, y, field.get(x, y));
                }
            }
            for (uint8_t y = 0; y < Canvas::Height; y++) {
                field.set(0, y, TetType::None);
            }
        }
    }
    
    new_tetromino();
    falling_timer.start();
}


void Tetris::new_tetromino() {
    falling = {
        .type = (TetType)((Random::get_uint() % 7)+1),
        .rotation = (uint8_t)(Random::get_uint() % 4),
        .x = -3,
        .y = Canvas::Height / 2 - 2
    };
    render();
}


void Tetris::fall() {
    // fall in x direction (left to right - because led matrix is flipped)
    falling.x++;
    
    if (detect_collision()) {
        // restore previous position
        falling.x--;
        
        // add falling tetromino to field (lying tetrominos)
        Bitmap<4,4> bricks = get_bricks(falling.type, falling.rotation);
        for (int8_t bricks_x = 0; bricks_x < 4; bricks_x++) {
            int8_t xpos = falling.x + bricks_x;
            
            for (int8_t bricks_y = 0; bricks_y < 4; bricks_y++) {
                int8_t ypos = falling.y + bricks_y;
                
                if (bricks.get(bricks_x, bricks_y)) {
                    if (xpos < 0) {
                        game_over();
                        return;
                    }
                    field.set(xpos, ypos, falling.type);
                }
            }
        }
        
        detect_rows_to_remove();
    }
    else render();
}


void Tetris::rotate() {
    uint8_t org_rotation = falling.rotation;
    falling.rotation = (falling.rotation < 3) ? falling.rotation + 1 : 0;
    if (detect_collision()) falling.rotation = org_rotation;
    else render();
}


void Tetris::move(Move m) {
    uint8_t org_x = falling.x;
    uint8_t org_y = falling.y;
    
    switch (m) {
        case Move::Left:
            falling.y++;
            break;
        case Move::Right:
            falling.y--;
            break;
        case Move::Down:
            falling.x++;
            break;
        case Move::InstantDown:
            // TODO
            break;
    }
    
    if (detect_collision()) {
        // restore previous position on collision
        falling.x = org_x;
        falling.y = org_y;
    }
    else render();
}


void Tetris::input_handler(Input i) {
    switch (i.data) {
        case Input::Left:
            move(Move::Left);
            break;
        case Input::Right:
            move(Move::Right);
            break;
        case Input::Up:
            rotate();
            break;
        case Input::Down:
            move(Move::Down);
            break;
        case Input::Ok:
            move(Move::InstantDown);
            break;
        case Input::Exit:
            exit();
            break;
    }
}


void Tetris::render() {
    // paint lying bricks
    for (uint8_t i = 0; i < Canvas::Pixels; i++) {
        canvas.set_pixel(i, get_color(field[i]));
    }
    
    // paint falling tetromino
    Bitmap<4,4> bricks = get_bricks(falling.type, falling.rotation);
    Color color = get_color(falling.type);
    if (falling.type != TetType::None) {
        for (int8_t bricks_x = 0; bricks_x < 4; bricks_x++) {
            int8_t xpos = falling.x + bricks_x;
            
            if (xpos >= 0) {
                for (int8_t bricks_y = 0; bricks_y < 4; bricks_y++) {
                    int8_t ypos = falling.y + bricks_y;
                    if (bricks.get(bricks_x, bricks_y)) 
                        canvas.set_pixel(xpos, ypos, color);
                }
            }
        }
    }
    
    canvas.render();
}
