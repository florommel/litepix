/*
 * transitions_block.h
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel
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


#ifndef _UTIL__TRANSITIONS_BLOCK_H_
#define _UTIL__TRANSITIONS_BLOCK_H_

#include <stdint.h>

void tr_fade_p(uint8_t* pic, uint32_t duration);

void tr_dissolve_p(uint8_t* pic, uint32_t duration);

void tr_roll_p(uint8_t* pic, uint32_t duration);

#endif
