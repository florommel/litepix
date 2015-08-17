/*
 * snake.hpp
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


#ifndef _SNAKE_HPP_
#define _SNAKE_HPP_

#include <stdint.h>
#include "core/input.hpp"
#include "core/canvas.hpp"
#include "core/container.hpp"
#include "util/transition.hpp"

namespace Module {
    
    class Snake {
      public:
        enum class Type : uint8_t {
            Empty = 0, PartLeft, PartRight, PartUp, PartDown, Food
        };
        
        Snake();
        
      private:
        Canvas canvas;
        Transition transition;
        Timer move_timer;
        CanvasMatrix<Type> field;
        uint8_t head_x;
        uint8_t head_y;
        uint8_t tail_x;
        uint8_t tail_y;
        Type prev;
        
        void game_over();
        void move();
        void place_food();
        void input(Input i);
        void render();
    };
    
}


#endif
