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
