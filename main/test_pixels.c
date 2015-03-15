/*
 * test_pixels.c
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


#include "main.h"
#include "core/timer.h"
#include "core/pix.h"


void test_pixels(void) {
    uint16_t x = PIX_NUM_BYTES-3;
    
    t_timer timer = timer_get(0);
    
    while (1) {
        if (timer_test(&timer, 100)) {
            pix_canvas[x++] = 0;
            pix_canvas[x++] = 0;
            pix_canvas[x++] = 0;
            if (x >= PIX_NUM_BYTES) x = 0;
            pix_canvas[x] = 255;
            pix_canvas[x+1] = 255;
            pix_canvas[x+2] = 255;
            pix_render();
        }
    }
}
