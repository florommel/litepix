/*
 * bitmap.h
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with litepix. If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _UTIL__BITMAP_H_
#define _UTIL__BITMAP_H_

#include <stdint.h>
#include <stdbool.h>


#define BITMAP_BYTE_SIZE(bit_size) ((((bit_size) & 0x07) == 0) \
    ? ((bit_size) >> 3) : (((bit_size) >> 3) + 1))


static inline void bitmap_fill(uint8_t* bitmap, uint8_t bit_size, bool value) {
    uint8_t n = BITMAP_BYTE_SIZE(bit_size);
    uint8_t v = value ? UINT8_MAX : 0;
    uint8_t i;
    for (i = 0; i < n; i++) bitmap[i] = v;
}


static inline bool bitmap_get(uint8_t* bitmap, uint8_t index) {
   return bitmap[index >> 3] & (1 << (index & 0x07));
}


static inline void bitmap_set(uint8_t* bitmap, uint8_t index) {
    bitmap[index >> 3] |= (1 << (index & 0x07));
}


static inline void bitmap_reset(uint8_t* bitmap, uint8_t index) {
    bitmap[index >> 3] &= ~(1 << (index & 0x07));
}


#endif
