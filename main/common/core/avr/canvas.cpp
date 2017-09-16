/*
 * pix.cpp
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
#include "../canvas.hpp"


#define OUT_PORT  (PORTC)
#define OUT_DR    (DDRC)
#define OUT_PIN_1 (PC2)
#define OUT_PIN_2 (PC1)
#define OUT_PIN_3 (PC0)

#define START_1   (0)
#define START_2   (Canvas::Bytes/3)
#define START_3   (Canvas::Bytes/3*2)


static void init_pix() __attribute__((constructor));
static void init_pix() {
    OUT_DR |= (1 << OUT_PIN_1);
    OUT_DR |= (1 << OUT_PIN_2);
    OUT_DR |= (1 << OUT_PIN_3);
    OUT_PORT &= ~(1 << OUT_PIN_1);
    OUT_PORT &= ~(1 << OUT_PIN_2);
    OUT_PORT &= ~(1 << OUT_PIN_3);
}


static void rearrange(uint8_t buffer[]) {
#ifdef REVERSE_VERTICAL
    uint8_t *ps = &buffer[0];
    uint8_t *pe = &buffer[60];
#else
    uint8_t *ps = &buffer[60];
    uint8_t *pe = &buffer[120];
#endif
    asm (
        "        ldi r18, 10\n\t"
        "        ldi r19, 6\n\t"

        "rearr:  ld r20, %a[ps]\n\t"
        "        ldd r21, %a[ps]+1\n\t"
        "        ldd r22, %a[ps]+2\n\t"
        "        ld r25, -%a[pe]\n\t"
        "        ld r24, -%a[pe]\n\t"
        "        ld r23, -%a[pe]\n\t"
        "        st %a[pe], r20\n\t"
        "        std %a[pe]+1, r21\n\t"
        "        std %a[pe]+2, r22\n\t"
        "        st %a[ps]+, r23\n\t"
        "        st %a[ps]+, r24\n\t"
        "        st %a[ps]+, r25\n\t"
        "        dec r18\n\t"
        "        brne rearr\n\t"
        "        subi r28,-90\n\t"  //TODO
        "        sbci r29,-1\n\t"
        "        subi r30,-150\n\t" //TODO
        "        sbci r31,-1\n\t"
        "        ldi r18, 10\n\t"
        "        dec r19\n\t"
        "        brne rearr\n\t"
        ::
        [ps] "y" (ps),
        [pe] "z" (pe)
        : "r18", "r19", "r20", "r21",
          "r22", "r23", "r24", "r25"
    );
}


void Canvas::render() {
    rearrange(buffer);

    uint8_t *p1 = &buffer[START_1];
    uint8_t *p2 = &buffer[START_2];
    uint8_t *p3 = &buffer[START_3];
    uint8_t d1 = *p1++;
    uint8_t d2 = *p2++;
    uint8_t d3 = *p3++;

    const uint8_t mask = _BV(OUT_PIN_1) | _BV(OUT_PIN_2) | _BV(OUT_PIN_3);
    const uint8_t low = OUT_PORT & (~mask);
    const uint8_t high = OUT_PORT | mask;

    uint8_t nbits = 7;
    uint8_t tmp = low;
    uint16_t nbytes = Bytes/3;

    uint8_t tmp_sreg = SREG;
    cli();

    asm volatile(
        "start:  nop\n\t"
        "        nop\n\t"
        "        nop\n\t"
        "set1:   out %[ioport], %[portdown]\n\t"
        "        lsl %[data1]\n\t"
        "        brcc a1\n\t"
        "        sbr %[tmp], %[bit1]\n\t"
        "a1:     lsl %[data2]\n\t"
        "        brcc a2\n\t"
        "        sbr %[tmp], %[bit2]\n\t"
        "a2:     out %[ioport], %[portup]\n\t"
        "        lsl %[data3]\n\t"
        "        brcc a3\n\t"
        "        sbr %[tmp], %[bit3]\n\t"
        "a3:     nop\n\t"
        "        dec %[bitcount]\n\t"
        "set0:   out %[ioport], %[tmp]\n\t"
        "        mov %[tmp], %[portdown]\n\t"
        "        brne start\n\t"

        "        lsl %[data1]\n\t"
        "        brcc b1\n\t"
        "        sbr %[tmp], %[bit1]\n\t"
        "b1:     lsl %[data2]\n\t"
        "        out %[ioport], %[portdown]\n\t"
        "        brcc b2\n\t"
        "        sbr %[tmp], %[bit2]\n\t"
        "b2:     ld %[data1], %a[ptr1]+\n\t"
                 // ^^^
        "        ld %[data2], %a[ptr2]+\n\t"
                 // ^^^
        "        out %[ioport], %[portup]\n\t"
        "        lsl %[data3]\n\t"
        "        brcc b3\n\t"
        "        sbr %[tmp], %[bit3]\n\t"
        "b3:     ld %[data3], %a[ptr3]+\n\t"
                 // ^^^
        "        out %[ioport], %[tmp]\n\t"
        "        ldi %[bitcount], 7\n\t"
        "        mov %[tmp], %[portdown]\n\t"
        "        sbiw %[bytecount], 1\n\t"
                 // ^^^
        "        brne set1\n\t"

        "        nop\n\t"
        "        out %[ioport], %[portdown]\n\t"
        ::
        [ioport]    "I" (_SFR_IO_ADDR(OUT_PORT)),
        [portup]    "l" (high),
        [portdown]  "l" (low),
        [bitcount]  "d" (nbits),
        [ptr1]      "e" (p1),
        [ptr2]      "e" (p2),
        [ptr3]      "e" (p3),
        [data1]     "d" (d1),
        [data2]     "d" (d2),
        [data3]     "d" (d3),
        [bit1]      "M" (1 << OUT_PIN_1),
        [bit2]      "M" (1 << OUT_PIN_2),
        [bit3]      "M" (1 << OUT_PIN_3),
        [tmp]       "d" (tmp),
        [bytecount] "w" (nbytes)
    );

    SREG = tmp_sreg;

    rearrange(buffer);
}
