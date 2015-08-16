/*
 * main.cpp
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


#include "main.hpp"
#include "core/mainloop.hpp"
#include "core/canvas.hpp"
#include "menu.hpp"
#include "ani_test.hpp"
#include "test_input.hpp"
#include "test_pixels.hpp"
#include "test_pixels2.hpp"
#include "test_transitions.hpp"
#include "tetris.hpp"
#include "startup.hpp"


#define CASE_RUN_MODULE(module_name) \
    case ModId::module_name: {\
        next = StdModule;\
        module_name ctx;\
        Mainloop::run();\
        break;\
    }\


using namespace Module;


static const ModId StdModule = ModId::Menu;
static const ModId StartUpModule = ModId::Menu;

static ModId next = StartUpModule;


static inline void start() {
    StartUp ctx;
    Mainloop::run();
}


#if defined ARCH_AVR
__attribute__((OS_main))
#endif
int main(void) {
    start();
    
    while (true) {
        Input::clear_handler();
        Input::clear_events();
        
        switch (next) {
            CASE_RUN_MODULE(Menu);
            CASE_RUN_MODULE(AniTest);
            CASE_RUN_MODULE(TestInput);
            CASE_RUN_MODULE(TestPixels);
            CASE_RUN_MODULE(TestPixels2);
            CASE_RUN_MODULE(TestTransitions);
            CASE_RUN_MODULE(Tetris);
            default: break;
        }
    }
}


void Module::invoke(ModId m) {
    next = m;
    Mainloop::exit();
}


void Module::exit() {
    Mainloop::exit();
}
