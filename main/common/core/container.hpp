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



/* --- I M P L E M E N T A T I O N --- */



template<typename T, uint8_t Width, uint8_t Height>
class Matrix : public Array<T, Width*Height> {
  public:

    /**
     * Get element (copy) at specified position.
     * @param   x   x-position
     * @param   y   y-position
     * @return  element at position (x, y)
     */
    constexpr T operator()(uint8_t x, uint8_t y) const {
        return (*this)[(uint16_t)y*Width + x];
    }

    /**
     * Get element reference at specified position.
     * @param   x   x-position
     * @param   y   y-position
     * @return  element reference at position (x, y)
     */
    T& operator()(uint8_t x, uint8_t y) {
        return (*this)[(uint16_t)y*Width + x];
    }
};


template<uint8_t Width, uint8_t Height>
class Matrix<bool, Width, Height>
    : public BitArray<Width*Height> {
  public:

    /**
     * Get bit value at specified position.
     * @param   x   x-position
     * @param   y   y-position
     * @return  bit value at position (x, y)
     */
    constexpr bool operator()(uint8_t x, uint8_t y) const {
        return (*this)[(uint16_t)y*Width + x];
    }

    /**
     * Get bit reference at specified position.
     * @param   x   x-position
     * @param   y   y-position
     * @return  bit reference at position (x, y)
     */
    typename BitArray<Width*Height>::BitRef operator()(uint8_t x, uint8_t y) {
        return (*this)[(uint16_t)y*Width + x];
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
    constexpr T operator[](IntT index) const {
        return buffer[index];
    }

    /**
     * Get element reference at index.
     * @param   index   element index
     * @return  element reference at index
     */
    template<typename IntT>
    T& operator[](IntT index) {
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
     * A Type to reference a bit value.
     * An object of this type may be returned by BitArray objects.
     * This class cannot be instantiated directly (except by copy and move
     * construction).
     */
    class BitRef final {
      public:

        BitRef(BitRef const& x) = default;

        BitRef(BitRef&& x) = default;

        BitRef& operator=(bool x) {
            *byte ^= (-x ^ (*byte)) & (1 << n);
            return *this;
        }

        BitRef& operator=(BitRef const& x) {
            *this = (bool)(((*x.byte) >> x.n) & 1);
            return *this;
        }

        BitRef& operator=(BitRef&& x) noexcept {
            *this = (bool)(((*x.byte) >> x.n) & 1);
            return *this;
        }

        operator bool() const {
            return ((*byte) >> n) & 1;
        }

      private:
        uint8_t* byte;
        uint8_t n;

        friend class Array<bool, Size>;
        constexpr BitRef(uint8_t* byte, uint8_t n) : byte(byte), n(n) {}
    };

    /**
     * Get bit value at index.
     * @param   index   bit index
     * @return  bit value at index
     */
    template<typename IntT>
    constexpr bool operator[](IntT index) const {
        return buffer[index >> 3] & (1 << (index & 0x07));
    }

    /**
     * Get bit reference at index.
     * @param   index   bit index
     * @return  bit reference at index
     */
    template<typename IntT>
    BitRef operator[](IntT index) {
        return BitRef(&(buffer[index >> 3]), index & 0x07);
    }

    /**
     * Set bit value at index (set to true).
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_set(IntT index) {
        buffer[index >> 3] |= (1 << (index & 0x07));
    }

    /**
     * Reset bit value at index (set to false).
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_reset(IntT index) {
        buffer[index >> 3] &= ~(1 << (index & 0x07));
    }

    /**
     * Toggle bit value at index.
     * This is faster than accessing the bit via index operator.
     * @param   index   bit index
     */
    template<typename IntT>
    void bit_toggle(IntT index) {
        buffer[index >> 3] ^= (1 << (index & 0x07));
    }

    /**
     * Assign bits of an integer to array.
     * The least significant bit will be the first array element.
     * @param   value   integer value
     */
    template<typename IntT>
    void assign(IntT value) {
        uint8_t c = (sizeof(IntT) > buffer_size(Size))
                    ? buffer_size(Size) : sizeof(IntT);
        for (uint8_t i = 0; i < c; ++i) {
            this->buffer[i] = (value >> (i*8)) & 0xFF;
        }
    }

  private:
    template<typename IntT>
    static constexpr IntT buffer_size(IntT size) {
        return ((size & 0x07) == 0) ? (size >> 3) : ((size >> 3) + 1);
    }

    uint8_t buffer[buffer_size(Size)] = {};
};


#endif
