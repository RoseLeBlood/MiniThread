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
#ifndef MINLIB_ESP32_COUNTING_SEMAPHORE_
#define MINLIB_ESP32_COUNTING_SEMAPHORE_

#include "mn_basic_semaphore.hpp"

class basic_counting_semaphore : public basic_semaphore {
public:
  /**
   *  Constructor to set the basic properties for this spinlock.
   * 
   *  @param count Must not be greater than maxCount.
   *  @param maxcount Must be greater than 0.
   */
  basic_counting_semaphore(int count = MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT, 
                           int maxcount = MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT);

  virtual ~basic_counting_semaphore() { destroy(); }
  /**
   * Create the counting semaphore  
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are created, 'ERR_SPINLOCK_ALREADYINIT' the mutex are already created,
   *         'ERR_SPINLOCK_BAD_INITIALCOUNT' when the initial count greater than maxcount is and 
   *         'ERR_SPINLOCK_CANTCREATEMUTEX' on error.
   * 
   */
  virtual int create(); 

  /**
   * Destroyed the counting semaphore
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are destroyed 
   * or ERR_SPINLOCK_NOTINIT when mutex not created
   */
  virtual int destroy();

  /**
   * Set the initial count
   * 
   * @param count the new initial count
   * @note only call befor create, after this have no effect
   */ 
  void set_initial_count(int count)       { m_uiCount = count; }
  /**
   * Set the max count
   * 
   * @param count the new max count
   * @note only call befor create, after this have no effect
   */ 
  void set_max_count(int count)           { m_uiMaxCount = count; }

   /**
   * Get the current count value. 
   *  
   * @return The current count and 0 if the semaphore is not available.
   */ 
  int get_count() const;
protected:
  int m_uiCount;
  int m_uiMaxCount;
};

using counting_semaphore_t = basic_counting_semaphore;

#endif