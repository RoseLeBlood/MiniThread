/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#include "mn_config.hpp"

#if MN_THREAD_CONFIG_USE_EXCEPTIONS ==  MN_THREAD_CONFIG_YES

#include "excp/mn_eventgroup_exception.hpp"


namespace mn {
	namespace error {
		eventgroup_exception::eventgroup_exception() { }

		//-----------------------------------
		//  to_string
		//-----------------------------------
		const char* eventgroup_exception::what() {
			return "Error on creating the event group";
		}
	}
}

#endif // MN_THREAD_CONFIG_USE_EXCEPTIONS



