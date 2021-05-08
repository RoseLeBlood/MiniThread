/**
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
 *
 * The Mini Thread Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3, or (at your option) any later version.

 * The Mini Thread Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.
*/
#ifndef MINLIB_STL_NLZ_UTILS_H_
#define MINLIB_STL_NLZ_UTILS_H_

#include <limits.h>

namespace mn {
	inline size_t nlz_base(uint64_t x) {
		 unsigned long long value = x;
		 return sizeof(value) * CHAR_BIT - __builtin_clzll(value);
	}
	inline size_t nlz(uint64_t x) {
		return nlz_base(x) - 1;
	}
}

#endif
