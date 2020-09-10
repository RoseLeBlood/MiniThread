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
#ifndef MINLIB_ESP32_REC_MUTEX_
#define MINLIB_ESP32_REC_MUTEX_

#include "mn_mutex.hpp"

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
 */
class recursive_mutex : public basic_mutex {
public:
  recursive_mutex();

  /**
   * Create the Mutex 
   * 
   * @return 'ERR_MUTEX_OK' the mutex are created, 'ERR_MUTEX_ALREADYINIT' the mutex are already created and
   *         'ERR_MUTEX_CANTCREATEMUTEX' on error.
   * 
   * @note create the mutex with 'xSemaphoreCreateRecursiveMutex'
   */
  virtual int create();
  
  /**
   *  Lock the Mutex.
   *
   *  @param Timeout How long to wait to get the Lock until giving up. (default = 0xffffffffUL)
   *  @return ERR_MUTEX_OK if the Lock was acquired, ERR_MUTEX_LOCK if it timed out.
   *  or ERR_MUTEX_NOTINIT when mutex not created
   * 
   *  @note use 'xSemaphoreTakeRecursiveFromISR' in ISR context or 'xSemaphoreTakeRecursive' in all other
   */
	virtual int lock(unsigned int timeout = (unsigned int) 0xffffffffUL);

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

#endif
