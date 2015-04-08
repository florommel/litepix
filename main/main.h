/*
 * main.h
 * This file is part of litepix
 *
 * Copyright (C) 2015 - Florian Rommel, Michael Nieß
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdint.h>

void anim_dice(uint32_t duration_fade, uint32_t duration_hold, uint8_t *color, uint8_t *bg_color);

void anim_swirl(uint8_t color[3],uint32_t duration, uint8_t start);
#endif
