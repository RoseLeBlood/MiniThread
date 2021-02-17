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
#include "mn_config.hpp"
#include "mn_version.hpp"

#include <stdio.h>

namespace mn {


	//-----------------------------------
	//  constructor
	//-----------------------------------
	version::version()
	{
		m_major = MN_THREAD_MAJOR_VERSION;
		m_minor = MN_THREAD_MINOR_VERSION;
		m_debug = MN_THREAD_DEBUG_VERSION;

		m_license = std::string("-LGPL");

	#if MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_MUTEX
		m_extras = std::string("-MX");
	#elif MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_BINARY_SEMAPHORE
		m_extras = std::string("-BS");
	#elif MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_COUNTING_SEMAPHORE
		m_extras = std::string("-CS");
	#endif
	}


	//-----------------------------------
	//  to_string
	//-----------------------------------
	std::string version::to_string() const {
		std::string text = std::string(MN_THREAD_VERSION_STRING);


	#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES
		text += std::string(MN_THREAD_BETA_VERSION_STRING);
	#endif
		
		text += m_license + m_extras;
		return text;
	}
}