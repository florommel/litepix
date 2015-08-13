/*
 * test_pixels.c
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


#include "test_pixels.hpp"

using namespace Module;


TestPixels::TestPixels()
    : timer(DELEGATE(this, step), 100), count(Canvas::Pixels-1) {
    timer.start();
}


void TestPixels::step() {
    canvas.set_pixel(count++, Color(0, 0, 0));
    if (count >= Canvas::Pixels) count = 0;
    canvas.set_pixel(count, Color(255, 255, 255));
    
    canvas.render();
}
