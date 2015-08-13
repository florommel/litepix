/*
 * canvas.cpp
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


#include "canvas.hpp"
#include "clock.hpp"
#include "random.hpp"


Color Color::fromHSV(uint8_t h, uint8_t s, uint8_t v) {
    if (s == 0) return Color(v, v, v);
    
    uint8_t hi, f;
    {
        uint16_t h6 = h * 6;
        hi = h6 >> 8;
        f = h6 - ((uint16_t)hi << 8);
    }
    
    uint8_t p = ((uint16_t)v * (255 - s)) >> 8;
    uint8_t q = ((uint16_t)v * (255 - (((uint16_t)s * f) >> 8))) >> 8;
    uint8_t t = ((uint16_t)v * (255 - (((uint16_t)s * (255 - f)) >> 8))) >> 8;
    
    switch (hi) {
        case 0:
            return Color(v, t, p);
        case 1:
            return Color(q, v, p);
        case 2:
            return Color(p, v, t);
        case 3:
            return Color(p, q, v);
        case 4:
            return Color(t, p, v);
        default:
            return Color(v, p, q);
    }
}


Canvas::Canvas() : buffer{} {}


void Canvas::fill(const Color color) {
    uint8_t* p = buffer;
    for (uint8_t i = 0; i < Pixels; i++) {
        *p++ = color.green;
        *p++ = color.red;
        *p++ = color.blue;
    }
}


void Canvas::fill_random() {
    Random::seed(Clock::short_time_ms());
    uint8_t* p = buffer;
    for (uint8_t i = 0; i < Pixels; i++) {
        uint16_t r = Random::get_uint();
        *p++ = (uint8_t)(r & 0xFF);
        r = Random::get_uint();
        *p++ = (uint8_t)(r & 0xFF);
        *p++ = (uint8_t)(r >> 8);
    }
}


void Canvas::fill_rect(int8_t x, int8_t y, int8_t width, int8_t height,
                       Color color) {
    int8_t cj = (height > 0) ? 1 : -1;
    int8_t ci = (width > 0) ? 1 : -1;
    for (int8_t j = y; j != y + height; j += cj) {
        if (j < 0 || j > Canvas::Height - 1) continue;
        for (int8_t i = x; i != x + width; i += ci) {
            if (i < 0 || i > Canvas::Width - 1) continue;
            set_pixel(i, j, color);
        }
    }
}


Color Canvas::get_pixel(uint16_t index) const {
    index *= 3;
    return Color(buffer[index+1], buffer[index], buffer[index+2]);
}


Color Canvas::get_pixel(const uint8_t x, const uint8_t y) const {
    return get_pixel(y*Width + x);
}


void Canvas::set_pixel(uint16_t index, const Color color) {
    index *= 3;
    buffer[index++] = color.green;
    buffer[index++] = color.red;
    buffer[index] = color.blue;
}


void Canvas::set_pixel(const uint8_t x, const uint8_t y, const Color color) {
    set_pixel(y*Width + x, color);
}
