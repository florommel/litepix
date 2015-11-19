/*
 * timer.hpp
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

/**
 * @file
 * scheduled task execution
 */

#ifndef _CORE__TIMER_HPP_
#define _CORE__TIMER_HPP_

#include <stdint.h>
#include "delegate.hpp"
#include "mainloop.hpp"


/**
 * A timer class to schedule task execution.
 * The timer delegate (task) is periodically executed.
 * Requires a running main loop (Mainloop::run()).
 */
class Timer final {
  public:

    /**
     * Initialize timer with empty delegate and
     * default interval (1000 milliseconds).
     */
    Timer();

    /**
     * Initialize timer with given interval and empty delegate.
     * @param   interval   timer interval in milliseconds
     */
    explicit Timer(int16_t interval);

    /**
     * Initialize timer with given delegate and
     * default interval (1000 milliseconds).
     * @param   delegate   timer delegate
     */
    explicit Timer(Delegate<void()> delegate);

    /**
     * Initialize timer with given delegate and interval.
     * @param   delegate   timer delegate
     * @param   interval   timer interval in milliseconds
     */
    Timer(Delegate<void()> delegate, int16_t interval);

    /**
     * Stop the timer when it is destroyed (e.g. goes out of scope).
     */
    ~Timer() noexcept;

    /*
     * Delete copy/move construtor and copy/move assignment operator.
     */
    Timer(Timer const&) = delete;
    Timer(Timer&&) = delete;
    Timer& operator=(Timer const&) = delete;
    Timer& operator=(Timer&& other) = delete;

    /**
     * Set timer interval.
     * @param   interval   timer interval in milliseconds
     */
    void set_interval(uint16_t interval) {
        this->interval = interval;
    }

    /**
     * Set timer delegate.
     * @param   delegate   timer delegate
     */
    void set_delegate(Delegate<void()> delegate) {
        this->delegate = delegate;
    }

    /**
     * Get timer delegate.
     * @return   timer delegate
     */
    Delegate<void()> get_delegate() const {
        return delegate;
    }

    /**
     * Get timer interval.
     * @return   timer interval in milliseconds
     */
    uint16_t get_interval() const {
        return interval;
    }

    /**
     * Determine if the timer is currently running.
     * @return   true when the timer is running, otherwise false
     */
    bool is_running() const;

    /**
     * Start the timer. The timer delegate will be executed periodically
     * (requires a running main loop Mainloop::run()).
     * The timer delegate must be set for this method to be effective.
     */
    void start();

    /**
     * Stop the timer.
     */
    void stop();

  private:
    static Timer* p_first;
    Timer* p_next;
    Delegate<void()> delegate;
    uint16_t interval;
    uint16_t timestamp;
    static void handle();
    friend void Mainloop::run();
};


#endif
