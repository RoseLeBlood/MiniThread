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

#include "mn_config.hpp"
#include "mn_semaphore.hpp"
#include "mn_error.hpp"

/**
 *  Base wrapper class around FreeRTOS's implementation of mutexes.
 *  These objects are not recursively acquirable. Calling lock() twice from
 *  the same Thread (i.e. task) will deadlock.
 */
class basic_mutex : public basic_semaphore {
public:
  /**
   * The Ctor 
   */
  basic_mutex();
  /**
   * Create the Mutex 
   * 
   * @return 'ERR_MUTEX_OK' the mutex are created, 'ERR_MUTEX_ALREADYINIT' the mutex are already created and
   *         'ERR_MUTEX_CANTCREATEMUTEX' on error.
   * 
   * @note When in config "MN_THREAD_CONFIG_MUTEX_CLASS" with MN_THREAD_CONFIG_BINARY_SEMAPHORE 
   *       then create the mutex with 'xSemaphoreCreateBinary' (esp32 online guids basic) and
   *       with 'MN_THREAD_CONFIG_MUTEX_CLASS' create the mutex with 'xSemaphoreCreateMutex'
   */
  virtual int create();

  /**
   * Destroyed the Mutex
   * 
   * @return 'ERR_MUTEX_OK' the mutex are destroyed 
   * or ERR_MUTEX_NOTINIT when mutex not created
   */
  virtual int destroy();

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

  /**
   * Try to lock the mutex
   * 
   * @note call lock with timeout from 0
   * 
   * @return true if the Lock was acquired, false when not
   */
  virtual bool try_lock();
};

using mutex_t = basic_mutex;



#endif
