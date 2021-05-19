/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef __MINLIB_MN_ADDRESSOF_H__
#define __MINLIB_MN_ADDRESSOF_H__


#include "../mn_config.hpp"
#include "../mn_def.hpp"

namespace mn {
	template<class T>
	constexpr inline T* addressof(T& o) noexcept {
		return __builtin_addressof(o);
	}
	template<class T> const T* addressof(const T&&) = delete;
}


#endif // __MINLIB_MN_ADDRESSOF_H__
