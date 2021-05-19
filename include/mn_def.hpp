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

#include "mn_config.hpp"

#include <stddef.h>

#include "mn_copyable.hpp"

#define MNTHREAD_STATIC_CAST_BYTE(BYTE)			static_cast<unsigned char>(BYTE)

#define offsetof(TYPE, MEMBER) 					__builtin_offsetof (TYPE, MEMBER)

namespace mn {

    using max_align_t = long double;
    using nullptr_t = decltype(nullptr);
    using addrof_null_t = nullptr_t ;

    using size_t = MN_THREAD_CONFIG_SIZE_TYPE;
    using ptrdiff_t = long;
    using uptrdiff_t = unsigned long;
    using intmax_t = int;
    using uintmax_t = unsigned int;

	constexpr size_t max_alignment = alignof(max_align_t);

    enum class byte : unsigned char { };

   // __CHAR_MAX__

    template<typename IntegerType>
    inline byte& operator <<= (byte& b, IntegerType shift) noexcept {
        return b = byte(MNTHREAD_STATIC_CAST_BYTE(b) << shift);
    }

    template<typename IntegerType>
    inline byte& operator >>= (byte& b, IntegerType shift) noexcept {
        return b = byte(MNTHREAD_STATIC_CAST_BYTE(b) >> shift);
    }

    template<typename IntegerType>
    inline constexpr byte operator << (byte& b, IntegerType shift) noexcept {
        return b = byte(MNTHREAD_STATIC_CAST_BYTE(b) << shift);
    }

    template<typename IntegerType>
    inline constexpr byte operator >> (byte& b, IntegerType shift) noexcept {
        return b = byte(MNTHREAD_STATIC_CAST_BYTE(b) >> shift);
    }

    inline constexpr bool operator==( const byte l, const byte r ) noexcept {
		return l == r;
	}

	inline constexpr bool operator!=( const byte l, const byte r ) noexcept {
		return !( l == r );
	}

	inline constexpr bool operator< ( const byte l, const byte r ) noexcept {
		return l < r;
	}

	inline constexpr bool operator<=( const byte l, const byte r ) noexcept {
		return !( r < l );
	}

	inline constexpr bool operator> ( const byte l, const byte r ) noexcept {
		return ( r < l );
	}

	inline constexpr bool operator>=( const byte l, const byte r ) noexcept {
		return !( l < r );
	}

    inline byte operator |= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(MNTHREAD_STATIC_CAST_BYTE(lhr) | MNTHREAD_STATIC_CAST_BYTE(rhr));
        return lhr;
    }

    inline byte operator &= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(MNTHREAD_STATIC_CAST_BYTE(lhr) & MNTHREAD_STATIC_CAST_BYTE(rhr));
        return lhr;
    }
    inline byte operator ^= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(MNTHREAD_STATIC_CAST_BYTE(lhr) ^ MNTHREAD_STATIC_CAST_BYTE(rhr));
        return lhr;
    }

    inline constexpr byte operator | (const byte& lhr, const byte& rhr) noexcept {
        return byte(MNTHREAD_STATIC_CAST_BYTE(lhr) | MNTHREAD_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator ^ (const byte& lhr, const byte& rhr) noexcept {
        return byte(MNTHREAD_STATIC_CAST_BYTE(lhr) ^ MNTHREAD_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator & (const byte& lhr, const byte& rhr) noexcept {
        return byte(MNTHREAD_STATIC_CAST_BYTE(lhr) & MNTHREAD_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator ~ (byte b) noexcept {
        return byte(~MNTHREAD_STATIC_CAST_BYTE(b));
    }
}

#endif
