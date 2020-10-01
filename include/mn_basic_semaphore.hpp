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
#ifndef MINLIB_ESP32_BASE_SEMAPHORE_
#define MINLIB_ESP32_BASE_SEMAPHORE_

#include "mn_error.hpp"
#include "mn_config_preview.hpp"
#include "mn_lock.hpp"

#include "excp/mn_lock_exptions.hpp"

#if MN_THREAD_CONFIG_USE_LOCK_CREATE ==  MN_THREAD_CONFIG_YES
    #if MN_THREAD_CONFIG_DEBUG  == MN_THREAD_CONFIG_YES
        /**
         * Helper util to throw the lockcreate_exception exception, debug version
         */ 
        #define THROW_LOCK_EXP(CODE) throw lockcreate_exception(CODE, __LINE__, __FILE__); 
    #else
        /**
         * Helper util to throw the lockcreate_exception exception, only the code
         */ 
        #define THROW_LOCK_EXP(CODE) throw lockcreate_exception(CODE); 
    #endif // MN_THREAD_CONFIG_DEBUG
#else
    /**
     * This THROW_LOCK_EXP util set only the error code
     */ 
    #define THROW_LOCK_EXP(CODE) set_error(CODE); 
#endif //MN_THREAD_CONFIG_USE_LOCK_CREATE

/**
 *
 *  Base wrapper class around FreeRTOS's implementation of semaphores.
 */
class basic_semaphore : public ILockObject {
public:
  basic_semaphore();
  basic_semaphore(const basic_semaphore& other);
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
	virtual int lock(unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT);

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

  int   get_error()                       { return m_iCreateErrorCode; }
protected:
  void  set_error(int error)              { m_iCreateErrorCode = error; }
protected:
  /**
   *  FreeRTOS semaphore handle.
   */
	void* m_pSpinlock;

  int m_iCreateErrorCode;
  
};

#endif
