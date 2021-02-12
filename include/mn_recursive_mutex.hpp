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
#ifndef MINLIB_ESP32_REC_MUTEX_
#define MINLIB_ESP32_REC_MUTEX_

#include "mn_mutex.hpp"

namespace mn { 
#if MN_THREAD_CONFIG_RECURSIVE_MUTEX == MN_THREAD_CONFIG_YES
/**
 *  Recursive usage Mutex.
 *
 *  By default calls to Lock these objects block forever, but this can be
 *  changed by simply passing in a argument to the lock() method.
 *  These objects are recursively acquirable. Calling lock() twice from
 *  the same Thread (i.e. task) works fine. The caller just needs to be sure to
 *  call unlock() as many times as lock().
 *
 *  @note Recursive mutexes use more resources than standard mutexes. You
 *        should be sure that you actually need this type of synchronization
 *        before using it.
 * 
 * @ingroup mutex
 * @ingroup lock
 */
class recursive_mutex : public basic_mutex {
public:
  /**
   * Create the recursive mutex
   * 
   * @note When enabled the config item MN_THREAD_CONFIG_USE_LOCK_CREATE then throw on error
   * the lockcreate_exception exceptions and the config item MN_THREAD_CONFIG_DEBUG 
   * enabled, then with debug informations.
   * When the config item MN_THREAD_CONFIG_USE_LOCK_CREATE disabled then get the error code
   * with basic_semaphore::get_error()
   */
  recursive_mutex();
  
  /**
   *  Lock the Mutex.
   *
   *  @param Timeout How long to wait to get the Lock until giving up. (default = 0xffffffffUL)
   *  @return ERR_MUTEX_OK if the Lock was acquired, ERR_MUTEX_LOCK if it timed out.
   *  or ERR_MUTEX_NOTINIT when mutex not created
   * 
   *  @note use 'xSemaphoreTakeRecursiveFromISR' in ISR context or 'xSemaphoreTakeRecursive' in all other
   */
	virtual int lock(unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT);

  /**
   *  Unlock the Mutex.
   *
   *  @return ERR_MUTEX_OK if the Lock was released, ERR_MUTEX_UNLOCK if it failed. 
   *  or ERR_MUTEX_NOTINIT when mutex not created
   * 
   *  @note use 'xSemaphoreGiveRecursiveFromISR' in ISR context or 'xSemaphoreGiveRecursive' in all other
   */
	virtual int unlock();
};

using remutex_t = recursive_mutex;

#endif // MN_THREAD_CONFIG_RECURSIVE_MUTEX MN_THREAD_CONFIG_YES

}

#endif // MINLIB_ESP32_REC_MUTEX_
