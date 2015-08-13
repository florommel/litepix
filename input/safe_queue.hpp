/*
 * safe_queue.hpp
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


#ifndef _SAFE_QUEUE_HPP_
#define _SAFE_QUEUE_HPP_

#include <util/atomic.h>


template<typename T>
struct ValueType {
    bool has_value;
    T value;
    operator T() {
        return value;
    }
};


template<typename T, uint8_t Size>
class SafeQueue {
  public:
    void push(T value) {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            buffer[back_pos] = value;
            
            if ((front_pos == back_pos) && (!is_empty)) {
                if (front_pos == 0) front_pos = Size-1;
                else front_pos--;
            }
            
            if (back_pos == 0) back_pos = Size-1;
            else back_pos--;
            
            is_empty = false;
        }
    }
    
    ValueType<T> pop() {
        ValueType<T> ret;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            if (!is_empty) {
                ret.has_value = true;
                ret.value = buffer[front_pos];
                if (front_pos == 0) front_pos = Size-1;
                else front_pos--;
                if (front_pos == back_pos) is_empty = true;
            } else {
                ret.has_value = false;
            }
        }
        return ret;
    }
    
    void clear() {
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            back_pos = front_pos;
            is_empty = true;
        }
    }
    
  private:
    T buffer[Size];
    uint8_t front_pos = 0;
    uint8_t back_pos = 0;
    bool is_empty = true;
};

#endif
