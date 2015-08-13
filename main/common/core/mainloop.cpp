/*
 * mainloop.cpp
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


#include "mainloop.hpp"
#include "timer.hpp"
#include "input.hpp"


static bool exit_flag = false;


void Mainloop::run() {
    exit_flag = false;
    while (!exit_flag) {
        Input::handle();
        Timer::handle();
    }
    exit_flag = false;
}


void Mainloop::exit() {
    exit_flag = true;
}
