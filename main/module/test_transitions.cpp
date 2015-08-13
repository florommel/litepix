/*
 * test_transitions.c
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


#include "test_transitions.hpp"
#include "main.hpp"

using namespace Module;


TestTransitions::TestTransitions() {
    Input::set_handler(DELEGATE(this, input_handler));
    
    for (uint8_t i = 0; i < Canvas::Pixels; i += 3) mask.bit_set(i);
    transition.set_destination(canvas1);
    transition1();
}


void TestTransitions::transition1() {
    canvas2.fill_random();
    transition.set_source(canvas2);
    transition.clear_mask();
    transition.fade(9000, DELEGATE(this, transition2));
}


void TestTransitions::transition2() {
    transition.set_source(Color(255, 0, 0));
    transition.clear_mask();
    transition.roll(Direction::LeftRight, 1000, DELEGATE(this, transition3));
}


void TestTransitions::transition3() {
    transition.set_source(Color(20, 5, 0));
    transition.clear_mask();
    transition.roll(Direction::RightLeft, 1000, DELEGATE(this, transition4));
}


void TestTransitions::transition4() {
    transition.set_source(Color(70, 0, 0));
    transition.set_mask(mask);
    transition.dissolve(6000, DELEGATE(this, transition5));
}


void TestTransitions::transition5() {
    transition.set_source(Color(0, 0, 0));
    transition.clear_mask();
    transition.dissolve(3000, DELEGATE(this, transition6));
}


void TestTransitions::transition6() {
    transition.set_source(Color(40, 0, 40));
    transition.set_mask(mask);
    transition.fade(1600, DELEGATE(this, transition7));
}


void TestTransitions::transition7() {
    transition.set_source(Color(255, 0, 255));
    transition.set_mask(mask);
    transition.roll(Direction::LeftRight, 300, DELEGATE(this, transition8));
}


void TestTransitions::transition8() {
    transition.set_source(Color(0, 0, 0));
    transition.clear_mask();
    transition.fade(1000, DELEGATE(this, transition1));
}


void TestTransitions::input_handler(Input input) {
    if (input.data == Input::Exit) exit();
}
