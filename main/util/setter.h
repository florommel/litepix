/*
 * setter.h
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Michael Nieß
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


#ifndef _UTIL__SETTER_H_
#define _UTIL__SETTER_H_

#include <stdint.h>

void set_pixel(uint8_t* pic, uint8_t index, uint8_t color[3]);

void set_full(uint8_t* pic, uint8_t color[3]);

void set_random(uint8_t* pic);

#endif
