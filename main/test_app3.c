/*
 * test_app3.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel
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
#include "util/transitions_block.h"


static void fill(uint8_t* canvas, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = g;
        *p++ = r;
        *p++ = b;
    }
}


void test_app3(void) {
    uint8_t canvas[PIX_NUM_BYTES];
    
    fill(pix_canvas, 0, 150, 250);
    
    while (1) {
        fill(canvas, 255, 0, 0);
        tr_fade_p(canvas, 200);
        fill(canvas, 0, 150, 250);
        tr_fade_p(canvas, 20000);
    }
}

