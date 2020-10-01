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
#ifndef _MINLIB_AUTOLOCK_H_
#define _MINLIB_AUTOLOCK_H_

#include "mn_config.hpp"
#include "mn_mutex.hpp"
#include "mn_semaphore.hpp"
#include "mn_critical.hpp"


/**
 *  Synchronization helper class that leverages the C++ language to help
 *  prevent deadlocks.
 *  This is a C++11 feature that allows LockObject 
 *  Locking and Unlocking to behave following an RAII style. 
 *  The constructor of this helper object locks the LockObject. 
 *  The destructor unlocks the LockObject. 
 */
template <class LOCK = basic_mutex>
class  basic_autolock {
public:
  /**
   *  Create a basic_autolock with a specific LockType.
   *
   *  @post The LockObject will be locked.
   */
	basic_autolock(LOCK &m) : m_ref_lock(m) {
    m_ref_lock.lock();
  }
  /**
   *  Destroy a basic_autolock.
   *
   *  @post The LockObject will be unlocked.
   */
	~basic_autolock() {
     m_ref_lock.unlock();
  }

  /**
   * Helper for lock sections, see example
   * 
   * @example @code
   * mutex_t mutex; mutex.create();
   * 
   * if( (basic_autolock<mutex_t>(mutex) lk) ) {
   *   locked
   * }
   * unlocked
   */ 
  explicit operator bool() { return m_ref_lock.is_initialized(); }

   /**
     *  We do not want a copy constructor.
     */
   basic_autolock(const basic_autolock&) = delete;
   basic_autolock& operator=(const basic_autolock&) = delete;
private:
  /**
   *  Reference to the LockObject we locked, so it can be unlocked
   *  in the destructor.
   */
	LOCK &m_ref_lock;
};

/**
 * A autolock type for counting_semaphore_t objects
 */
using autocsemp_t = basic_autolock<counting_semaphore_t>;

/**
 * A autolock type for binary_semaphore_t objects
 */
using autobinsemp_t = basic_autolock<binary_semaphore_t>;

/**
 * A autolock type for mutex_t objects
 */
using automutx_t = basic_autolock<mutex_t>;


/**
 * A autolock type for basic_critical_lock objects
 * 
 * @note lock enter in the critial section and with unlock leave the critical section
 */
using auto_critical_t = basic_autolock<basic_critical_lock>;

/**
 * A autolock type for interrupts_lock_t objects
 * 
 * @note the lock disabled the interrupts and save the state, 
 * with unlock the interrupts are enabled and set the olt states  
 */
using auto_interrupt_t = basic_autolock<interrupts_lock_t>;

/**
 * A autolock type for sheduler_lock_t objects
 * 
 * @note lock disable the shedular and unlock enabled it
 */
using auto_schedular_t = basic_autolock<sheduler_lock_t>;


#if (MN_THREAD_CONFIG_RECURSIVE_MUTEX == MN_THREAD_CONFIG_YES)
#include "mn_recursive_mutex.hpp"
/**
 * A autolock type for remutex_t objects
 */
using autoremutx_t = basic_autolock<remutex_t>;
#endif

#if MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_MUTEX
  using LockType_t = mutex_t;
#elif MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_BINARY_SEMAPHORE
  using LockType_t = binary_semaphore_t;
#elif MN_THREAD_CONFIG_LOCK_TYPE == MN_THREAD_CONFIG_COUNTING_SEMAPHORE
  using LockType_t = counting_semaphore_t;
#endif

using autolock_t = basic_autolock<LockType_t>;

#endif
