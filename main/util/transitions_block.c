/*
 * transitions_block.c
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


#include "transitions_block.h"
#include "transitions_step.h"
#include "../core/pix.h"
#include "../core/timer.h"


#define TWO_HOURS 7200000
#define BYTE_MAX 255
#define BYTE_SIZE 256
#define DEFAULT_SAMPLE 16


void tr_fade_p(uint8_t* pic, uint32_t duration) {
    t_tr_state state; // TODO get_tr_state verwenden
    state.step_curr = 0;
    int16_t interval;
    
    if (duration > TWO_HOURS) duration = TWO_HOURS;
    
    if (duration >= DEFAULT_SAMPLE * BYTE_SIZE) {
        state.step_count = BYTE_MAX;
        float f = duration;
        f = f / BYTE_SIZE;
        interval = f + 0.5;
    } else {
        interval = 16;
        float f = duration;
        f = f / DEFAULT_SAMPLE;
        state.step_count = f + 0.5;
    }
    
    t_timer timer = timer_get(interval);
    
    while (1) {
        if (timer_test(&timer, interval)) {
            if (!tr_fade_step_p(&state, pic)) return;
            pix_render();
        }
    }
}
