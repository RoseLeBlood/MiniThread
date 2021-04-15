/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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

#include <cstdio>
#include "excp/mn_lock_exptions.hpp"

#if MN_THREAD_CONFIG_USE_EXCEPTIONS ==  MN_THREAD_CONFIG_YES
namespace mn {
	namespace error {
		//-----------------------------------
		//  Constructor
		//-----------------------------------
		lockcreate_exception::lockcreate_exception(int code)
			: m_iCode(code), m_iLine(0), m_strFile("nofile")  { }

		//-----------------------------------
		//  Constructor - debug
		//-----------------------------------
		lockcreate_exception::lockcreate_exception(int code, int line, const char* file)
				: m_iCode(code), m_iLine(line), m_strFile(file) { }

		//-----------------------------------
		//  to_string
		//-----------------------------------
		const char* lockcreate_exception::what() {
			char *_g_buffer = new char[64];

			snprintf(_g_buffer, 64, "[%d @ %s] Error on creating the lock object: %d", m_iLine, m_strFile, m_iCode);
			return _g_buffer;
		}
	}
}

#endif // MN_THREAD_CONFIG_USE_EXCEPTIONS ==  MN_THREAD_CONFIG_YES
