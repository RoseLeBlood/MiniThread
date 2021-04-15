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
#ifndef MINLIB_ESP32_LOCK_EXCEPTION_
#define MINLIB_ESP32_LOCK_EXCEPTION_

#include "../mn_config.hpp"

#include <exception>

#if MN_THREAD_CONFIG_USE_EXCEPTIONS ==  MN_THREAD_CONFIG_YES
    #if MN_THREAD_CONFIG_DEBUG  == MN_THREAD_CONFIG_YES
        /**
         * Macro to throw the lockcreate_exception exception, debug version
         */
        #define MN_THROW_LOCK_EXP(CODE) throw mn::error::lockcreate_exception(CODE, __LINE__, __FILE__);
        #define MN_THROW_LOCK_EXP2(CODE, RET) throw mn::error::lockcreate_exception(CODE, __LINE__, __FILE__);
    #else
        /**
         * Macro to throw the lockcreate_exception exception, only the code
         */
        #define MN_THROW_LOCK_EXP(CODE) throw lockcreate_exception(CODE);
        #define MN_THROW_LOCK_EXP2(CODE, RET) throw lockcreate_exception(CODE);
    #endif // MN_THREAD_CONFIG_DEBUG
#else
    /**
     * This Macro util set only the error code
     */
    #define MN_THROW_LOCK_EXP(CODE) set_error(CODE);
    /**
     * This Macro util set only the error code, and return setted return code
     */
    #define MN_THROW_LOCK_EXP2(CODE, RET)  { set_error(CODE); return RET; }
#endif //MN_THREAD_CONFIG_USE_EXCEPTIONS

#if MN_THREAD_CONFIG_USE_EXCEPTIONS ==  MN_THREAD_CONFIG_YES
namespace mn {
	 namespace error {
		/**
		* Lock creating exception
		*
		* @ingroup lock
		*/
		class lockcreate_exception : public std::exception {
		public:
			/**
			* Constructor for normal use
			*
			* @param code The error code on creating the lock object
			*/
			lockcreate_exception(int code);
			/**
			* Constructor for debugging
			*
			* @param code The error code on creating the lock object
			* @param line The line in the source code, where the given error code throwed
			* @param file The file name where the given error code throwed
			*/
			lockcreate_exception(int code, int line, const char* file);

			/**
			* Returns a C-style character string describing the general cause
			* of the current error.
			*
			* @return The C-style character string describing the general cause
			* of the current error.
			*/
			virtual const char* what();
		private:
			int m_iCode; 			/**! error code of the exeption  */
			int m_iLine; 			/**! which line was throw the exeption */
			const char* m_strFile; 	/**! in which line was throw the exeption */
		};
	}
}
#endif // MN_THREAD_CONFIG_USE_EXCEPTIONS

#endif //MINLIB_ESP32_LOCK_EXCEPTION_
