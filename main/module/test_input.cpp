/*
 * test_input.cpp
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


#include "test_input.hpp"
#include "main.hpp"

using namespace Module;


TestInput::TestInput() {
    transition.set_destination(canvas);
    Input::set_handler(DELEGATE(this, ihandler));
}


void TestInput::ihandler(Input input) {
    x += 100;
    transition.set_source(Color(x-40, 0, x));
    
    switch (input.data) {
        case Input::Left:
            transition.roll(Direction::RightLeft, 500, DELEGATE(this, fout));
            break;
        case Input::Right:
            transition.roll(Direction::LeftRight, 500, DELEGATE(this, fout));
            break;
        case Input::Up:
            transition.roll(Direction::BottomTop, 500, DELEGATE(this, fout));
            break;
        case Input::Down:
            transition.roll(Direction::TopBottom, 500, DELEGATE(this, fout));
            break;
        case Input::Ok:
            transition.dissolve(1000, DELEGATE(this, fout));
            break;
        case Input::Exit:
            exit();
            break;
    }
}


void TestInput::fout() {
    transition.set_source(Color(0, 0, 0));
    transition.fade(500);
}
