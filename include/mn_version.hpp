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
#ifndef _MINLIB_VERSION_H_
#define _MINLIB_VERSION_H_

#include <string>
#include "mn_singleton.hpp"

namespace mn {
	/**
	 * Version wrapper for this library
	 * 
	 * @ingroup base
	 */ 
	class version : public basic_singleton<version> {
		friend class basic_singleton<version>;
		version();
	public:
		
		/**
		 * Get the major version
		 * @return The major version
		 */ 
		int get_major_version() const								{ return m_major; }
		/**
		 * Get the minor version
		 * @return The minor version
		 */ 
		int get_minor_version() const								{ return m_minor; }
		/**
		 * Get the debug version
		 * @return The debug version
		 */ 
		int get_debug_version() const								{ return m_debug; }
		/**
		 * Get the license string
		 * @return The license string
		 */ 
		std::string get_license() const								{ return m_license; }

		/**
		 * Get the version string with extra informations
		 * @return The version string with extra informations
		 */ 
		std::string to_string() const;

		version(const version&)       	= delete;
        void operator=(const version&)  = delete; 
	private:
		/**
		 * The license string holder
		 */
		std::string m_license;
		/**
		 * The Version holders
		 */
		int m_major, m_minor, m_debug;
		/**
		 * The extra versions infomations string holder
		 */
		std::string m_extras;
	};

	using version_t = version;
}
#endif
