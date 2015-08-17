/*
 * startup.cpp
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


#include "main.hpp"
#include "startup.hpp"


static const uint8_t Wh = Canvas::Width / 2 - 1;
static const uint8_t Hh = Canvas::Height / 2 - 1;


Module::StartUp::StartUp()
        : timer(DELEGATE(this, animation), 30), lsize(12), lval(0) {
    transition.set_destination(canvas1);
    transition.set_source(canvas2);
    timer.start();
}


void Module::StartUp::paintl(const Color color) {
    canvas2.set_pixel(Wh, Hh, color);
    canvas2.set_pixel(Wh + 1, Hh, color);
    canvas2.set_pixel(Wh + 1, Hh + 1, color);
}


void Module::StartUp::animation() {
    Color lcolor = Color(lval, 0, 0);
    
    canvas1.fill(0x000000);
    canvas1.fill_rect(Wh, Hh, -lsize, -lsize, lcolor);
    canvas1.fill_rect(Wh + 1, Hh, lsize, -lsize, lcolor);
    canvas1.fill_rect(Wh + 1, Hh + 1, lsize, lsize, lcolor);
    canvas1.render();
    
    lsize--;
    lval += 12;
    
    if (lsize == 0) timer.set_delegate(DELEGATE(this, transition0));
}


void Module::StartUp::transition0() {
    timer.stop();
    paintl(0xff4444);
    transition.fade(1000, DELEGATE(this, transition1));
}


void Module::StartUp::transition1() {
    canvas2.fill(0x000010);
    paintl(0x000000);
    transition.fade(1000, DELEGATE(this, transition2));
}


void Module::StartUp::transition2() {
    canvas2.fill(0x0033dd);
    paintl(0x000000);
    transition.roll(Direction::LeftRight, 700, DELEGATE(this, transition3));
}


void Module::StartUp::transition3() {
    canvas2.fill(0x000000);
    transition.fade(1200, DELEGATE_STATIC(exit));
}
