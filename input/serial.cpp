/*
 * serial.cpp
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


#include "serial.hpp"
#include "safe_queue.hpp"


namespace {
    
    static const uint8_t MasterSendRequest = 0x01;
    static const uint8_t MasterClearRequest = 0x02;
    
    uint8_t request = 0;
    static SafeQueue<Input, 12> send_queue;
    
};


void serial::init() {
    // set synchronous mode and frame format: 8data, 2stop bit
    UCSRC = (1 << UMSEL) | (1 << USBS) | (1 << UCSZ1) | (1 << UCSZ0);
        
    // enable receiver and transmitter
    UCSRB = (1 << RXEN) | (1 << TXEN);
}


void serial::handle() {
    if (UCSRA & (1 << RXC)) {
        request = UDR;
    }
    
    switch (request) {
        case MasterSendRequest:
            {
                ValueType<Input> v = send_queue.pop();
                if (v.has_value) {
                    while (!(UCSRA & (1 << UDRE)));
                    UDR = v.value.device;
                    while (!(UCSRA & (1 << UDRE)));
                    UDR = v.value.data;
                    request = 0;
                }
            }
            break;
        case MasterClearRequest:
            send_queue.clear();
            request = 0; // TODO!!!!!!!!
            break;
    }
}


void serial::queue(Input input) {
    send_queue.push(input);
}
