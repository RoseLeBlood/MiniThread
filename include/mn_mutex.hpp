#ifndef MINLIB_ESP32_MUTEX_
#define MINLIB_ESP32_MUTEX_

#include "mn_config.hpp"
#include "mn_spinlock.hpp"
#include "mn_error.hpp"

/**
 *  Base wrapper class around FreeRTOS's implementation of mutexes.
 *  These objects are not recursively acquirable. Calling lock() twice from
 *  the same Thread (i.e. task) will deadlock.
 */
class basic_mutex {
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
   * @note When in config "MN_THREAD_CONFIG_MUTEX_CLASS" with MN_THREAD_CONFIG_BINSPHORE 
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
	virtual int lock(unsigned int timeout = (unsigned int) 0xffffffffUL);

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

  /**
   * Is the mutex created (initialized) ?
   * 
   * @return true if the mutex created (initialized) and false when not
   */ 
  bool is_initialized() const 						{ return m_pmutex == NULL; }

  /**
   * Get the FreeRTOS handle
   * 
   * @return the FreeRTOS handle
   */
  void* get_handle()                      { return m_pmutex; }
protected:
  /**
   *  FreeRTOS semaphore handle.
   */
	void* m_pmutex;
};

using mutex_t = basic_mutex;



#endif
