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
#ifndef MINLIB_ESP32_BINARY_SEMAPHORE_
#define MINLIB_ESP32_BINARY_SEMAPHORE_

#include "mn_basic_semaphore.hpp"

namespace mn {
  /**
   * \ingroup lock
   */ 
  class basic_binary_semaphore : public basic_semaphore {
  public:
    /**
     * Create the binary semaphore
     * 
     * @note When enabled the config item MN_THREAD_CONFIG_USE_EXCEPTIONS then throw on error
     * the lockcreate_exception exceptions and the config item MN_THREAD_CONFIG_DEBUG 
     * enabled, then with debug informations.
     * When the config item MN_THREAD_CONFIG_USE_EXCEPTIONS disabled then get the created error code
     * with basic_semaphore::get_error()
     */
    basic_binary_semaphore();
    virtual ~basic_binary_semaphore();
  };

  using binary_semaphore_t = basic_binary_semaphore;
}

#endif