/*
 * test_pixels.c
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


#include "test_pixels2.hpp"
#include "main.hpp"

using namespace Module;


TestPixels2::TestPixels2()
    : timer(DELEGATE(this, step0), 2000), count(Canvas::Pixels-1) {
    Input::set_handler(DELEGATE(this, input_handler));
    timer.start();
}


void TestPixels2::step0() {
    canvas.set_pixel(7, 5, Color(255, 255, 255));
    canvas.set_pixel(8, 6, Color(255, 255, 255));
    canvas.set_pixel(9, 5, Color(255, 255, 255));
    
    canvas.set_pixel(7, 6, Color(0, 0, 0));
    canvas.set_pixel(8, 5, Color(0, 0, 0));
    canvas.set_pixel(9, 6, Color(0, 0, 0));
    
    canvas.render();
    timer.set_delegate(DELEGATE(this, step1));
}


void TestPixels2::step1() {   
    canvas.set_pixel(7, 6, Color(255, 0, 0));
    canvas.set_pixel(8, 5, Color(255, 0, 0));
    canvas.set_pixel(9, 6, Color(255, 0, 0));
    
    canvas.render();
    timer.set_delegate(DELEGATE(this, step2));
}


void TestPixels2::step2() {
    canvas.set_pixel(7, 5, Color(0, 0, 255));
    canvas.set_pixel(8, 6, Color(0, 0, 255));
    canvas.set_pixel(9, 5, Color(0, 0, 255));
    
    canvas.render();
    timer.set_delegate(DELEGATE(this, step3));
}


void TestPixels2::step3() {   
    canvas.set_pixel(7, 6, Color(0, 0, 0));
    canvas.set_pixel(8, 5, Color(0, 0, 0));
    canvas.set_pixel(9, 6, Color(0, 0, 0));
    
    canvas.render();
    timer.set_delegate(DELEGATE(this, step4));
    timer.set_interval(400);
}


void TestPixels2::step4() {
    canvas.set_pixel(7, 5, Color(0, 0, 255));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step5));
}


void TestPixels2::step5() {
    canvas.set_pixel(7, 6, Color(55, 0, 200));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step6));
} 


void TestPixels2::step6() {
    canvas.set_pixel(8, 5, Color(128, 0, 128));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step7));
}


void TestPixels2::step7() {
    canvas.set_pixel(8, 6, Color(90, 0, 38));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step8));
}


void TestPixels2::step8() {
    canvas.set_pixel(9, 5, Color(255, 0, 0));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step9));
}


void TestPixels2::step9() {
    canvas.set_pixel(9, 6, Color(220, 35, 0));
    canvas.render();
    timer.set_delegate(DELEGATE(this, step0));
    timer.set_interval(2000);
}


void TestPixels2::input_handler(Input input) {
    if (input.data == Input::Exit) exit();
}
