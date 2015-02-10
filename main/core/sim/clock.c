/*
 * clock.c
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


#include <glib.h>
#include "../init.h"
#include "../clock.h"


#define TIMER_MAX 250
#define TIMER_INC 4


static volatile uint16_t milliseconds;
static GMutex milliseconds_mutex;


static gpointer timer_inc_thread_func(gpointer data) {
    while (1) {
        g_mutex_lock(&milliseconds_mutex);
        milliseconds += TIMER_INC;
        g_mutex_unlock(&milliseconds_mutex);
        g_usleep(4000);
    }
    return NULL;
}


void init_clock(void) {
 	g_thread_new("timer_thread", timer_inc_thread_func, NULL);
}


uint16_t clock_time_ms(void) {
    uint16_t tmp;
    
    g_mutex_lock(&milliseconds_mutex);
    tmp = milliseconds;
    g_mutex_unlock(&milliseconds_mutex);
    
    return tmp;
}
