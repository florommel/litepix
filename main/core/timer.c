/*
 * timer.c
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


#include "timer.h"


bool timer_test(t_timer* timer, int16_t new_interval) {
    if (!timer->_state) return false;

    uint16_t curr_time = clock_time_ms();
    uint16_t a = curr_time - timer->_next;
    uint16_t b = timer->_next - curr_time;
    
    if (a <= b) {
        timer->_next = curr_time + new_interval;
        timer->_state = (new_interval >= 0);
        return true;
    }
    return false;
}
