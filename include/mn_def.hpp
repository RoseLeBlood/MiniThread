/**
 * @file
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
 * The Mini Thread Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.
 */
#ifndef _MINLIB_37360095_2231_4183_8f0e_f55524c5cda1_H_
#define _MINLIB_37360095_2231_4183_8f0e_f55524c5cda1_H_

#include "mn_config.hpp"

#include <stddef.h>
#include <stdint.h>

#include "mn_copyable.hpp"
#include "mn_void.hpp"

#define MNTHREAD_STATIC_CAST_BYTE(BYTE)			static_cast<unsigned char>(BYTE)

#define offsetof(TYPE, MEMBER) 					__builtin_offsetof (TYPE, MEMBER)

/// Returns the number of bits in the given type
#define BitsInType(t)	(sizeof(t) * CHAR_BIT)

/// Returns the mask of type \p t with the lowest \p n bits set.
#define BitMask(t,n)	(t(~t(0)) >> (BitsInType(t) - (n)))

namespace mn {
    using max_align_t 	= uintptr_t;
    using nullptr_t 	= decltype(nullptr);
    using addrof_null_t = nullptr_t ;

    using size_t 	 	= MN_THREAD_CONFIG_SIZE_TYPE;
    using ptrdiff_t  	= long;
    using uptrdiff_t 	= unsigned long;
    using intmax_t 	 	= signed long;
    using uintmax_t 	= unsigned long;

    const size_t default_alignment = alignof(void*);
	constexpr size_t max_alignment = alignof(max_align_t);

	using ::clock_t;
	using ::time_t;
	using ::tm;

	using ::clock;
	using ::difftime;
	using ::mktime;
	using ::time;
	using ::asctime;
	using ::ctime;
	using ::gmtime;
	using ::localtime;
	using ::strftime;

	class null_type { };

	namespace math {
		constexpr double pi               = 3.14159265358979;
		constexpr double pi_reciprocal    = 0.31830988618379;
		constexpr double pi_squared       = 9.86960440108936;
		constexpr double e                = 2.71828182845905;
		constexpr double e_reciprocal     = 0.36787944117144;
		constexpr double e_squared        = 7.38905609893065;
		constexpr double root2            = 1.41421356237310;
		constexpr double root2_reciprocal = 0.70710678118655;
		constexpr double euler            = 0.57721566490153;
		constexpr double golden_ratio     = 1.61803398874989;
	}

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
