/*
 * random.cpp
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


#include "random.hpp"


static uint16_t rand = 0xAAAA;


void Random::seed(uint16_t seed) {
    if (seed == 0) seed = 0xAAAA;
    rand = seed;
}


uint16_t Random::get_uint() {
    rand ^= (rand << 13);
    rand ^= (rand >> 9);
    rand ^= (rand << 7);
    return rand;
}


bool Random::get_bool() {
    while (true) {
        uint16_t x = get_uint();
        if (x <= 0x8000) return true;
        else if (x == 0xFFFF) continue;
        else return false;
    }
}
