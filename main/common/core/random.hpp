/*
 * random.hpp
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
 * random number generation
 */

#ifndef _CORE__RANDOM_HPP_
#define _CORE__RANDOM_HPP_

#include <stdint.h>


/**
 * Provides functions for pseudorandom number generation.
 */
namespace Random {

    /**
     * Initialize random number generator with a seed.
     * @param   seed   the seed.
     */
    void seed(uint16_t seed);

    /**
     * Get a pseudorandom integer value between 1 and 2^16-1.
     * Note that the value 0 is never returned.
     * @return   random integer value between 1 and 2^16-1
     */
    uint16_t get_uint();

    /**
     * Get a pseudorandom boolean value.
     * @return   random boolean value
     */
    bool get_bool();

}


#endif
