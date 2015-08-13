/*
 * test_pixels2.hpp
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


#ifndef _TEST_PIXELS2_HPP_
#define _TEST_PIXELS2_HPP_

#include "stdint.h"
#include "core/timer.hpp"
#include "core/canvas.hpp"
#include "core/input.hpp"

namespace Module {
    
    class TestPixels2 {
      public:
        TestPixels2();

      private:
        Canvas canvas;
        Timer timer;
        uint8_t count;
        
        void step0();
        void step1();
        void step2();
        void step3();
        void step4();
        void step5();
        void step6();
        void step7();
        void step8();
        void step9();
        
        void input_handler(Input input);
    };
    
}

#endif
