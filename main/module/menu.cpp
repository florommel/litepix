/*
 * menu.cpp
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


#include "core/delegate.hpp"
#include "menu.hpp"
#include "main.hpp"

using namespace Module;


Menu::Menu() {
    Input::set_handler(DELEGATE(this, input));
    canvas.fill(0x500000);
    canvas.render();
    transition.set_destination(canvas);
}


void Menu::input(Input i) {
    transition.set_source(0x000000);
    
    if (i.data == Input::Ok)
        transition.dissolve(1000, DELEGATE(this, start_tetris));
}


void Menu::start_tetris() {
    invoke(ModId::Tetris);
}
