/*
 * timer.cpp
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


#include "timer.hpp"
#include "clock.hpp"


Timer* Timer::p_first = 0;


void Timer::handle() {
    Timer* p = p_first;
    while (p != 0) {
        uint16_t curr_time = Clock::short_time_ms();
        {
            uint16_t a = curr_time - p->timestamp;
            uint16_t b = p->timestamp - curr_time;
            if (a > b) return;
        }
        p->delegate();
        p->timestamp = curr_time + p->interval;
        p = p->p_next;
    }
}


Timer::Timer()
    : interval(1000), timestamp(Clock::short_time_ms() + interval) {}


Timer::Timer(int16_t interval)
    : interval(interval), timestamp(Clock::short_time_ms() + interval) {}


Timer::Timer(Delegate<void()> delegate)
    : delegate(delegate), interval(1000),
      timestamp(Clock::short_time_ms() + interval) {}


Timer::Timer(Delegate<void()> delegate, int16_t interval)
    : delegate(delegate), interval(interval),
      timestamp(Clock::short_time_ms() + interval) {}


Timer::~Timer() noexcept {
    stop();
}


bool Timer::is_running() const {
    Timer* p = p_first;
    while (p != 0) {
        if (p == this) return true;
        p = p->p_next;
    }
    return false;
}


void Timer::start() {
    if (delegate.empty()) return;

    if (p_first == 0) {
        p_first = this;
    } else {
        if (p_first == this) return;
        Timer* p = p_first;
        while (p->p_next != 0) {
            if (p->p_next == this) return;
            p = p->p_next;
        }
        p->p_next = this;
    }
    this->p_next = 0;
    timestamp = Clock::short_time_ms() + interval;
}


void Timer::stop() {
    if (p_first == 0) return;

    if (p_first == this) {
        p_first = this->p_next;
    } else {
        Timer* p = p_first;
        while (p->p_next != 0) {
            if (p->p_next == this) {
                p->p_next = this->p_next;
                return;
            }
            p = p->p_next;
        }
    }
}
