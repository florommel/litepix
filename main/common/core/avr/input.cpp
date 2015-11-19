/*
 * input.cpp
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
#include "../input.hpp"


static const uint8_t MasterSendRequest = 0x01;
static const uint8_t MasterClearRequest = 0x02;

static uint8_t first_byte;
static bool first_byte_received = false;
static Delegate<void(Input)> input_handler;
static uint8_t request = MasterSendRequest;

static void init_serial(void) __attribute__((constructor));
static void init_serial(void) {
    cli();

    // set baud rate
    UBRR0 = 0x19; // baudrate 38400

    // set synchronous mode and frame format: 8data, 2stop bit
    UCSR0C = (1 << UMSEL00) | (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);

    // enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // XCK pin as ouput
    DDRD |= (1 << PD4);

    // TODO enable parity bit generation

    sei();
}


void Input::handle() {
    if (input_handler.empty()) return;

    if (UCSR0A & (1 << RXC0)) {
        uint8_t received = UDR0;
        if (!first_byte_received) {
            first_byte = received;
            first_byte_received = true;
        }
        else {
            input_handler(Input(first_byte, (Data)received));
            if (request == 0) request = MasterSendRequest;
            first_byte_received = false;
        }
    }

    if ((request != 0) && (UCSR0A & (1 << UDRE0))) {
        UDR0 = request;
        request = 0;
    }
}


void Input::set_handler(Delegate<void(Input)> handler) {
    input_handler = handler;
}


void Input::clear_handler() {
    input_handler.clear();
}


void Input::clear_events() {
    // TODO
    //request = MasterClearRequest;
}
