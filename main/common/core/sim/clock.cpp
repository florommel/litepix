/*
 * clock.cpp
 * This file is part of Litepix
 *
 * Copyright (C) 2015 - Florian Rommel
 *
 * Litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Litepix is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Litepix. If not, see <http://www.gnu.org/licenses/>.
 */


#include <glib.h>
#include "../clock.hpp"


static const uint8_t TimerInc = 4;

static volatile uint16_t milliseconds;
static GMutex milliseconds_mutex;


#include <stdio.h>
static gpointer timer_inc_thread_func(gpointer data) {
    while (1) {
        g_mutex_lock(&milliseconds_mutex);
        milliseconds += TimerInc;
        g_mutex_unlock(&milliseconds_mutex);
        g_usleep(4000);
    }
    return NULL;
}


static void init_clock() __attribute__((constructor));
static void init_clock() {
    g_thread_new("timer_thread", timer_inc_thread_func, NULL);
}


uint16_t Clock::short_time_ms() {
    uint16_t tmp;
    
    g_mutex_lock(&milliseconds_mutex);
    tmp = milliseconds;
    g_mutex_unlock(&milliseconds_mutex);
    
    return tmp;
}
