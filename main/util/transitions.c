/*
 * transitions.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel, Michael Nieß
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
#include "transitions.h"
#include "../core/pix.h"
#include "../core/timer.h"


#define TWO_HOURS 7200000
#define BYTE_MAX 255
#define BYTE_SIZE 256
#define DEFAULT_SAMPLE 16


// TODO random??
static void shuffle(uint8_t *arr, uint8_t size) {
    uint8_t i;
    for (i = 0; i < size - 1; i++) {
        uint8_t j = i + rand() / (RAND_MAX / (size-i) + 1);
        uint8_t t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
    }
}

//Fade from current canvas to a given canvas in a specific time
void tr_fade_p(uint8_t* pic, uint32_t duration, uint8_t* mask) {
    uint8_t step = 0;    
    t_timer timer = TIMER_INIT(16);
    
    while (step < (duration / 16)) {
    
        if (timer_test(&timer, 16)) {
        
            uint8_t i;
            uint8_t* p_dest = pix_canvas;
            uint8_t* p_src = pic;
            uint8_t x = (duration / 16) - step;
            
            for (i = 0; i < PIX_NUM_PIXELS; i++) {
                if ((mask == NULL) || bitmap_get(mask, i)) {
                    int16_t tmp;
                    
                    tmp = (*p_src - *p_dest) / x;
                    *p_dest += tmp;
                    p_dest++;
                    p_src++;
                    
                    tmp = (*p_src - *p_dest) / x;
                    *p_dest += tmp;
                    p_dest++;
                    p_src++;
                    
                    tmp = (*p_src - *p_dest) / x;
                    *p_dest += tmp;
                    p_dest++;
                    p_src++;
                }
                else {
                    p_dest += 3;
                    p_src += 3;
                }
            }
            
            step++;
            pix_render();
        }
    }
}

//switch random pixels to new canvas-pixels until all pixels are changed
void tr_dissolve_p(uint8_t* pic, uint32_t duration) {   
    uint8_t i;
    uint8_t order[PIX_NUM_PIXELS];
    uint32_t interval = duration / PIX_NUM_PIXELS;
    for (i = 0; i < PIX_NUM_PIXELS; i++) {
    	order[i] = i;
    }
    shuffle(order, PIX_NUM_PIXELS);
    
    t_timer timer = TIMER_INIT(0);
    i = 0;
    while (i < PIX_NUM_PIXELS) {
        if (timer_test(&timer, interval)) {            
        	uint16_t p = order[i] * 3;
            pix_canvas[p] = pic[p];
            pix_canvas[p+1] = pic[p+1];
            pix_canvas[p+2] = pic[p+2];
                
            i++;
            pix_render();
        }
    }
}
