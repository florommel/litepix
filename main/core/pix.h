/*
 * pix.h
 * This file is part of litepix
 *
 * Copyright (C) 2014, 2015 - Florian Rommel
 *
 * litepix is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
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


#ifndef _CORE__PIX_H_
#define _CORE__PIX_H_


#include <stdint.h>


#define PIX_NUM_PIXELS (240)
#define PIX_WIDTH (20)
#define PIX_HEIGHT (12)

#define PIX_NUM_BYTES  (PIX_NUM_PIXELS*3)


extern uint8_t pix_canvas[PIX_NUM_BYTES];


void pix_render(void);


#endif
