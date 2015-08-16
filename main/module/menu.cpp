/*
 * menu.cpp
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


#include "core/delegate.hpp"
#include "core/progmem.hpp"
#include "menu.hpp"
#include "main.hpp"

using namespace Module;

struct ModuleInfo {
    ModId module;
    const Color* icon;
    
    constexpr ModuleInfo(ModId module, const Color* icon)
        : module(module), icon(icon) {}
};

static const uint8_t HighlightInc = 0x60;
static const uint8_t IconWidth = 5;
static const uint8_t IconHeight = 4;
static const uint8_t IconPixels = IconWidth * IconHeight;
static const uint8_t ColNum = (Canvas::Width - 2) / (IconWidth + 1);
static const uint8_t RowNum = (Canvas::Height - 2) / (IconHeight + 1);
static const uint8_t PageIconNum = ColNum * RowNum;

/*static const Color StdIcon[IconPixels] PROGMEM = {
    0x000000, 0x885000, 0x885000, 0x885000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x885000, 0x000000,
    0x000000, 0x000000, 0x000000, 0x000000, 0x000000,
    0x000000, 0x000000, 0x885000, 0x000000, 0x000000
};*/

static const Color AniTestIcon[IconPixels] PROGMEM = {
    0x000000, 0x280b00, 0x481400, 0x521700, 0x000000,
    0x481400, 0x4a1500, 0x521700, 0x2e0d00, 0x481400,
    0x280b00, 0x2e0d00, 0x280b00, 0x521700, 0x280b00,
    0x000000, 0x481400, 0x581900, 0x280b00, 0x000000
};

static const Color TestInputIcon[IconPixels] PROGMEM = {
    0x900000, 0x900000, 0x900000, 0x301000, 0x301000,
    0x900000, 0x900000, 0x900000, 0x301000, 0x301000,
    0x900000, 0x900000, 0x900000, 0x301000, 0x301000,
    0x900000, 0x900000, 0x900000, 0x301000, 0x301000
};

static const Color TestPixels2Icon[IconPixels] PROGMEM = {
    0x006600, 0x006600, 0x006600, 0x006600, 0x006600,
    0x000000, 0x000000, 0x006600, 0x000000, 0x000000,
    0x000000, 0x000000, 0x006600, 0x000000, 0x000000,
    0x000000, 0x000000, 0x006600, 0x000000, 0x000000
};

static const Color TestTransitionsIcon[IconPixels] PROGMEM = {
    0x000000, 0x800080, 0x000000, 0x800080, 0x000000,
    0x800080, 0x000000, 0x800080, 0x000000, 0x800080,
    0x000000, 0x800080, 0x000000, 0x800080, 0x000000,
    0x800080, 0x000000, 0x800080, 0x000000, 0x800080
};

static const Color TetrisIcon[IconPixels] PROGMEM = {
    0x000000, 0x000000, 0x008880, 0x000000, 0x000088,
    0x000000, 0x885500, 0x008880, 0x008880, 0x000088,
    0x885500, 0x885500, 0x008880, 0x880000, 0x000088,
    0x885500, 0x880000, 0x880000, 0x880000, 0x000088
};

static const ModuleInfo modules[] PROGMEM = {
    ModuleInfo(ModId::AniTest, AniTestIcon),
    ModuleInfo(ModId::TestInput, TestInputIcon),
    ModuleInfo(ModId::TestPixels2, TestPixels2Icon),
    ModuleInfo(ModId::TestTransitions, TestTransitionsIcon),
    ModuleInfo(ModId::Tetris, TetrisIcon),
};


template <typename T, uint8_t N>
constexpr uint8_t size_of_array(const T(&)[N]) {
    return N;
}


static inline void calc_pos(uint8_t& x, uint8_t& y, const uint8_t page_index) {
    uint8_t row = page_index / ColNum;
    uint8_t col = page_index % ColNum;
    x = 1 + col * (IconWidth + 1);
    y = 1 + row * (IconHeight + 1);
}


static inline void inc(uint8_t& value, uint8_t amount) {
    if (value >= 0xFF - amount) value = 0xFF;
    else value += amount;
}


Menu::Menu() : curr_index(0) {
    paint_page(false);
    transition.set_source(canvas1);
    transition.set_destination(canvas2);
    transition.fade(800, DELEGATE(this, fade_in_finished));
}


void Menu::paint_icon(uint8_t index, bool highlight) {
    const Color* progmem_icon = Progmem::read(modules, index).icon;
    uint8_t page = index / PageIconNum;
    uint8_t offset = page * PageIconNum;
    uint8_t x, y;
    calc_pos(x, y, index - offset);
    
    for (uint8_t j = 0; j < IconHeight; j++) {
        for (uint8_t i = 0; i < IconWidth; i++) {
            Color pixel = Progmem::read(progmem_icon, j * IconWidth + i);
            if (highlight && (pixel.red || pixel.green || pixel.blue)) {
                inc(pixel.red, HighlightInc);
                inc(pixel.green, HighlightInc);
                inc(pixel.blue, HighlightInc);
            }
            canvas1.set_pixel(x + i, y + j, pixel);
        }
    }
}


void Menu::paint_page(bool highlight) {
    canvas1.fill(0x000000);
    uint8_t page = curr_index / PageIconNum;
    uint8_t offset = page * PageIconNum;
    for (uint8_t i = offset; i < offset + PageIconNum; i++) {
        if (i >= size_of_array(modules)) break;
        paint_icon(i, highlight && curr_index == i);
    }
}


void Menu::input(Input i) {
    switch (i.data) {
        case Input::Left:
            if (curr_index > 0) curr_index--;
            break;
        case Input::Right:
            if (curr_index < size_of_array(modules) - 1)
                curr_index++;
            break;
        case Input::Up:
            if (curr_index >= ColNum)
                curr_index -= ColNum;
            break;
        case Input::Down:
            if (curr_index < size_of_array(modules) - ColNum)
                curr_index += ColNum;
            else curr_index = size_of_array(modules) - 1;
            break;
        case Input::Ok:
            run_current();
            return;
        default: break;
    }
    
    move();
}


void Menu::fade_in_finished() {
    Input::set_handler(DELEGATE(this, input));
    Input::clear_events();
    pulse0();
}


void Menu::pulse0() {
    paint_page(true);
    transition.fade(500, DELEGATE(this, pulse1));
}


void Menu::pulse1() {
    paint_page(false);
    transition.fade(700, DELEGATE(this, pulse0));
}


void Menu::move() {
    paint_page(true);
    transition.fade(200, DELEGATE(this, pulse1));
}


void Menu::run_current() {
    Input::clear_handler();
    
    canvas1.fill(0x000000);
    paint_icon(curr_index, false);
    transition.fade(800, DELEGATE(this, run_current0));
}


void Menu::run_current0() {
    transition.set_source(0x000000);
    transition.fade(800, DELEGATE(this, run_current1));
}


void Menu::run_current1() {
    auto m_info = Progmem::read(modules, curr_index);
    invoke(m_info.module);
}
