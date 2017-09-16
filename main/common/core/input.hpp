/*
 * input.hpp
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
 * input handling
 */

#ifndef _CORE__INPUT_HPP_
#define _CORE__INPUT_HPP_

#include "stdint.h"
#include "delegate.hpp"
#include "mainloop.hpp"


/**
 * A class to handle input events.
 * An instance of this class represents a single input event.
 * The static functions set_handler(), clear_handler() and clear_events() are
 * used to manage input event handling.
 */
class Input {
  public:

    /** A type that represents a device ID */
    typedef uint8_t Device;

    /** A type that represents input data */
    enum Data : uint8_t {
        Ok = 1,  // start with 1, 0 represents invalid Input
#ifdef REVERSE_VERTICAL
        Right,
        Left,
#else
        Left,
        Right,
#endif
#ifdef REVERSE_HORIZONTAL
        Down,
        Up,
#else
        Up,
        Down,
#endif
        Exit
    };

    /** Represents the input device ID */
    Device device;

    /** Represents the input data */
    Data data;

    /**
     * Initialize an new input object.
     * @param   device   input device ID
     * @param   data     input data
     */
    constexpr Input(Device device, Data data)
        : device(device), data(data) {}

    /**
     * Set input handler.
     * @param   handler   delegate that handles input events.
     */
    static void set_handler(Delegate<void(Input)> handler);

    /**
     * Remove active input handler.
     */
    static void clear_handler();

    /**
     * Clear all pending input events.
     * All events that are pending to be processed are removed.
     */
    static void clear_events(); // TODO not working on avr

  private:
    static void handle();
    friend void Mainloop::run();
};


#endif
