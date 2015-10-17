/*
 * ir.cpp
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include "ir.hpp"
#include "input.hpp"
#include "serial.hpp"


void ir::init() {
    // init timer
    TCCR1B = (1 << CS12);  //prescaler = 256
    
    // init pcint0
    GIMSK = (1 << PCIE);
    PCMSK = (1 << PCINT0);
    
    sei();
}


namespace {
    
    static const uint8_t StartS = 0;
    static const uint8_t SigP   = 1;
    static const uint8_t SigS   = 2;
    static const uint8_t Sig0   = 3;
    static const uint8_t Sig1   = 4;
    static const uint8_t Off    = 5;
    
    
    static void decode(uint8_t* sig) {
        if ((sig[0] != 0) || (sig[1] != 0xFF)) return;
        else if ((sig[2] ^ sig[3]) != 0xFF) return;
        
        Input::Data d;
        switch (sig[2]) {
            case 0x46: d = Input::Up; break;
            case 0x15: d = Input::Down; break;
            case 0x40: d = Input::Left; break;
            case 0x43: d = Input::Right; break;
            case 0x45: d = Input::Exit; break;
            case 0x44: d = Input::Ok; break;
            default: return;
        }
        
        serial::queue(Input(1, d));
    }
    

    static uint8_t transition(uint8_t st, uint16_t dt, uint8_t ps) {
        const uint8_t dt_diff = 16;
        const uint16_t valid_dt[] = {281, 141, 18, 18, 53};
        const uint8_t valid_ps = (1 << StartS) | (1 << SigS);
        ps = ps & 0x1;
        
        if (st <= SigS) ++st;
        else if (st <= Sig1) st = SigS;
        else st = StartS;
        
        while (true) {
            uint16_t tmp = valid_dt[st];
            if ((((valid_ps >> st) & 0x1) == ps) &&   //-> check ps
                (dt > tmp - dt_diff) && (dt < tmp + dt_diff)) //-> check dt
            {
                return st;
            }
            if (st == Sig0) ++st;
            else if (st != StartS) st = StartS;
            else break;
        }
        
        return Off;
    }
    
}


ISR(PCINT_vect) {
    static uint16_t time = 0;
    static uint8_t st = Off;
    
    uint16_t dt = TCNT1-time;
    time = TCNT1;
    GIMSK &= ~(1 << PCIE);
    sei();
    
    st = transition(st, dt, (PINB & (1 << PB0)));
    
    static uint8_t sig[4];
    static int8_t sigc;
    
    if (st == StartS) {
        sigc = -1;
        sig[0] = sig[1] = sig[2] = sig[3] = 0;
    }
    else if (st == SigS) {
        ++sigc;
        if (sigc == 32) decode(sig);
    }
    else if (st == Sig1) {
        if ((sigc >= 0) || (sigc <= 31)) {
            sig[sigc >> 3] |= (1 << (sigc & 0x7));
        }
    }
    
    cli();
    GIMSK = (1 << PCIE);
}
