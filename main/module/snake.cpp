/*
 * snake.cpp
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


#include "snake.hpp"
#include "main.hpp"
#include "core/random.hpp"

using Module::Snake;


static const uint8_t mx = Canvas::Width / 2 - 1;
static const uint8_t my = Canvas::Height / 2 - 1;


static void inc(uint8_t& value, uint8_t max) {
    if (value >= max) value = 0;
    else value++;
}


static void dec(uint8_t& value, uint8_t max) {
    if (value == 0) value = max;
    else value--;
}


Snake::Snake()
: move_timer(DELEGATE(this, move), 200), head_x(mx+1), head_y(my),
  tail_x(mx-1), tail_y(my), prev(Type::PartRight) {
    
    field.set(mx-1, my, Type::PartRight);
    field.set(mx, my, Type::PartRight);
    field.set(mx+1, my, Type::PartRight);
    place_food();
    render();
    
    Input::set_handler(DELEGATE(this, input));
    move_timer.start();
}


void Snake::game_over() {
    move_timer.stop();
}


void Snake::move() {
    Type h = field.get(head_x, head_y);
    prev = h;
    switch (h) {
        case Type::PartLeft:
            dec(head_x, Canvas::Width-1);
            break;
        case Type::PartRight:
            inc(head_x, Canvas::Width-1);
            break;
        case Type::PartUp:
            dec(head_y, Canvas::Height-1);
            break;
        case Type::PartDown:
            inc(head_y, Canvas::Height-1);
            break;
        default: return;
    }
    
    Type t = field.get(head_x, head_y);
    bool eat = (t == Type::Food);
    if (t != Type::Empty && t != Type::Food) {
        game_over();
        return;
    }
    
    field.set(head_x, head_y, h);
    
    if (eat) {
        place_food();
    } else {
        Type t = field.get(tail_x, tail_y);
        field.set(tail_x, tail_y, Type::Empty);
        switch (t) {
            case Type::PartLeft:
                dec(tail_x, Canvas::Width-1);
                break;
            case Type::PartRight:
                inc(tail_x, Canvas::Width-1);
                break;
            case Type::PartUp:
                dec(tail_y, Canvas::Height-1);
                break;
            case Type::PartDown:
                inc(tail_y, Canvas::Height-1);
                break;
            default: return;
        }
    }
    
    render();
}


void Snake::place_food() {
    uint8_t i = Random::get_uint() % Canvas::Pixels;
    
    while (field[i] != Type::Empty) {
        inc(i, Canvas::Pixels);
    }
    
    field[i] = Type::Food;
}


void Snake::input(Input i) {
    Type old_type = field.get(head_x, head_y);
    Type new_type;
    
    switch (i.data) {
        case Input::Left:
            if (prev == Type::PartRight) return;
            new_type = Type::PartLeft;
            break;
        case Input::Right:
            if (prev == Type::PartLeft) return;
            new_type = Type::PartRight;
            break;
        case Input::Up:
            if (prev == Type::PartDown) return;
            new_type = Type::PartUp;
            break;
        case Input::Down:
            if (prev == Type::PartUp) return;
            new_type = Type::PartDown;
            break;
        case Input::Exit:
            exit();
        default:
            return;
    }
    
    if (old_type == new_type) return;
    field.set(head_x, head_y, new_type);
}


void Snake::render() {
    for (uint16_t i = 0; i < Canvas::Pixels; i++) {
        switch (field[i]) {
            case Type::Empty:
                canvas.set_pixel(i, 0x000000);
                break;
            case Type::PartLeft:
            case Type::PartRight:
            case Type::PartUp:
            case Type::PartDown:
                canvas.set_pixel(i, 0x0000ff);
                break;
            case Type::Food:
                canvas.set_pixel(i, 0xff0000);
                break;
        }
    }
    canvas.render();
}
