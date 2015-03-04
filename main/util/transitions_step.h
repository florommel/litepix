/*
 * transitions_step.h
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


#ifndef _UTIL__TRANSITIONS_STEP_H_
#define _UTIL__TRANSITIONS_STEP_H_

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    uint8_t step_count;
    uint8_t step_curr;
    uint8_t d_sum;
} t_tr_state;


t_tr_state get_tr_state(uint8_t step_count);

void tr_shuffle(uint8_t *arr, uint8_t size); // TODO

bool tr_fade_step_p(t_tr_state* state, uint8_t* pic);

bool tr_dissolve_step_p(t_tr_state* state, uint8_t* pic, uint8_t* order);

bool tr_roll_step_p(t_tr_state* state, uint8_t* pic);

#endif
