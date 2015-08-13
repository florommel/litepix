/*
 * main.hpp
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


#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <stdint.h>

/**
 * Contains Litepix modules.
 */
namespace Module {
    
    /**
     * Identifies a Litepix module.
     */
    enum class ModId : uint8_t {
        Menu,
        AniTest,
        TestInput,
        TestPixels,
        TestPixels2,
        TestTransitions,
        Tetris
    };
    
    /**
     * Exit current mainloop (module) as fast as possible (usually after current
     * event handler (timer or input) returns) and invoke a new module.
     * @param   m   the module to be invoked
     */
    void invoke(ModId m);
    
    /**
     * Exit current mainloop (module) as fast as possible (usually after current
     * event handler (timer or input) returns).
     * Calls Mainloop:exit().
     */
    void exit();
}

#endif
