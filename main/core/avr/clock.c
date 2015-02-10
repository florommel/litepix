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
 

#include <avr/interrupt.h>
#include <util/atomic.h>
#include "../init.h"
#include "../clock.h"

#define TIMER_MAX 250
#define TIMER_INC 4


static volatile uint16_t milliseconds;


ISR(TIMER0_COMPA_vect) {
    milliseconds += TIMER_INC;
}


void init_clock(void) {
 	TCCR0A |= (1 << WGM01); // CTC mode
 	TCCR0B |= (1 << CS02); // prescaler: 256
 	OCR0A = TIMER_MAX;
 	TIMSK0 |= (1 << OCIE0A);
 	sei();
}


uint16_t clock_time_ms(void) {
    uint16_t tmp;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = milliseconds;
    }
    
    return tmp;
}
