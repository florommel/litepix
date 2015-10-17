/*
 * ani_test.cpp
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


#include "ani_test.hpp"
#include "core/random.hpp"
#include "main.hpp"

using namespace Module;


AniTest::AniTest() : step_timer(DELEGATE(this, step), 20), hue(0), pos(0) {
    step_timer.start();
    Random::seed(2677);
    Input::set_handler(DELEGATE(this, input));
}


void AniTest::spot() {
    uint8_t i = Random::get_uint() % 240;
    if (int_matrix1[i] == 0) up_mask0.bit_set(i);
}


void AniTest::spot2() {
    if (int_matrix1[pos] == 0) up_mask0.bit_set(pos);
    if (pos >= Canvas::Pixels - 1) pos = 0;
    else ++pos;
}


void AniTest::step() {
    spot();
    
    for (uint8_t i = 0; i < Canvas::Pixels; ++i) {
        if (int_matrix1[i] <= DecValue1) int_matrix1[i] = 0;
        else int_matrix1[i] -= DecValue1;
        
        if (up_mask0[i]) {
            if (int_matrix0[i] >= 90-DecValue0) up_mask0.bit_reset(i);
            else int_matrix0[i] += DecValue0;
        } else {
            if (int_matrix0[i] <= DecValue0) int_matrix0[i] = 0;
            else int_matrix0[i] -= DecValue0;
        }
        if (int_matrix1[i] == 0)
            canvas.set_pixel(i, Color::fromHSV(hue, 255, int_matrix0[i]));
        else
            canvas.set_pixel(i, Color::fromHSV(hue + 18, 255, int_matrix1[i]));
    }
    
    canvas.render();
    
    ++hue;
}


void AniTest::input(Input in) {
    if (in.data == Input::Ok) {
        for (uint8_t i = 0; i < 24; ++i) {
            uint8_t n = Random::get_uint() % 240;
            int_matrix1[n] = 255;
            int_matrix0[n] = 0;
            up_mask0.bit_reset(n);
        }
    } else if (in.data == Input::Exit) {
        exit();
    }
}
