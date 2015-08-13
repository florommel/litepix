/*
 * progmem.hpp
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
 * place and read data in program memory
 */

#ifndef _CORE__PROGMEM_HPP_
#define _CORE__PROGMEM_HPP_

#include "stdint.h"


/**
 * Mark a static const var to be placed in program memory.
 * This is especially useful for constant arrays.
 * Usage: `static const type name[] PROGMEM = { item1, item2, item3 };`
 *
 * *Important:* data in program memory cannot be accessed directly.
 * Use Progmem::read().
 */
#define PROGMEM  // see "avr/progmem.inc" for real implementation.


/**
 * Contains functions to read data from program memory.
 * Use these functions in combination with vars that are placed in
 * program memory (see #PROGMEM).
 */
namespace Progmem {
    
    /** Type to measure the size (in bytes) of an arbitrary object */
    typedef decltype(sizeof(uint8_t)) size_t;
    
    /**
     * Get an object of type T from program memory at the specified address.
     * @param   address   a pointer to the object
     */
    template<typename T>
    inline T read(const T* address);
    
    /**
     * Get an object of type T from program memory at the specified address
     * with given offset.
     * This function should be used to access arrays in program memory.
     * @param   address   a pointer to the object
     * @param   offset    offset in sizeof(T) = index
     */
    template<typename T>
    inline T read(const T* address, size_t offset);
}


// Include file for "avr" or "sim"
#if defined ARCH_AVR
    #include "avr/progmem.inc"
#elif defined ARCH_SIM
    #include "sim/progmem.inc"
#endif


#endif
