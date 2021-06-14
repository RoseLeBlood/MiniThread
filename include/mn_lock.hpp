/**
 * @file
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
 * The Mini Thread Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.
 */
#ifndef _MINLIB_ILOCK_INTERFACE_H_
#define _MINLIB_ILOCK_INTERFACE_H_

#include "mn_config.hpp"

#include <assert.h>
#include <freertos/FreeRTOS.h>
#include <time.h>

#include "mn_functional.hpp"
#include "mn_copyable.hpp"
#include "mn_error.hpp"
#include "mn_micros.hpp"

/**
 * Macro for locked sections
 *
 * @param LOCK The typ of lock class
 * @param OBJECT The lock object
 *
 * @example @code
 * mutex_t mutex;
 * LOCKED_SECTION(mutex_t, mutex) {
 *  locked
 * }
 * unlocked
 * @endcode
 *
 * @ingroup lock
 */
#define LOCKED_SECTION(LOCK, OBJECT) if( (mn::basic_autolock<LOCK> lock(OBJECT)) )


#define UNLOCKED_SECTION(LOCK, OBJECT) if( (mn::basic_autounlock<LOCK> lock(OBJECT)) )
namespace mn {
    /**
     * Interface for all lock types in this library
     * This is an abstract base class.
     * To use this, you need to subclass it. All of your LockObjetcs should
     * be derived from the ILockObject class. Then implement the virtual lock,
     * unlock and is_initialized functions.
     *
     * @ingroup Interface
     * @ingroup lock
     */
    class ILockObject  : MN_ONSIGLETN_CLASS {
    public:
		using this_type = ILockObject;

		ILockObject() { }

        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) = 0;

        virtual int time_lock(const struct timespec *timeout) = 0;
        /**
         *  unlock (give) a semaphore.
         */
        virtual int unlock() = 0;

        /**
         * Try to lock the ILockObject
         *
         * @note call lock with timeout from 0
         *
         * @return true if the Lock was acquired, false when not
         */
        virtual bool try_lock() {
            return (lock(0) == 0);
        }

        /**
         * Is the ILockObject created (initialized) ?
         *
         * @return true if the ILockObject created (initialized) and false when not
         */
        virtual bool is_initialized() const = 0;

        /**
         * @brief Is locked?
         * @return True if locked and false when not.
         */
        virtual bool is_locked() const = 0;
    };




  /**
   *  Synchronization helper class that leverages the C++ language to help
   *  prevent deadlocks.
   *  This is a C++11 feature that allows ILockObject
   *  Locking and Unlocking to behave following an RAII style.
   *  The constructor of this helper object locks the ILockObject.
   *  The destructor unlocks the ILockObject.
   *
   * @ingroup semaphore
   * \ingroup lock
   */
  template <class LOCK>
  class  basic_autolock  : MN_ONSIGLETN_CLASS  {
  public:
    /**
     *  Create a basic_autolock with a specific LockType, without timeout
     *
     *  @post The LockObject will be locked.
     */
    basic_autolock(LOCK &m)
      : m_ref_lock(m) {
      assert( (m_ref_lock.lock(portMAX_DELAY) != NO_ERROR) );
    }
    /**
     * Create a basic_autolock with a specific LockType, with timeout
     *
     * @param xTicksToWait How long to wait to get the lock until giving up.
     *
     * @post The LockObject will be locked.
     */
    basic_autolock(LOCK &m, unsigned long xTicksToWait)
      : m_ref_lock(m) {
      assert( (m_ref_lock.lock(xTicksToWait) != NO_ERROR) );
    }
    /**
     *  Destroy a basic_autolock.
     *
     *  @post The LockObject will be unlocked, when the lock Object locked
     */
    ~basic_autolock() {
        m_ref_lock.unlock();
    }

    operator bool () {
		return m_ref_lock.is_locked();
    }
  private:
    /**
     *  Reference to the LockObject we locked, so it can be unlocked
     *  in the destructor.
     */
    LOCK &m_ref_lock;
  };


  /**
   *  This is a C++11 feature that allows ILockObject
   *  Locking and unlocking to behave following an RAII style.
   *  The constructor of this helper object unlocks the ILockObject.
   *  The destructor locks the ILockObject.
   *
   * @ingroup semaphore
   * \ingroup lock
   */
  template <class LOCK>
  class  basic_autounlock  : MN_ONSIGLETN_CLASS  {
  public:
    /**
     *  Create a basic_autounlock with a specific LockType.
     *  @post The LockObject will be unlocked.
     */
    basic_autounlock(LOCK &m)
      : m_ref_lock(m) {
        m_ref_lock.unlock();
        m_xTicksToWait = portMAX_DELAY;
	}

	basic_autounlock(LOCK &m, unsigned long xTicksToWait)
      : m_ref_lock(m) {
        m_ref_lock.unlock();
        m_xTicksToWait = xTicksToWait;
	}
    /**
     *  Destroy a basic_autounlock.
     *
     *  @post The LockObject will be locked.
     */
    ~basic_autounlock() {
        assert( (m_ref_lock.lock(m_xTicksToWait) != NO_ERROR) );
    }

    void set_timeout(unsigned long xTicksToWait = portMAX_DELAY) {
    	m_xTicksToWait = xTicksToWait;
    }

    operator bool () {
		return !(m_ref_lock.is_locked());
    }
  private:
    /**
     *  Reference to the LockObject we unlocked, so it can be locked
     *  in the destructor.
     */
    LOCK &m_ref_lock;

    /**
     * @brief A saved / coped lock timeout for lock in the destructor.
     */
     unsigned long m_xTicksToWait;
  };

}

#endif
