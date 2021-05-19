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
#ifndef _MINLIB_BYTE_H_
#define _MINLIB_BYTE_H_

#include "mn_functional.hpp"

namespace mn {
	using mn::byte;

	template<typename IntegerType, class = typename mn::enable_if<mn::is_integral<IntegerType>::value>::type>
    inline IntegerType to_integer(byte b) {
        return IntegerType(b);
    }

	template < class IntegerType, class = typename mn::enable_if<mn::is_integral<IntegerType>::value>::type>
	inline constexpr byte to_byte( IntegerType v ) noexcept {
    	return static_cast<byte>( v );
	}

	inline constexpr unsigned char to_uchar( byte b ) noexcept {
    	return to_integer<unsigned char>( b );
	}

	inline constexpr unsigned char to_uchar( int i ) noexcept {
    	return static_cast<unsigned char>( i );
	}

}

#endif // _MINLIB_BYTE_H_
