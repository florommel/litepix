/*
 * transition.hpp
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
 * canvas transitions
 */

#ifndef _UTIL__TRANSITION_HPP_
#define _UTIL__TRANSITION_HPP_

#include <stdint.h>
#include "core/delegate.hpp"
#include "core/timer.hpp"
#include "core/canvas.hpp"


/**
 * Describes a 2-dimensional direction.
 */
enum class Direction : uint8_t {
    LeftRight = 0,
    RightLeft = 1,
    TopBottom = 2,
    BottomTop = 3
};


/**
 * Provides methods to run non blocking transitions.
 * The methods fade(), roll() and dissolve() will return immediately -
 * transition steps are executed as timed events (see Timer) by the main
 * event loop (see Mainloop). The instance holds transition state
 * variables between steps and must therefore exist at least until the end of
 * the transition.
 *
 * Before starting a transition there has to be set a destination canvas
 * (see set_destination()) and a source color or canvas (see set_source()).
 * The destination canvas holds the original image which will be altered step
 * by step (when a transition is started) until it contains the source image or
 * color. The source canvas is never modified.
 */
class Transition {
  public:

    /**
     * Start fade transition.
     * @param   duration         the transition's duration in milliseconds.
     * @param   finished_action  action to be executed when the transition
     *                           has finished.
     */
    void fade(uint16_t duration,
              Delegate<void()> finished_action = Delegate<void()>());

    /**
     * Start roll transition.
     * @param   direction        the transition's direction
     * @param   duration         the transition's duration in milliseconds.
     * @param   finished_action  action to be executed when the transition
     *                           has finished.
     */
    void roll(Direction direction, uint16_t duration,
              Delegate<void()> finished_action = Delegate<void()>());

    /**
     * Start dissolve transition.
     * @param   duration         the transition's duration in milliseconds.
     * @param   finished_action  action to be executed when the transition
     *                           has finished.
     */
    void dissolve(uint16_t duration,
                  Delegate<void()> finished_action = Delegate<void()>());

    /**
     * Set the destination canvas. The destination canvas holds the
     * original image which will be altered step by step when a transition
     * is started until it contains the source image or color.
     * @param   canvas   destination canvas (as reference - the instance
     *                   must exist until the end of the transition!)
     */
    void set_destination(Canvas& canvas) {
        dest = &canvas;
    }

    /**
     * Set canvas as source. The source canvas is never modified.
     * @param   canvas   source canvas (as reference - the instance
     *                   must exist until the end of the transition!)
     */
    void set_source(Canvas& canvas) {
        src.canvas = &canvas;
        is_color = false;
    }

    /**
     * Set color as source.
     * @param   color   source color
     */
    void set_source(Color color) {
        src.color_buffer[0] = color.green;
        src.color_buffer[1] = color.red;
        src.color_buffer[2] = color.blue;
        is_color = true;
    }

    /**
     * Set transition mask. The mask defines which pixels will be affected by
     * future transitions.
     * @param   mask   transition mask.
     */
    void set_mask(Mask& mask) {
        this->mask = &mask;
    }

    /**
     * Clear transition mask. All pixels will be affected by future transitions.
     */
    void clear_mask() {
        mask = nullptr;
    }

  private:
    union {
        Canvas* canvas;
        uint8_t color_buffer[3];
    } src = {};
    Canvas* dest = nullptr;
    Mask* mask = nullptr;
    Timer timer;
    Delegate<void()> finished_action;
    uint8_t step_count;
    uint8_t step;
    union {
        Direction direction;
        uint8_t seed;
    } ext;
    bool is_color = true;

    void set_common_parameters(uint16_t duration,
                               Delegate<void()> finished_action);
    void finish_step();
    void fade_step();
    void roll_step();
    void dissolve_step();
};


#endif
