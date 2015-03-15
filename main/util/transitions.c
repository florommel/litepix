/*
 * transitions.c
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


static void calculate_parameters(uint32_t duration,
        int16_t* interval, uint8_t* steps) {
    if (duration > TWO_HOURS) duration = TWO_HOURS;
    
    if (duration >= DEFAULT_SAMPLE * BYTE_SIZE) {
        *steps = BYTE_MAX;
        float f = duration;
        f = f / BYTE_SIZE;
        *interval = f + 0.5;
    } else {
        *interval = DEFAULT_SAMPLE;
        float f = duration;
        f = f / DEFAULT_SAMPLE;
        *steps = f + 0.5;
    }
}


void tr_fade_p(uint8_t* pic, uint32_t duration, uint8_t* mask) {
    uint8_t step = 0;
    uint8_t step_count;
    int16_t interval;
    calculate_parameters(duration, &interval, &step_count);
    
    t_timer timer = TIMER_INIT(interval);
    
    while (step < step_count) {
    
        if (timer_test(&timer, interval)) {
        
            uint8_t i;
            uint8_t* p_dest = pix_canvas;
            uint8_t* p_src = pic;
            uint8_t x = step_count - step;
            
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


void tr_dissolve_p(uint8_t* pic, uint32_t duration, uint8_t* mask) {
    uint8_t step = 0;
    uint8_t d_sum = 0;
    uint8_t step_count;
    int16_t interval;
    calculate_parameters(duration, &interval, &step_count);
    
    uint8_t i, j;
    uint8_t order_size = 0;
    if (mask == NULL) order_size = PIX_NUM_PIXELS;
    else {
        for (i = 0; i < PIX_NUM_PIXELS; i++) {
            if (bitmap_get(mask, i)) order_size++;
        }
    }
    uint8_t order[order_size];
    for (i = 0, j= 0; i < PIX_NUM_PIXELS; i++) {
        if ((mask == NULL) || bitmap_get(mask, i)) {
            order[j] = i;
            j++;
        }
    }
    shuffle(order, order_size);
    
    t_timer timer = TIMER_INIT(interval);
    
    while (step < step_count) {
        if (timer_test(&timer, interval)) {  
            uint8_t d = (order_size - d_sum) / (step_count - step);
            uint16_t i = d_sum;
            uint8_t n;
            
            for (n = d; n > 0; n--) {
                uint16_t p = order[i] * 3;
                pix_canvas[p] = pic[p];
                pix_canvas[p+1] = pic[p+1];
                pix_canvas[p+2] = pic[p+2];
                i++;
            }
            
            step++;
            d_sum += d;
            pix_render();
        }
    }
}


void tr_roll_p(uint8_t* pic, uint32_t duration, tr_direction direction,
               uint8_t* mask) {
    uint8_t step = 0;
    uint8_t step_count;
    int16_t interval;
    calculate_parameters(duration, &interval, &step_count);
    
    // calculate direction dependant vars
    uint8_t run_along, inc1, inc2, substep_count;
    if (direction <= tr_right_left) {
        run_along = PIX_HEIGHT;
        inc1 = PIX_WIDTH;
        inc2 = 1;
        substep_count = step_count / PIX_WIDTH;
    }
    else {
        run_along = PIX_WIDTH;
        inc1 = 1;
        inc2 = PIX_WIDTH;
        substep_count = step_count / PIX_HEIGHT;
    }
    
    /*if (substep_count == 0) {
        substep_count = 1;
        step_count = inc1;
    }*/ // TODO
    
    t_timer timer = TIMER_INIT(interval);
    
    while (step < step_count) {
        if (timer_test(&timer, interval)) {
            uint8_t masterstep = step / substep_count;
            uint8_t substep = step - (masterstep * substep_count);
            int8_t x = substep_count - substep;
            
            if (direction == tr_right_left)
                masterstep = PIX_WIDTH - masterstep - 1;
            else if (direction == tr_bottom_top)
                masterstep = PIX_HEIGHT - masterstep - 1;
            
            uint8_t i, ib = masterstep * inc2;
            for (i = 0; i < run_along; i++) {
                if ((mask == NULL) || bitmap_get(mask, ib)) {
                    uint8_t *p1 = &pix_canvas[ib * 3];
                    uint8_t *p2 = &pic[ib * 3];
                    *p1 += (*p2 - *p1) / x;
                    p1++; p2++;
                    *p1 += (*p2 - *p1) / x;
                    p1++; p2++;
                    *p1 += (*p2 - *p1) / x;
                }
                ib += inc1;
            }
            
            step++;
            pix_render();
        }
    }
}
