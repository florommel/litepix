/*
 * transition.cpp
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


#include "transition.hpp"
#include "core/random.hpp"
#include "core/clock.hpp"


static const uint8_t ByteMax = 255;
static const uint16_t ByteSize = 256;
static const uint8_t DefaultSample = 20;


static inline int16_t abs(int16_t x) {
    return (x ^ (x >> 15)) - (x >> 15);
}


static void shuffle(uint8_t* arr, uint8_t size, uint16_t seed) {
    Random::seed(seed);
    for (uint8_t i = 0; i < size - 1; ++i) {
        uint8_t j = Random::get_uint() % size;
        uint8_t t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
    }
}


static uint8_t ones(Mask* mask) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < Canvas::Pixels; ++i) {
        if ((*mask)[i]) ++count;
    }
    return count;
}


void Transition::fade(uint16_t duration, Delegate<void()> finished_action) {
    if (dest == nullptr) return;
    set_common_parameters(duration, finished_action);
    timer.set_delegate(DELEGATE(this, fade_step));
    timer.start();
}


void Transition::roll(Direction direction, uint16_t duration,
                      Delegate<void()> finished_action) {
    if (dest == nullptr) return;
    set_common_parameters(duration, finished_action);
    timer.set_delegate(DELEGATE(this, roll_step));
    timer.start();
    ext.direction = direction;
}


void Transition::dissolve(uint16_t duration, Delegate<void()> finished_action) {
    if (dest == nullptr) return;
    set_common_parameters(duration, finished_action);
    timer.set_delegate(DELEGATE(this, dissolve_step));
    timer.start();
    Random::seed(Clock::short_time_ms());
    ext.seed = Random::get_uint();
}


void Transition::set_common_parameters(uint16_t duration,
                                       Delegate<void()> finished_action) {
    this->finished_action = finished_action;
    
    if (duration >= DefaultSample * ByteSize) {
        step_count = ByteMax;
        timer.set_interval(duration / ByteSize);
    } else {
        timer.set_interval(DefaultSample);
        step_count = duration / DefaultSample;
    }
    step = 0;
}


void Transition::finish_step() {
    ++step;
    dest->render();
    if (step >= step_count) {
        timer.stop();
        if(!finished_action.empty()) finished_action();
    }
}


void Transition::fade_step() {
    {   
        uint8_t* p_dest = dest->get_buffer();
        uint8_t* p_src = src.canvas->get_buffer();
        uint8_t x = step_count - step;
        if (x == 0) x = 1;
        
        for (uint8_t i = 0; i < Canvas::Pixels; ++i) {
            if ((mask == nullptr) || ((*mask)[i])) {
                if (is_color) p_src = src.color_buffer;
                for (uint8_t n = 3; n > 0; --n) {
                    int16_t diff = *p_src - *p_dest;
                    if (diff != 0) {
                        if (x <= abs(diff)) {
                            *p_dest += diff / x;
                        } else if (x % diff == 0) {
                            *p_dest += (diff < 0) ? -1 : 1;
                        }
                    }
                    ++p_dest;
                    ++p_src;
                }
            }
            else {
                p_dest += 3;
                p_src += 3;
            }
        }
    }
    finish_step();
}


void Transition::roll_step() {
    {
        uint8_t row_size, dist, ib;
        int8_t x;
        {
            uint8_t col_dist, substep_count;
            if (ext.direction <= Direction::RightLeft) {
                row_size = Canvas::Height;
                dist = Canvas::Width;
                col_dist = 1;
                substep_count = step_count / Canvas::Width;
                if (substep_count == 0) substep_count = 1;
                step_count = substep_count * Canvas::Width;
            }
            else {
                row_size = Canvas::Width;
                dist = 1;
                col_dist = Canvas::Width;
                substep_count = step_count / Canvas::Height;
                if (substep_count == 0) substep_count = 1;
                step_count = substep_count * Canvas::Height;
            }
            
            uint8_t column = step / substep_count;
            uint8_t substep = step - (column * substep_count);
            x = substep_count - substep;
            
            if (ext.direction == Direction::RightLeft)
                column = Canvas::Width - column - 1;
            else if (ext.direction == Direction::BottomTop)
                column = Canvas::Height - column - 1;
            
            ib = column * col_dist;
        }
        
        for (uint8_t i = 0; i < row_size; ++i) {
            if ((mask == nullptr) || ((*mask)[ib])) {
                uint8_t *p1 = &(dest->get_buffer()[ib * 3]);
                uint8_t *p2;
                if (is_color) p2 = src.color_buffer;
                else p2 = &(src.canvas->get_buffer()[ib * 3]);
                *p1 += (*p2 - *p1) / x;
                ++p1; ++p2;
                *p1 += (*p2 - *p1) / x;
                ++p1; ++p2;
                *p1 += (*p2 - *p1) / x;
            }
            ib += dist;
        }
    }
    finish_step();
}


void Transition::dissolve_step() {
    {
        uint8_t count = (mask == nullptr) ? Canvas::Pixels : ones(mask);
        uint8_t order[count];
        for (uint8_t i = 0, j = 0; i < Canvas::Pixels; ++i) {
            if ((mask == nullptr) || ((*mask)[i])) order[j++] = i;
        }
        shuffle(order, count, ext.seed | (step_count << 8));
        
        uint8_t d_sum, d;
        if (step_count == 0) {
            d_sum = 0;
            d = count + 1;
        } else {
            d_sum = ((uint16_t)count * step) / step_count;
            d = (count - d_sum) / (step_count - step) + 1;
        }
        
        for (; d > 0; --d) {
            if (d_sum >= count) break;
            uint16_t x = order[d_sum];
            if (is_color) dest->set_pixel(x, Color(src.color_buffer[1],
                                    src.color_buffer[0], src.color_buffer[2]));
            else dest->set_pixel(x, src.canvas->get_pixel(x));
            ++d_sum;
        }
    }
    finish_step();
}
