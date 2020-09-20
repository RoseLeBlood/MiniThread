/*
  This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
  Copyright (c) 2018 Amber-Sophia Schroeck
  
  The Mini Thread Library is free software; you can redistribute it and/or modify  
  it under the terms of the GNU Lesser General Public License as published by  
  the Free Software Foundation, version 3, or (at your option) any later version.
 
  The Mini Thread Library is distributed in the hope that it will be useful, but 
  WITHOUT ANY WARRANTY; without even the implied warranty of 
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
  General Public License for more details.
 
  You should have received a copy of the GNU Lesser General Public
  License along with the Mini Thread  Library; if not, see
  <https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_ESP32_THREAD_
#define MINLIB_ESP32_THREAD_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"
 
/**
 *  Wrapper class around FreeRTOS's implementation of a task.
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your threads should
 *  be derived from the basic_thread class. Then implement the virtual on_thread
 *  function.
 */
class  basic_thread {
public:
  basic_thread() { }

  /**
   *  Constructor for this thread.
   *
   *  @param strName Name of the thread. Only useful for debugging.
   *  @param uiPriority FreeRTOS priority of this Thread.
   *  @param usStackDepth Number of "words" allocated for the Thread stack. default configMINIMAL_STACK_SIZE
   */
  basic_thread(char const* strName, unsigned int uiPriority,
       unsigned short  usStackDepth = configMINIMAL_STACK_SIZE);
  
  
  /**
   *  Our destructor. Delete the task 
   */
	virtual ~basic_thread();

  /**
   *  Create and starts the thread.
   *
   *  This is the API call that actually starts the thread running. 
   *  It creates a backing FreeRTOS task. By separating object creation 
   *  from starting the Thread, it solves the pure virtual fuction call 
   *  failure case. Call after creating the Task the function on_create
   * 
   * @param uiCore on which core run this thread (i. e. task ), -1 then create with 'xTaskCreate' and
   * all other use 'xTaskCreatePinnedToCore'
   * 
   * @return ERR_THREAD_OK The task are creating, 'ERR_THREAD_CANTINITMUTEX' on error creating the using 
   * LockObjets, the task is not created, 'ERR_THREAD_ALREADYRUNNING' the Task is allready running and
   * 'ERR_THREAD_CANTSTARTTHREAD' can't create the task
   */
  virtual int                   create(int uiCore = MN_THREAD_CONFIG_DEFAULT_CORE);

  /**
   * Destroy and delete the task and call the function 'on_kill'
   * 
   * @return ERR_THREAD_OK The tasx are destroyed and 'ERR_THREAD_NOTRUNNING' the task is not running
   */ 
  int                   kill();

  /**
   * Is the thread (i. e. task) running?
   * 
   * @return true If the thread (i. e. task) running, false If not
   */ 
  bool                  is_running();

  /**
   * Get the debug name of this thread (i. e. task) 
   * 
   * @return The name of this thread (i. e. task) 
   */ 
  const char*           get_name();
  /**
   * Get the priority of this thread (i. e. task) 
   * 
   * @return The priority
   */ 
  unsigned int          get_priority();
  /**
   * Get the stack depth of this thread (i. e. task) 
   * 
   * @return The stack depth
   */ 
  unsigned short        get_stackdepth();
  /**
   * Accessor to get the thread's backing task handle.
   * There is no setter, on purpose.
   * 
   * @return FreeRTOS task handle.
   */ 
  xTaskHandle           get_handle();
  /**
   * Get the return value of this thread (i. e. task) - after run 
   * 
   * @return The return value 
   */
  void*                 get_return_value();
  /**
   * Get the time since start of this thread (i. e. task) 
   * 
   * @return The time since start of this thread (i. e. task) 
   */ 
  uint32_t              get_time_since_start();
  /**
   * Get the FreeRTOS Thread Numberid of this thread (i. e. task) 
   * 
   * @return The FreeRTOS Thread (i. e. task)  Number
   */ 
  uint32_t              get_id();
  /**
   * Get the core number of this thread (i. e. task) run
   * 
   * @return The core number
   */ 
  uint32_t              get_on_core();

  /**
   *  Set the priority of this thread.
   *
   *  @param uiPriority The thread's new priority.
   */
  void                  setPriority(unsigned int uiPriority);

  /**
   *  Suspend this thread.
   *
   *  @note While a Thread can suspend() itself, it cannot resume() 
   *  itself, becauseit's suspended.
   */
  void                  suspend();
  /**
   *  Resume a specific thread.
   */
  void                  resume();

  /**
   * This virtual function call on creating, use for user code
   */ 
  virtual void          on_create() {  }
  /**
   * This virtual function call on kill, use for user code
   */ 
  virtual void          on_kill()   {  }

  /**
   * Implementation of your actual thread code.
   * You must override this function.
   * 
   * @return Your return your task function, get with get_return_value()
   */
  virtual void*         on_thread() = 0;

  /**
   *  Called on exit from your on_thread() routine. 
   *  
   *  It is optional whether you implement this or not.
   *
   *  If you allow your Thread to exit its on_thread method, 
   *  implementing a on_cleanup method allows you to call 
   *  your Thread's destructor.
   */ 
  virtual void          on_cleanup() { }

  /**
   * Get the root task of this task list
   * 
   * @return The root task
   */ 
  basic_thread*         get_root();
  /**
   * Get the child task of this task
   * 
   * @return The child task
   */ 
  basic_thread*         get_child();

  /**
   * Add a child task to this task. 
   * 
   * @return True The child tasx are add and false when not
   * 
   * @note For example this task handle the WiFi connection and the child the TCP Connection
   * on signal or broadcast this task, will signal and broadcast the child too.
   */ 
  bool                  add_child_thread(basic_thread* thread);

public:
  /**
   * Suspend the given thread.
   *
   * @param t The given task to suspend
   * 
   * @note While a Thread can suspend() itself, it cannot resume() 
   * itself, becauseit's suspended.
   */
  static void suspend(basic_thread *t)  { t->suspend(); }

  /**
   * Resume the given thread.
   *
   * @param t The given task to resume
   */
  static void resume(basic_thread *t)   {   t->resume(); }

  /**
   *  Yield the scheduler.
   */
  static void yield()                   { taskYIELD(); }
  /**
   *  sleep this thread for n seconds
   *
   *  @param secs How long seconds to sleep the thread.
   */
  static void sleep(unsigned int secs)     { ::mn_sleep(secs); }
  /**
   *  sleep this thread for n micro seconds
   *
   *  @param secs How long micro seconds to sleep the thread.
   */
  static void usleep(unsigned int usec)     { ::mn_usleep(usec); }
  /**
   * pause execution for a specified time
   * @note see Linux nanosleep function
  */
  static void nsleep(const struct timespec *req, struct timespec *rem)     {
    ::mn_nsleep(req, rem);
  }

  static void lock(basic_thread * t)    { t->m_runningMutex->lock(); }
  static void unlock(basic_thread * t)    { t->m_runningMutex->unlock(); }

protected:
  /**
   *  Adapter function that allows you to write a class
   *  specific on_task() function that interfaces with FreeRTOS.
   */
  static void runtaskstub(void* parm);
  /**
   * Internal function 
   */ 
	void thread_started();

protected:
  /**
   *  Reference to the underlying task handle for this thread.
   *  Can be obtained from GetHandle().
   */
  xTaskHandle handle;
  /**
   *  The name of this thread.
   */
  const char* m_strName;
  /**
   *  A saved / cached copy of what the Thread's priority is.
   */
  unsigned int m_uiPriority;
  /**
   *  Stack depth of this Thread, in words.
   */
  unsigned short m_usStackDepth;
  /**
   * The return value from user task routine
   */ 
  void* m_retval;

  /**
   *  Flag whether or not the LockObject was created.
   */
  bool m_bMutexInit;
  /**
   *  Flag whether or not the Thread was started.
   */
  bool m_bRunning;
  /**
   * The FreeRTOS Thread Number
   */ 
  uint32_t m_iID;
  /**
   * A saved / cached copy of which core this task is running on
   */ 
  uint32_t m_iCore;
  /**
   * Lock Objekt for thread safty
   */ 
  LockType_t *m_runningMutex;
  /**
   * Lock Objekt for thread safty
   */ 
  LockType_t *m_contextMutext;
  /**
   * Lock Objekt for thread safty
   */ 
  LockType_t *m_continuemutex, *m_continuemutex2;

  /**
   * The child task pointer
   */ 
  basic_thread *m_pChild;
  /**
   * The parent task pointer of this tas
   */
  basic_thread *m_pParent;
};


/**
 * Wrapper class around FreeRTOS's implementation of a task, 
 * for foreign miniThread Threads 
 */
class foreign_thread : public basic_thread {
public:
  /**
   * Constructor - the current thread
   * @note please call create, for creating the usings Locktyps
   */ 
  foreign_thread();
  /**
   * Constructor - from a FreeRTOS handle
   * @note please call create, for creating the usings Locktyps
   */ 
  foreign_thread(void* t);

  /**
   * Create and set the usings Semaphores and informations
   */ 
  int                   create(int uiCore = -1);

protected:
  /**
   * Override - Do nothings and never call
   * @return NULL
   */
  void*         on_thread() { return NULL; }
};

using thread_t = basic_thread;
using foreign_thread_t = foreign_thread;

#endif
