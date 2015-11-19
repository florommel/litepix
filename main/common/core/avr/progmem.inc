/*
 * progmem.inc
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


#ifndef DOCUMENTATION_SKIP


#undef PROGMEM
#define PROGMEM __attribute__((progmem))


namespace Progmem {

    template<>
    inline uint8_t read<uint8_t>(uint8_t const* address);

}



template<typename T>
static inline T Progmem::read(T const* address) {
    uint8_t data[sizeof(T)];
    uint8_t const* pgm_ptr = reinterpret_cast<uint8_t const*>(address);
    for (size_t i = 0; i < sizeof(T); i++) {
        data[i] = read(pgm_ptr++);
    }
    void* ram_ptr = data;
    return *reinterpret_cast<T*>(ram_ptr);
}


template<>
inline uint8_t Progmem::read<uint8_t>(uint8_t const* address) {
    register uint8_t result;
    asm volatile (
        "lpm" "\n\t"
        "mov %0, r0" "\n\t"
        : "=r" (result)
        : "z" (address)
        : "r0"
        );
    return result;
}


template<typename T>
static inline T Progmem::read(T const* address, size_t offset) {
    return read<T>(address + offset);
}

#endif
