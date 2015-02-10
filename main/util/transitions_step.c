/*
 * transitions_step.c
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
#include "transitions_step.h"
#include "../core/pix.h"


t_tr_state get_tr_state(uint8_t step_count) {
    t_tr_state state;
    state.step_count = step_count;
    state.step_curr = 0;
    state.d_sum = 0;
    return state;
}


bool tr_fade_step_p(t_tr_state* state, uint8_t* pic) {
    if (state->step_curr >= state->step_count) return false;
    
    uint16_t count;
    uint8_t* p_dest = pix_canvas;
    uint8_t* p_src = pic;
    uint8_t x = state->step_count - state->step_curr;
    for (count = PIX_NUM_BYTES; count > 0; count--) {
        
        int16_t tmp = (*p_src - *p_dest) / x;
        *p_dest += tmp;
        
        p_dest++;
        p_src++;
    }
    
    state->step_curr++;
    return true;
}


// TODO: rand() ????
void tr_shuffle(uint8_t *arr, uint8_t size) {
    uint8_t i;
    for (i = 0; i < size - 1; i++) {
        uint8_t j = i + rand() / (RAND_MAX / (size-i) + 1);
        uint8_t t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
    }
}


bool tr_dissolve_step_p(t_tr_state* state, uint8_t* pic, uint8_t* order) {
    if (state->step_curr >= state->step_count) return false;
    
    uint8_t d = (PIX_NUM_PIXELS - state->d_sum)
        / (state->step_count - state->step_curr);
    
    uint16_t i = state->d_sum; // TODO eleminate i
    uint8_t n;
    for (n = d; n > 0; n--) {
        uint16_t p = order[i] * 3;
        pix_canvas[p] = pic[p];
        pix_canvas[p+1] = pic[p+1];
        pix_canvas[p+2] = pic[p+2];
        i++;
    }
    
    state->step_curr++;
    state->d_sum += d;
    
    return true;
}
