/*
 * tetris.hpp
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


#ifndef _TETRIS_HPP_
#define _TETRIS_HPP_

#include "stdint.h"
#include "core/canvas.hpp"
#include "core/input.hpp"
#include "core/timer.hpp"
#include "util/transition.hpp"

namespace Module {
    
    class Tetris {
      public:
        enum class TetType : uint8_t {
            None = 0, I, J, L, O, S, T, Z
        };
        
        enum class Move : uint8_t {
            Left, Right, Down, InstantDown
        };
        
        Tetris();

      private:
        Canvas canvas;
        Transition transition;
        Mask mask;
        Timer falling_timer;
        CanvasMatrix<TetType> field;
        BitArray<Canvas::Width> rows_to_remove;
        bool is_game_over;
        struct {
            TetType type;
            uint8_t rotation;
            int8_t x;
            int8_t y;
        } falling;
        
        void game_over();
        bool detect_collision();
        void detect_rows_to_remove();
        void remove_rows_start();
        void remove_rows_start2();
        void remove_rows_finish();
        void new_tetromino();
        void fall();
        void move(Move m);
        void rotate();
        void input_handler(Input i);
        void render();
    };
    
}

#endif
