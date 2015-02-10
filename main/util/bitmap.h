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
#include "../core/pix.h"


#if PIX_NUM_PIXELS % 8 == 0
    #define BITMAP_SIZE (COUNT/8)
#else
    #define BITMAP_SIZE ((COUNT/8)+1)
#endif


typedef struct {
    uint8_t byte_index;
    uint8_t bit_index;
} t_bitmap_index;


static inline t_bitmap_index to_bitmap_index(uint8_t index) {
    t_bitmap_index bi;
    bi.byte_index = index >> 3;
    bi.bit_index = index - (bi.byte_index << 3);
    return bi;
}

static inline void bitmap_index_inc(t_bitmap_index *index) {
    if (index->bit_index > 7) {
        index->byte_index++;
        index->bit_index = 0;
    }
    else {
        index->bit_index++;
    }
}

static inline bool bitmap_get(uint8_t* bitmap, t_bitmap_index index) {
   return bitmap[index.byte_index] & (1 << index.bit_index);
}

static inline void bitmap_set(uint8_t* bitmap, t_bitmap_index index) {
    bitmap[index.byte_index] = 
        bitmap[index.byte_index] | (1 << index.bit_index);
}


#endif
