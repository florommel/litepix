/*
 * setter.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Michael Nieß
 *
 * litepix is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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


#include <stdlib.h>
#include <stdbool.h>
#include "setter.h"
#include "../core/pix.h"

//Set a pixels color
void set_pixel(uint8_t* canvas, uint8_t index, uint8_t color[3]){
	uint8_t i = index * 3;
	canvas[i++] = color[1];
	canvas[i++] = color[0];
	canvas[i] = color[2];
}

//Set the whole canvas to one color
void set_full(uint8_t* canvas, uint8_t color[3]) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = color[1];
        *p++ = color[0];
        *p++ = color[2];
    }
}
//Set random colors in whole canvas
void set_random(uint8_t* canvas){
	    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = rand() % 256;
        *p++ = rand() % 256;
        *p++ = rand() % 256;
    }
}

