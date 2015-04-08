/*
 * main.c
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel, Michael Nieß
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
#include "core/init.h"
#include "core/timer.h"
#include "main.h"
#include "util/setter.h"
#include "util/transitions.h"

//do nothing for some milliseconds
static void pause(uint32_t milliseks){
	t_timer timer = TIMER_INIT(milliseks);
    while(!timer_test(&timer, 0)){};
}

int main(void) {
    init_pix();
    init_clock();
    
    uint8_t i,j;
    uint8_t buffer[PIX_NUM_BYTES];
    //Some predefined Colors
    uint8_t rainbow[][3] = {{255,255,000}, //0 yellow
    						{255,125,000}, //1 orange
    						{255,000,000}, //2 red
    						{255,000,125}, //3 pink
    						{255,000,255}, //4 purple
    						{125,000,255}, //5 half-blue
    						{000,000,255}, //6 blue
    						{000,125,255}, //7 lightblue
    						{000,255,255}, //8 turkish
    						{000,255,125}, //9 half-green
    						{000,255,000}, //10 green
    						{125,255,000}}; //11 light green
    uint8_t black[] = {0,0,0};
    
    uint8_t warmwhite[] = {255,255,220};
    uint8_t blacklight[]= {10,0,10};
    
    //Mask direkt to save program-space
    uint8_t mask_fife[TR_MASK_SIZE] = {85,255};
    uint8_t mask_not_fife[TR_MASK_SIZE] = {170,0};
    
    while(1){
    
    	//DO Swirlanimation with every color in every direction
		for (j = 0; j < 9; j += 2) //15
		{
			for (i = 0; i < 12; i++)
			{
				anim_swirl(rainbow[i],2500,j);
			}
		}
		//Pink Dice
    	anim_dice(1000,2000,rainbow[3],black);
    	//Dissolve Rainbowcolor
    	for (i = 0; i < 12; i++) //12
		{
			set_full(buffer,rainbow[i%12]);
			tr_dissolve_p(buffer,6000);
		}
		//Green Dice
		anim_dice(1000,2000,rainbow[11],black);
		//Random Dissolve
		for (i = 0; i < 10; i++) //10
		{
			set_random(buffer);
			tr_dissolve_p(buffer,6000);
			
			pause(1000);
    		
			set_random(buffer);
			tr_fade_p(buffer,2000,NULL);
			
			pause(2000);
		}
		//Another Dice
		anim_dice(1000,2000,rainbow[8],black);
		
		//Mask random fade
		for (i = 0; i < 10; i++) //10
		{
			uint8_t randCol[] = {rand()%256,rand()%256,rand()%256};
			set_full(buffer,randCol);
			tr_fade_p(buffer,1000,mask_fife);
			
			pause(2000);
			
			uint8_t randCol2[] = {rand()%256,rand()%256,rand()%256};
			set_full(buffer,randCol2);
			tr_fade_p(buffer,1000,mask_not_fife);
			
			pause(2000);
		}
		//White Dice
		anim_dice(1000,2000,warmwhite,blacklight);
		//Random blink
		for (i = 0; i < 10; i++) //10
		{
			uint8_t randCol[] = {rand()%256,rand()%256,rand()%256};
			uint8_t randIndex = rand() % PIX_NUM_PIXELS;
			
			set_pixel(buffer,randIndex,randCol);
			tr_fade_p(buffer,1500,NULL);
			
			pause(1500);
			
			set_full(buffer,black);
			tr_fade_p(buffer,1500,NULL);
			
			pause(500);
		}
    }
    return 0;
}
