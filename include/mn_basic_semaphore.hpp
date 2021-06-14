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
#ifndef MINLIB_ESP32_BASE_SEMAPHORE_
#define MINLIB_ESP32_BASE_SEMAPHORE_

#include "mn_config.hpp"

#include <time.h>

#include "mn_error.hpp"
#include "mn_lock.hpp"
#include "excp/mn_lock_exptions.hpp"



namespace mn {

  /**
   *
   *  Base wrapper class around FreeRTOS's implementation of semaphores.
   *
   * @ingroup semaphore
   * \ingroup lock
   */
  class basic_semaphore : public ILockObject {
  public:
    /**
     * Construtor
     */
    basic_semaphore();

#if( configSUPPORT_STATIC_ALLOCATION == 0 )
	basic_semaphore(const basic_semaphore& other);
	basic_semaphore(basic_semaphore&& other);
#endif
    /**
     * lock (take) a semaphore.
     *
     * @param timeout How long to wait to get the lock until giving up.
     * @return ERR_SPINLOCK_OK if the Semaphore was locked, ERR_SPINLOCK_LOCK if it timed out.
     *
     * @example
     * basic_semaphore sem = basic_semaphore();
     * sem.lock(100);
     */
    virtual int lock(unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT);

    /**
     *  lock (take) a semaphore.
     *
     *  @param abs_time How long to wait to get the lock until giving up.
     *
     *  @return ERR_SPINLOCK_OK if the Semaphore was locked, ERR_SPINLOCK_LOCK if it timed out.
     */
    virtual int time_lock(const struct timespec *timeout);

    /**
     *  unlock (give) a semaphore.
     *
     *  @return ERR_SPINLOCK_OK if the Semaphore was unlocked, ERR_SPINLOCK_UNLOCK if it failed.
     */
    virtual int unlock();

    /**
     * Get the FreeRTOS handle
     *
     * @return the FreeRTOS handle
     */
    void* get_handle()                      { return m_pSpinlock; }

    /**
     * Get the error code on creating
     * @return The error code on creating
     */
    int   get_error()                       { return m_iCreateErrorCode; }

    virtual bool is_initialized() const     { return m_pSpinlock != NULL; }

    #if configQUEUE_REGISTRY_SIZE > 0
      void set_name(const char* name)       { vQueueAddToRegistry(m_pSpinlock, name); }
    #endif // configQUEUE_REGISTRY_SIZE


    /**
	 * @brief Is locked?
	 * @return True if locked and false when not.
	 */
	virtual bool is_locked() const { return m_isLocked; }
  public:

    bool operator == (const basic_semaphore &r) const {
      return m_pSpinlock == r.m_pSpinlock;
    }

    bool operator != (const basic_semaphore &r) const {
      return !operator==(r);
    }

    bool operator < (const basic_semaphore &r) const {
      return m_pSpinlock < r.m_pSpinlock;
    }

    bool operator > (const basic_semaphore &r) const {
      return m_pSpinlock > r.m_pSpinlock;
    }
  protected:
    /** Set the error codes @param error The error code */
    void  set_error(int error)              { m_iCreateErrorCode = error; }
  protected:
    /**
     *  FreeRTOS semaphore handle.
     */
    void* m_pSpinlock;

    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
      StaticSemaphore_t m_SemaphoreBasicBuffer;
 	#endif

    /**
     * A saved / cached copy of error code on creating
     */
    int m_iCreateErrorCode;
	bool m_isLocked;
  };
}

#endif // MINLIB_ESP32_BASE_SEMAPHORE_
