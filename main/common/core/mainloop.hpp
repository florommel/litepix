/*
 * mainloop.hpp
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

/**
 * @file
 * main event loop
 */

#ifndef _CORE__MAINLOOP_HPP_
#define _CORE__MAINLOOP_HPP_

#include <stdint.h>


/**
 * Contains functions to run an stop the main event loop.
 */
namespace Mainloop {

    /**
     * Runs the main event loop.
     * The main loop dispatches timer and input events. It runs until exit() is
     * called (inside an event handler).
     * There should only be one main loop at a time, therefore run() must not be
     * called inside a timer or input handler.
     */
    void run();
    
    /**
     * Stops the main event loop (started with run()) as fast as possible
     * (usually after current event handler (timer or input) returns).
     * This funciton should be called inside an event handler or a funciton
     * invoked by one. If the main loop is not running this has no effect.
     */
    void exit();
    
}


#endif
