/*
 * test_input.hpp
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


#ifndef _TEST_INPUT_HPP_
#define _TEST_INPUT_HPP_

#include "stdint.h"
#include "core/canvas.hpp"
#include "core/input.hpp"
#include "util/transition.hpp"

namespace Module {
    
    class TestInput {
      public:
        TestInput();
        
      private:
        Canvas canvas;
        uint8_t x = 100;
        Transition transition;
        
        void ihandler(Input input);
        void fout();
    };
    
}

#endif
