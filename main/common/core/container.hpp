/*
 * container.hpp
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
 * data container types
 */

#ifndef _CORE__CONTAINER_HPP_
#define _CORE__CONTAINER_HPP_

#include <stdint.h>


/**
 * A simple array wrapper.
 */
template<typename T, uint16_t Size>
class Array;

/**
 * A bit (boolean) array optimized for space allocation.
 */
template<uint16_t Size>
using BitArray = Array<bool, Size>;

/**
 * A 2-dimensional matrix.
 */
template<typename T, uint8_t Width, uint8_t Height>
class Matrix;

/**
 * A 2-dimensional bit (boolean) matrix optimized for space allocation.
 */
template<uint8_t Width, uint8_t Height>
using Bitmap = Matrix<bool, Width, Height>;


template<typename T, uint8_t Width, uint8_t Height>
class Matrix : public Array<T, Width*Height> {
  public:
    
    /**
     * Get element (copy) at specified position.
     * @param   x   x-position
     * @param   y   y-position
     * @return  element at position (x, y)
     */
    constexpr T get(const uint8_t x, const uint8_t y) const {
        return (*this)[(uint16_t)y*Width + x];
    }
    
    /**
     * Set element at specified position.
     * @param   x       x-position
     * @param   y       y-position
     * @param   value   element to set
     */
    void set(const uint8_t x, const uint8_t y, const T value) {
        (*this)[(uint16_t)y*Width + x] = value;
    }
};


template<typename T, uint16_t Size>
class Array {
  public:
    
    /**
     * Get element (copy) at index.
     * @param   index   element index
     * @return  element (copy) at index
     */
    template<typename IntT>
    constexpr const T operator[](const IntT index) const {
        return buffer[index];
    }
    
    /**
     * Get element reference at index.
     * @param   index   element index
     * @return  element reference at index
     */
    template<typename IntT>
    T& operator[](const IntT index) {
        return buffer[index];
    }
    
  private:
    T buffer[Size] = {};
};


/**
 * A boolean array optimized for space allocation.
 * Each element occupies only one bit.
 */
template<uint16_t Size>
class Array<bool, Size> {
  public:
    
    /**
     * Type to reference a bit value.
     */
    template<typename IntT>
    class Reference {
      public:
        constexpr Reference(uint8_t* byte, IntT n)
            : byte(byte), n(n) {}
        
        Reference& operator=(const bool x) {
            *byte ^= (-x ^ (*byte)) & (1 << n);
            return *this;
        }
        
        Reference& operator=(const Reference& x) {
            *this = (bool)(((*x.byte) >> x.n) & 1);
            return *this;
        }
        
        operator bool() {
            return ((*byte) >> n) & 1;
        }
      private:
        uint8_t* byte;
        IntT n;
    };
    
    /**
     * Get bit value at index.
     * @param   index   bit index
     * @return  bit value at index
     */
    template<typename IntT>
    constexpr bool operator[](const IntT index) const {
        return buffer[index >> 3] & (1 << (index & 0x07));
    }
    
    /**
     * Get bit reference at index.
     * @param   index   bit index
     * @return  bit reference at index
     */
    template<typename IntT>
    Reference<IntT> operator[](const IntT index) {
        return Reference<IntT>(&(buffer[index >> 3]), index & 0x07);
    }
    
    /**
     * Set bit value at index (set to true).
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_set(const IntT index) {
        buffer[index >> 3] |= (1 << (index & 0x07));
    }
    
    /**
     * Reset bit value at index (set to false).
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_reset(const IntT index) {
        buffer[index >> 3] &= ~(1 << (index & 0x07));
    }
    
    /**
     * Toggle bit value at index.
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_toggle(const IntT index) {
        buffer[index >> 3] ^= (1 << (index & 0x07));
    }
    
    /**
     * Assign bits of an integer to array.
     * The least significant bit will be the first array element.
     * @param   value   integer value
     */
    template<typename IntT>
    void assign(const IntT value) {
        uint8_t c = (sizeof(IntT) > buffer_size(Size))
                    ? buffer_size(Size) : sizeof(IntT);
        for (uint8_t i = 0; i < c; ++i) {
            this->buffer[i] = (value >> (i*8)) & 0xFF;
        }
    }
    
  private:
    template<typename IntT>
    static constexpr IntT buffer_size(const IntT size) {
        return ((size & 0x07) == 0) ? (size >> 3) : ((size >> 3) + 1);
    }
    
    uint8_t buffer[buffer_size(Size)] = {};
};


#endif
