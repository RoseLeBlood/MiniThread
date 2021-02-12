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
#include <string>

namespace mn {
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
      virtual std::string to_string();
  private:
    int m_iCode;
    int m_iLine;
    const char* m_strFile;
  };
}

#endif //MINLIB_ESP32_LOCK_EXCEPTION_