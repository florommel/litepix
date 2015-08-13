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


#include <avr/interrupt.h>
#include <util/atomic.h>
#include "../clock.hpp"


static const uint8_t TimerMax = 250;
static const uint8_t TimerInc = 4;

static volatile uint16_t milliseconds;


static void init_timer(void) __attribute__((constructor));
static void init_timer(void) {
    TCCR0A |= (1 << WGM01); // CTC mode
 	TCCR0B |= (1 << CS02);  // prescaler: 256
 	OCR0A = TimerMax;
 	TIMSK0 |= (1 << OCIE0A);
 	sei();
}


ISR(TIMER0_COMPA_vect) {
    milliseconds += TimerInc;
}


uint16_t Clock::short_time_ms(void) {
    uint16_t tmp;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        tmp = milliseconds;
    }
    
    return tmp;
}
