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


#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include "stdint.h"


struct Input {
    /* ---- NESTED TYPES ---- */
    typedef uint8_t Device;
    
    enum Data : uint8_t {
        NoData = 0,
        Ok = 1,
        Left = 2,
        Right = 3,
        Up = 4,
        Down = 5,
        Exit = 6
    };
    
    /* ---- DATA MEMBERS ---- */
    Device device;
    Data data;
    
    constexpr Input()
        : device(0), data(NoData) {}
    constexpr Input(Device device, Data data)
        : device(device), data(data) {}
};

#endif
