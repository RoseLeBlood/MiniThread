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
#ifndef _MINLIB_37360095_2231_4183_8f0e_f55524c5cda1_H_
#define _MINLIB_37360095_2231_4183_8f0e_f55524c5cda1_H_

#include <stddef.h>
#include "mn_config.hpp"

namespace mn {
    using max_align_t = int;
    using nullptr_t = decltype(nullptr);

    using size_t = MN_THREAD_CONFIG_SIZE_TYPE;
    using ptrdiff_t = long;
    using uptrdiff_t = unsigned long;
    using intmax_t = int;
    using uintmax_t = unsigned int;

    enum class byte : unsigned char { };
 
    
    template<typename IntegerType>
    inline byte& operator <<= (byte& b, IntegerType shift) { 
        return b = byte(static_cast<unsigned char>(b) << shift); 
    }

    template<typename IntegerType>
    inline byte& operator << (byte& b, IntegerType shift) { 
        return b = byte(static_cast<unsigned char>(b) << shift); 
    }

    template<typename IntegerType>
    inline byte& operator >>= (byte& b, IntegerType shift) { 
        return b = byte(static_cast<unsigned char>(b) >> shift); 
    }

    template<typename IntegerType>
    inline byte& operator >> (byte& b, IntegerType shift) { 
        return b = byte(static_cast<unsigned char>(b) >> shift); 
    }


   
    inline byte& operator |= (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) | static_cast<unsigned char>(rhr));
    }

    inline byte& operator | (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) | static_cast<unsigned char>(rhr));
    }

    inline byte& operator &= (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) & static_cast<unsigned char>(rhr));
    }

    inline byte& operator & (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) & static_cast<unsigned char>(rhr));
    }

    inline byte& operator ^= (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) ^ static_cast<unsigned char>(rhr));
    }

    inline byte& operator ^ (byte& lhr, byte rhr)  {
        return lhr = byte(static_cast<unsigned char>(lhr) ^ static_cast<unsigned char>(rhr));
    }

    inline byte operator ~ (byte b) { 
        return byte(~static_cast<unsigned char>(b)); 
    }
    
    template<typename IntegerType>
    inline IntegerType to_integer(byte b) { 
        return IntegerType(b); 
    }
}

#endif