/*
 * anim_dice.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Michael Nieß
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


#include <stdlib.h>
#include "main.h"
#include "core/timer.h"
#include "core/pix.h"
#include "util/transitions.h"
#include "util/setter.h"

//Create a Dice Number on a 9x9 Panel
static void fillNumber(uint8_t* canvas, uint8_t number, uint8_t *color, uint8_t *bg_color){
	set_full(canvas, bg_color);
	if(number % 2 == 1){
		set_pixel(canvas,4,color);
	}
	if(number != 1){
		set_pixel(canvas,0,color);
		set_pixel(canvas,8,color);
	}
	if(number >= 4){
		set_pixel(canvas,2,color);
		set_pixel(canvas,6,color);
	}
	if(number == 6){
		set_pixel(canvas,3,color);
		set_pixel(canvas,5,color);
	}
}

//Count from 1 to 6 
void anim_dice(uint32_t duration_fade, uint32_t duration_hold, uint8_t *color, uint8_t *bg_color) {
    
    uint8_t buffer[PIX_NUM_BYTES];
    uint8_t i;
    

    
    for (i = 1 ; i <= 6; i += 1)
    {
    	fillNumber(buffer,i,color,bg_color);
    	tr_fade_p(buffer,duration_fade,NULL);
 	
 		t_timer timer = TIMER_INIT(duration_hold);
    	while(!timer_test(&timer, duration_hold)){};
    	
    	set_full(buffer,bg_color);
    	tr_fade_p(buffer,duration_fade,NULL);	
    }
}

