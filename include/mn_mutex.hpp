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
#ifndef MINLIB_ESP32_MUTEX_
#define MINLIB_ESP32_MUTEX_

#include "mn_basic_semaphore.hpp"
#include "mn_error.hpp"

namespace mn {
  /**
   *  Base wrapper class around FreeRTOS's implementation of mutexes.
   *  These objects are not recursively acquirable. Calling lock() twice from
   *  the same Thread (i.e. task) will deadlock.
   * 
   * @ingroup mutex
   * @ingroup lock
   */
  class basic_mutex : public basic_semaphore {
  public:
    /**
     * Create the mutex
     * 
     * @note When enabled the config item MN_THREAD_CONFIG_USE_LOCK_CREATE then throw on error
     * the lockcreate_exception exceptions and the config item MN_THREAD_CONFIG_DEBUG 
     * enabled, then with debug informations.
     * When the config item MN_THREAD_CONFIG_USE_LOCK_CREATE disabled then get the created error code
     * with basic_semaphore::get_error()
     */
    basic_mutex();
    /**
     * Destrutor - destroy the mutex
     */ 
    virtual ~basic_mutex();

    /**
     *  Lock the Mutex.
     *
     *  @param Timeout How long to wait to get the Lock until giving up. (default = 0xffffffffUL)
     *  @return ERR_MUTEX_OK if the Lock was acquired, ERR_MUTEX_LOCK if it timed out.
     *  or ERR_MUTEX_NOTINIT when mutex not created
     */
    virtual int lock(unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT);

    /**
     *  Unlock the Mutex.
     *
     *  @return ERR_MUTEX_OK if the Lock was released, ERR_MUTEX_UNLOCK if it failed. 
     *  or ERR_MUTEX_NOTINIT when mutex not created
     */
    virtual int unlock();
  };




  using mutex_t = basic_mutex;
}


#endif
