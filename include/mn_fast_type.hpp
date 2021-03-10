/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_FAST_TYPE_H_
#define MINLIB_FAST_TYPE_H_

#include "mn_config.hpp"
#include <stdint.h>
#include "mn_def.hpp"

#include "mn_uint128.hpp"
#include "mn_algorithm.hpp"

namespace mn {
    /**
     * This class is a fast bit for all fast_types
     */ 
    class basic_fbit {
    public:
        /**
         * The fast bit
         */ 
        union {
            unsigned char bit : 1;
        };
        /**
         * Ctor - set the bit to 0
         */ 
        basic_fbit() : bit(0) { }
        /**
         * Ctor - set the bit
         * @param b The state for this bit
         */ 
        basic_fbit(bool b) { bit = b ? 1 : 0; }
        /**
         * copyble ctor
         */ 
        basic_fbit(const basic_fbit& b) { bit = b.bit; }
 
        basic_fbit& operator = (const basic_fbit& other) {
            bit = other.bit;
        }
        bool operator == (const basic_fbit& other) {
            return bit == other.bit;
        }
        bool operator != (const basic_fbit& other) {
            return bit != other.bit;
        }

        operator bool() { return bit == 1; }
        /**
         * Flip the bit from 0 to 1 or from 0 to 1
         */ 
        void flip() { bit = (bit ==1) ? 0 : 1; }
    };

    #include <stddef.h>

    /**
     * Template class for all fast types, 
     * a fast type can manupaleted bittable
     */ 
    template <size_t Bits, typename TBaseType>
    class fast_type {
    public:
        using self_type = fast_type<Bits, TBaseType>;
        using value_type = TBaseType;
        using bit_type = fbit;
        
        /**
         * The new type 
         */ 
        union {
            /** The real value **/
            value_type Value;
            /** the value as bits */
            bit_type bits[Bits];
        };
        /**
         * Ctor 
         * @param value_type the value for this type
         * @code
         * fast_type<int> a(3);
         * @endcode 
         */ 
        explicit fast_type(value_type v) : Value(v) {}
        fast_type(const fast_type& c) : Value(c.Value) { }
        
        /**
         * Get the count of bits to set to 1
         * @return The count of bits to set to 1
         */ 
        unsigned int count() { 
            unsigned int i;
            for(int j = 0; j < Bits; j++)
                i += bits[i].bit;
            return i;
        }
        /**
         * Get the count of bits to set to 0
         * @return The count of bits to set to 0
         */ 
        unsigned int zeros() { 
            return Bits - count(); 
        }
        /**
         * Set the a bit on given position
         * @param [in] pos The position to set
         * @param [in] b The bit value to set
         */ 
        unsigned int set(size_t pos, bool p) {
            bits[pos].bit = p ? 1 : 0;
        } 
        /**
         * Get the numbers of bits
         * @return The numbers of bits
         */ 
        size_t size() { return Bits; }
        
        /**
         * Operator to get the bit from given positon
         * @return The bit from given positon
         */ 
        value_type& operator [] (const size_t p) {
            return bits[p];
        }
        /**
         * Set this fast type value to a new value
         * @return this
         */ 
        self_type& operator = (value_type& v) {
            Value = v; return *this;
        }

        self_type& operator = (self_type& other) {
            Value = other.Value; return *this;
        }
        /**
         * Is the other value with this equel
         * @return True when equel and false when not
         */ 
        bool operator == (self_type& other) {
            return Value == other.Value;
        }
        /**
         * Is the other value with this not equel
         * @return True when not equel and false when not
         */ 
        bool operator != (self_type& other) {
            return Value != other.Value;
        }
        bool operator <= (self_type& other) {
            return Value <= other.Value;
        }
        bool operator >= (self_type& other) {
            return Value >= other.Value;
        }
        bool operator < (self_type& other) {
            return Value < other.Value;
        }
        bool operator > (self_type& other) {
            return Value > other.Value;
        }
        
        self_type& operator += (self_type& other) {
            Value += other.Value; return *this;
        }
        self_type& operator -= (self_type& other) {
            Value -= other.Value; return *this;
        }
        self_type& operator *= (self_type& other) {
            Value *= other.Value; return *this;
        }
        self_type& operator &= (self_type& other) {
            Value &= other.Value; return *this;
        }
        self_type& operator |= (self_type& other) {
            Value |= other.Value; return *this;
        }
        self_type& operator ^= (self_type& other) {
            Value ^= other.Value; return *this;
        }
        self_type& operator <<= (self_type& other) {
            Value <<= other.Value; return *this;
        }
        self_type& operator >>= (self_type& other) {
            Value >>= other.Value; return *this;
        }
        self_type& operator /= (self_type& other) {
            Value /= other.Value; return *this;
        }
        self_type& operator += (value_type& v) {
            Value += v; return *this;
        }
        self_type& operator -= (value_type& v) {
            Value -= v; return *this;
        }
        self_type& operator *= (value_type& v) {
            Value *= v; return *this;
        }
        self_type& operator /= (value_type& v) {
            Value /= v; return *this;
        }
        self_type& operator <<= (value_type& v) {
            Value <<= v; return *this;
        }
        self_type& operator >>= (value_type& v) {
            Value >>= v; return *this;
        }
        self_type& operator - ( const self_type& rhs) {
            return self_type(*this) -= rhs;
        }
        self_type& operator + ( const self_type& rhs) {
            return self_type(*this) += rhs;
        }
        self_type& operator * ( const self_type& rhs) {
            return self_type(*this) *= rhs;
        }
        self_type& operator / ( const self_type& rhs) {
            return self_type(*this) /= rhs;
        }
        self_type& operator << ( const self_type& rhs) {
            return self_type(*this) <<= rhs;
        }
        self_type& operator >> ( const self_type& rhs) {
            return self_type(*this) >>= rhs;
        }
        self_type& operator | ( const self_type& rhs) {
            self_type result = self_type(*this);
            result.Value |= rhs.Value;
            return result;
        }
        self_type& operator ^ ( const self_type& rhs) {
            self_type result = self_type(*this);
            result.Value ^= rhs.Value;
            return result;
        }
        self_type& operator & ( const self_type& rhs) {
            self_type result = self_type(*this);
            result.Value &= rhs.Value;
            return result;
        }
        self_type& operator ~ () {
            self_type result = self_type(*this);
            result.Value = ~result.Value;
            return result;
        }
        
        self_type& operator ++ () {
            Value++; return *this;
        }
        self_type& operator -- () {
            Value--; return *this;
        }
        self_type& operator ~ () {
            Value = ~Value; return *this;
        }
    };

    

    

    using fsize_t           = fast_type<value2size<size_t>::size,           size_t>;
    using ffloat_t          = fast_type<value2size<float>::size,            float>;
    using fdouble_t         = fast_type<value2size<double>::size,           double>;

    using fint8_t           = fast_type<value2size<int8_t>::size,           int8_t>;
    using fint16_t          = fast_type<value2size<int16_t>::size,          int16_t>;
    using fint32_t          = fast_type<value2size<int32_t>::size,          int32_t>;
    using fint64_t          = fast_type<value2size<int64_t>::size,          int64_t>;

    using fuint8_t          = fast_type<value2size<uint8_t>::size,          uint8_t>;
    using fuint16_t         = fast_type<value2size<uint16_t>::size,         uint16_t>;
    using fuint32_t         = fast_type<value2size<uint32_t>::size,         uint32_t>;
    using fuint64_t         = fast_type<value2size<uint64_t>::size,         uint64_t>;
    using fuint128_t        = fast_type<value2size<uint128_t>::size,        uint128_t>; 

    using fint_least8_t     = fast_type<value2size<int_least8_t>::size,     int_least8_t>;
    using fint_least16_t    = fast_type<value2size<int_least16_t>::size,    int_least16_t>;
    using fint_least32_t    = fast_type<value2size<int_least32_t>::size,    int_least32_t>;
    using fint_least64_t    = fast_type<value2size<int_least64_t>::size,    int_least64_t>;

    using fuint_least8_t    = fast_type<value2size<uint_least8_t>::size,    uint_least8_t>;
    using fuint_least16_t   = fast_type<value2size<uint_least16_t>::size,   uint_least16_t>;
    using fuint_least32_t   = fast_type<value2size<uint_least32_t>::size,   uint_least32_t>;
    using fuint_least64_t   = fast_type<value2size<uint_least64_t>::size,   uint_least64_t>;
    using fuint_least128_t  = fast_type<value2size<uint_least128_t>::size,  uint_least128_t>; 

}

#endif