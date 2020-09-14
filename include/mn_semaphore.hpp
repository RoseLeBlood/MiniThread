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
#ifndef MINLIB_ESP32_SPINLOCK_
#define MINLIB_ESP32_SPINLOCK_

#include "mn_error.hpp"


/**
 *
 *  Base wrapper class around FreeRTOS's implementation of semaphores.
 */
class basic_semaphore {
public:
  basic_semaphore();
  /**
   *  Aquire (take) a semaphore.
   *
   *  Example of blocking indefinitely:
   *      aSemaphore.Take();
   *
   *  Example of blocking for 100 ticks:
   *      aSemaphore.Take(100);
   *
   *  @param timeout How long to wait to get the Lock until giving up.
   *  @return ERR_SPINLOCK_OK if the Semaphore was acquired, ERR_SPINLOCK_LOCK if it timed out.
   */
	virtual int lock(unsigned int timeout = (unsigned int) 0xffffffffUL);

  /**
   *  Release (give) a semaphore.
   *
   *  @return ERR_SPINLOCK_OK if the Semaphore was released, ERR_SPINLOCK_UNLOCK if it failed.
   */
	virtual int unlock();

  /**
   * Try to lock the spinlock
   * 
   * @note call lock with timeout from 0
   * 
   * @return true if the Lock was acquired, false when not
   */
  virtual bool try_lock();

  /**
   * Is the spinlock created (initialized) ?
   * 
   * @return true if the spinlock created (initialized) and false when not
   */
  bool is_initialized() const;

  /**
   * Get the FreeRTOS handle
   * 
   * @return the FreeRTOS handle
   */
  void* get_handle()                      { return m_pSpinlock; }
protected:
  /**
   *  FreeRTOS semaphore handle.
   */
	void* m_pSpinlock;
  
};

class basic_counting_semaphore : public basic_semaphore {
public:
  /**
   *  Constructor to set the basic properties for this spinlock.
   * 
   *  @param count Must not be greater than maxCount.
   *  @param maxcount Must be greater than 0.
   */
  basic_counting_semaphore(int count = 1, int maxcount = 0x7fffffff);

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
   * Get the current count
   * @return The current count 
   */ 
  int get_count() const;
protected:
  int m_uiCount;
  int m_uiMaxCount;
};


class basic_binary_semaphore : public basic_semaphore {
public:
  basic_binary_semaphore();

  /**
   * Create the binary semaphore  
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are created, 'ERR_SPINLOCK_ALREADYINIT' the mutex are already created,
   *         'ERR_SPINLOCK_BAD_INITIALCOUNT' when the initial count greater than maxcount is and 
   *         'ERR_SPINLOCK_CANTCREATEMUTEX' on error.
   * 
   */
  virtual int create();

  /**
   * Destroyed the binary semaphore
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are destroyed 
   * or ERR_SPINLOCK_NOTINIT when mutex not created
   */
  virtual int destroy();
};



using counting_semaphore_t = basic_counting_semaphore;
using binary_semaphore_t = basic_binary_semaphore;

#endif
