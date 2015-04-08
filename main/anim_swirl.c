/*
 * anim_swirl.c
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
#include "main.h"
#include "core/pix.h"
#include "util/transitions.h"
#include "util/setter.h"

//dim the current color
static void dim_color(uint8_t *color){
	color[0] = color[0] / 1.4;
	color[1] = color[1] / 1.4;
	color[2] = color[2] / 1.4;
}

//make a snake with fading colors
void anim_swirl(uint8_t *color, uint32_t duration, uint8_t start) {
    uint8_t buffer[PIX_NUM_BYTES];
    uint8_t i;
    uint8_t orig[] = {color[0], color[1], color[2]};
    uint8_t a[] = {0,1,2,5,8,7,6,3};
    for (i = start; i < start+8; i += 1)
    {
    	set_pixel(buffer,a[i%8],orig);
    	dim_color(orig);
    }
    set_pixel(buffer,4,orig);
    tr_fade_p(buffer,duration,NULL);
}


