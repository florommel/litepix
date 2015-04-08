/*
 * pix.c
 * This file is part of litepix
 *
 * Copyright (C) 2014, 2015 - Florian Rommel, Michael Nieß
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


#include <avr/io.h>
#include <avr/interrupt.h>
#include "../pix.h"
#include "../init.h"


#define OUT_PORT  (PORTB)
#define OUT_DR    (DDRB)
#define OUT_PIN	  (PB4)

#define START     (0)
uint8_t pix_canvas[PIX_NUM_BYTES] = {0};

//switch pixel 3 with 5
static void rearrange(void) {  
    uint8_t tmp1 = pix_canvas[9];
    uint8_t tmp2 = pix_canvas[10];
    uint8_t tmp3 = pix_canvas[11];
    pix_canvas[9] = pix_canvas[15];
    pix_canvas[10] = pix_canvas[16];
    pix_canvas[11] = pix_canvas[17];
    pix_canvas[15] = tmp1;
    pix_canvas[16] = tmp2;
    pix_canvas[17] = tmp3;
}


void init_pix(void) {
    OUT_DR |= (1 << OUT_PIN);
    OUT_PORT &= ~(1 << OUT_PIN);
}

void pix_render(void) {
    rearrange();    
    uint8_t tmp_sreg = SREG;
    cli();

    uint8_t *p = &pix_canvas[START];
    uint8_t d = *p++;

    const uint8_t mask = _BV(OUT_PIN);
    const uint8_t low = OUT_PORT & (~mask);
    const uint8_t high = OUT_PORT | mask;

    uint8_t nbits = 7;
    uint8_t tmp = low;
    uint8_t nbytes = PIX_NUM_BYTES;

    asm volatile(    
        "set1:   out %[ioport], %[portdown]\n\t"	
        "        lsl %[data1]\n\t"					
        "        brcc a1\n\t"						
        "        sbr %[tmp], %[bit1]\n\t"			
        "a1:     out %[ioport], %[portup]\n\t"		
        "        nop\n\t"							
        "        dec %[bitcount]\n\t"				
        "set0:   out %[ioport], %[tmp]\n\t"			
        "        mov %[tmp], %[portdown]\n\t"		 
        "        brne set1\n\t"						
        "	 nop\n\t"								
        "b1:     out %[ioport], %[portdown]\n\t"	
        "        lsl %[data1]\n\t"					
        "        brcc b2\n\t"						
        "        sbr %[tmp], %[bit1]\n\t"			
        "b2:     ld %[data1], %a[ptr1]+\n\t"		
                 // ^^^
        "        nop\n\t"							
        "        out %[ioport], %[portup]\n\t"		
        "        nop\n\t"							
        "        nop\n\t"							
        "b3:     ldi %[bitcount], 7\n\t"
        "        out %[ioport], %[tmp]\n\t"
        "        mov %[tmp], %[portdown]\n\t"
        " 	 subi %[bytecount], 1\n\t"
        " 	 brne set1\n\t"
        "        out %[ioport], %[portdown]\n\t"
        ::
        [ioport]    "I" (_SFR_IO_ADDR(OUT_PORT)),
        [portup]    "l" (high),
        [portdown]  "l" (low),
        [bitcount]  "d" (nbits),
        [ptr1]      "e" (p),
        [data1]     "d" (d),
        [bit1]      "I" (1 << OUT_PIN),
        [tmp]       "d" (tmp),
        [bytecount] "d" (nbytes)
    );
    
    SREG = tmp_sreg;
    
    rearrange();
}

