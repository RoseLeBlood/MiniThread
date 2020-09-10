/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "mn_config.hpp"
#include "mn_version.hpp"

#include <stdio.h>

libmnVersion* libmnVersion::m_pInstance = NULL;

libmnVersion::libmnVersion()
{
	m_major = MN_THREAD_MAJOR_VERSION;
	m_minor = MN_THREAD_MINOR_VERSION;
	m_debug = MN_THREAD_DEBUG_VERSION;
	m_license = std::string("LGPL");

#if MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_MUTEX
	m_extras = std::string("MX");
#elif MN_THREAD_CONFIG_AUTOLOCK == MN_THREAD_CONFIG_SPINLOCK
	m_extras = std::string("SL");
#else
	m_extras = std::string("??");
#endif

}

libmnVersion::~libmnVersion() { }

std::string libmnVersion::to_string() const {
	char str[32];
	snprintf(str, 32, "%d.%d.%d - %s (%s)", m_major, m_minor, m_debug, 
		m_license.c_str(), m_extras.c_str() );
	return std::string(str);
}
