/*
 * canvas.hpp
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
 * LED matrix output
 */

#ifndef _CORE__CANVAS_HPP_
#define _CORE__CANVAS_HPP_

#include <stdint.h>
#include "container.hpp"

//Define to mirror image on vertical line
//#define REVERSE_VERTICAL

//Define to mirror image on horizontal line
//#define REVERSE_HORIZONTAL
/**
 * Describes a color.
 */
struct Color {
    /** color rgb values */
    uint8_t red, green, blue;
    
    /**
     * Initialize new color (black).
     */
    constexpr Color()
        : red(0), green(0), blue(0) {}
    
    /**
     * Initialize new color with given values.
     * @param   red     red value
     * @param   green   green value
     * @param   blue    blue value
     */
    constexpr Color(uint8_t red, uint8_t green, uint8_t blue)
        : red(red), green(green), blue(blue) {}
    
    /**
     * Initialize new color from rgb integer value.
     * @param   rgb   rgb value as integer
     *                least significant byte: blue value
     *                second least significant byte: green value
     *                third least significant byte: red value
     */
    constexpr Color(uint32_t rgb)
        : red((uint8_t)(rgb >> 16)), green((uint8_t)(rgb >> 8)),
          blue((uint8_t)rgb) {};
    
    /**
     * Create new color from HSV.
     * @param   h   hue
     * @param   s   saturation
     * @param   v   value
     */
    static Color from_hsv(uint8_t h, uint8_t s, uint8_t v);
};


/**
 * A canvas that can be rendered to the led matrix.
 * It provides high level methods to get and set pixel values, but also allows
 * to access the underlying buffer.
 */
class Canvas {
  public:
    
    /** Width in pixels */
    static const uint8_t Width = 20;
    
    /** Height in pixels */
    static const uint8_t Height = 12;
    
    /** Total pixel count */
    static const uint8_t Pixels = Width * Height;
    
    /** Total byte count */
    static const uint16_t Bytes = Pixels * 3;
    
    /**
     * Initialize a new canvas object filled with black pixels.
     */
    Canvas();
    
    /**
     * Render the canvas object to the pixel matrix.
     */
    void render();
    
    /**
     * Fill canvas with given color.
     * @param   color   color which is used to fill canvas
     */
    void fill(Color color);
    
    /**
     * Fill canvas with randomly colored pixels.
     */
    void fill_random();
    
    /**
     * Fill rect with given color.
     * The parameters width and height can be negative.
     * Tge rect's dimensions can exceed the canvas' dimensions.
     * @param   x        x-position
     * @param   y        y-position
     * @param   width    rect width
     * @param   height   rect height
     * @param   color    color which is used to fill rect
     */
    void fill_rect(int8_t x, int8_t y, int8_t width, int8_t height,
                   Color color);
    
    /**
     * Get pixel at specified index.
     * @param   index   pixel index (= y-pos * width + x-pos)
     * @return  the pixel's color
     */
    Color get_pixel(uint16_t index) const;
    
    /**
     * Get pixel at specified position
     * @param   x   x-position
     * @param   y   y-position
     * @return  the pixel's color
     */
    Color get_pixel(uint8_t x, uint8_t y) const;
    
    /**
     * Set pixel at specified index.
     * @param   index   pixel index (= y-pos * width + x-pos)
     * @param   color   color to set
     */
    void set_pixel(uint16_t index, Color color);
    
    /**
     * Get pixel at specified position
     * @param   x       x-position
     * @param   y       y-position
     * @param   color   color to set
     */
    void set_pixel(uint8_t x, uint8_t y, Color color);
    
    /**
     * Get the canvas' underlying buffer.
     * This is a byte array containing the sequential raw pixel data.
     *
     *     The following shows the shape of the buffer's data
     *       ----> width
     *       pix0  pix1  pix2  pix3  ...
     *     | G R B G R B G R B G R B G R B G R B G R B G R B
     *     | G R B G R B G R B G R B G R B G R B G R B G R B
     *     v G R B G R B G R B G R B G R B G R B G R B G R B
     *     height
     */
    uint8_t* get_buffer() {
        return buffer;
    }
    
  private:
    uint8_t buffer[Bytes];
};


/**
 * A bitmap with the canvas' pixel dimensions
 * (Canvas::Width x Canvas::Height).
 */
using Mask = Bitmap<Canvas::Width, Canvas::Height>;


/**
 * A matrix with the canvas' pixel dimensions
 * (Canvas::Width x Canvas::Height).
 */
template<typename T>
using CanvasMatrix = Matrix<T, Canvas::Width, Canvas::Height>;


#endif
