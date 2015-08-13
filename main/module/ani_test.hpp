/*
 * ani_test.hpp
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


#ifndef _ANI_TEST_HPP_
#define _ANI_TEST_HPP_

#include "stdint.h"
#include "core/timer.hpp"
#include "core/canvas.hpp"
#include "core/input.hpp"

namespace Module {
    
    class AniTest {
      public:
        AniTest();
        
      private:
        static const uint8_t DecValue0 = 2;
        static const uint8_t DecValue1 = 5;
        
        Timer step_timer;
        Canvas canvas;
        CanvasMatrix<uint8_t> int_matrix0;
        CanvasMatrix<uint8_t> int_matrix1;
        Mask up_mask0;
        uint8_t hue;
        uint8_t pos;
        
        void spot();
        void spot2();
        void step();
        void input(Input in);
    };
    
}

#endif
