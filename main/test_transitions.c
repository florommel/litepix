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
#include "util/transitions.h"


static void fill(uint8_t* canvas, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = g;
        *p++ = r;
        *p++ = b;
    }
}


static void fill_random(uint8_t* canvas) {
    uint8_t i;
    uint8_t* p = canvas;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
        *p++ = rand() % 256;
        *p++ = rand() % 256;
        *p++ = rand() % 256;
    }
}


void test_transitions(void) {
    // TODO -> no fixed number
    srand(46721);
    
    uint8_t buffer[PIX_NUM_BYTES];
    
    uint8_t mask[TR_MASK_SIZE] = {0};
    uint8_t i;
    for (i = 0; i < PIX_NUM_PIXELS; i += 3) {
        bitmap_set(mask, i);
    }
    
    while (1) {
        fill_random(buffer);
        tr_dissolve_p(buffer, 2000, NULL);
        
        fill(buffer, 0, 0, 0);
        tr_fade_p(buffer, 1000, mask);
        
        fill(buffer, 0, 0, 0);
        tr_fade_p(buffer, 1000, NULL);
        
        fill(buffer, 100, 50, 100);
        tr_dissolve_p(buffer, 2000, mask);
        
        fill_random(buffer);
        tr_dissolve_p(buffer, 2000, mask);
        
        tr_dissolve_p(buffer, 2000, NULL);
        
        fill(buffer, 255, 0, 0);
        tr_roll_p(buffer, 1400, NULL);
        
        fill(buffer, 0, 0, 0);
        tr_dissolve_p(buffer, 2000, mask);
        
        fill(buffer, 0, 0, 255);
        tr_fade_p(buffer, 2000, NULL);
        
        fill(buffer, 20, 0, 30);
        tr_fade_p(buffer, 5000, NULL);
        
        fill(buffer, 0, 0, 0);
        tr_roll_p(buffer, 4000, NULL);
    }
}

