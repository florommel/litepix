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

static const uint8_t snake_speed = 150;


static inline void inc(uint8_t& value, uint8_t max) {
    if (value >= max) value = 0;
    else ++value;
}


static inline void dec(uint8_t& value, uint8_t max) {
    if (value == 0) value = max;
    else --value;
}


static void follow_direction(uint8_t& x, uint8_t& y, const Snake::Type move) {
    switch (move) {
        case Snake::Type::PartLeft:
            dec(x, Canvas::Width-1);
            break;
        case Snake::Type::PartRight:
            inc(x, Canvas::Width-1);
            break;
        case Snake::Type::PartUp:
            dec(y, Canvas::Height-1);
            break;
        case Snake::Type::PartDown:
            inc(y, Canvas::Height-1);
            break;
        default: break;
    }
}


Snake::Snake()
: head_x(mx+1), head_y(my), tail_x(mx-1),
  tail_y(my), prev(Type::PartRight) {
    Input::set_handler(DELEGATE(this, input));
    transition.set_destination(canvas1);
    transition.set_source(canvas2);
    transition.fade(800, DELEGATE(this, move));
    
    field(mx-1, my) = Type::PartRight;
    field(mx, my) = Type::PartRight;
    field(mx+1, my) = Type::PartRight;
    place_food();
    render();
}


void Snake::game_over() {
    // TODO
}


void Snake::move() {
    {
        prev = field(head_x, head_y);
        follow_direction(head_x, head_y, prev);
        
        Type t = field(head_x, head_y);
        bool eat = (t == Type::Food);
        if (t != Type::Empty && t != Type::Food) {
            game_over();
            return;
        }
        
        field(head_x, head_y) = prev;
        
        if (eat) {
            place_food();
        } else {
            Type t = field(tail_x, tail_y);
            field(tail_x, tail_y) = Type::Empty;
            follow_direction(tail_x, tail_y, t);
        }
    }
    
    render();
    transition.fade(snake_speed, DELEGATE(this, move));
}


void Snake::place_food() {
    uint8_t i = Random::get_uint() % Canvas::Pixels;
    
    while (field[i] != Type::Empty) {
        inc(i, Canvas::Pixels);
    }
    
    field[i] = Type::Food;
}


void Snake::input(Input i) {
    Type old_type = field(head_x, head_y);
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
    field(head_x, head_y) = new_type;
}


void Snake::render() {
    for (uint16_t i = 0; i < Canvas::Pixels; ++i) {
        switch (field[i]) {
            case Type::Empty:
                canvas2.set_pixel(i, 0x000000);
                break;
            case Type::Food:
                canvas2.set_pixel(i, 0xff0000);
                break;
            default: break;
        }
    }
    
    static const uint8_t hue_min = 0x7a;
    static const uint8_t hue_max = 0xaa;
    uint8_t hue = hue_max;
    uint8_t hue_inc = -2;
    uint8_t x = tail_x;
    uint8_t y = tail_y;
    while (true) {
        canvas2.set_pixel(x, y, Color::from_hsv(hue, 0xee, 0xff));
        if (hue <= hue_min) hue_inc = 2;
        else if (hue >= hue_max) hue_inc = -2;
        hue += hue_inc;
        
        if (x == head_x && y == head_y) break;
        
        follow_direction(x, y, field(x, y));
    }
}
