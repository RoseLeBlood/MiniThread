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
#ifndef _MINILIB_06685af7_1230_4e91_a381_14df27622880_H_
#define _MINILIB_06685af7_1230_4e91_a381_14df27622880_H_

#include "mn_config.hpp"

#include <stdint.h>

namespace mn {

	class basic_timestamp {
	public:
		using time_type = int64_t;
		using int_type= uint64_t;
		using self_type = basic_timestamp;

		basic_timestamp();

	protected:

	private:
	};

	using timestamp_t = basic_timestamp;
}

#endif // _MINILIB_06685af7_1230_4e91_a381_14df27622880_H_
