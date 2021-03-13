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
#ifndef MINLIB_ESP32_THREAD_
#define MINLIB_ESP32_THREAD_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"

#include "mn_eventgroup.hpp"

#include <string>

namespace mn {
    
  /**
   * @brief Wrapper class around FreeRTOS's implementation of a task.
   *
   * @note To use this, you need to subclass it. All of your task should
   * be derived from the basic_task class. Then implement the virtual on_task
   * function.
   * 
   * @code 
   * 
   * #include <miniThread.hpp>
   * // The number of test tasks
   * #define NUMBER_OF_TEST_TASK      2
   * 
   * // The tost task class
   * class hello_world_task : public basic_task {
   * public:
   *     hello_world_task() : basic_task("HelloWorld", 1) { }
   * 
   *     virtual void*  on_task() override { 
   *         int id = get_id(); // get the task id from this task
   *         int core = get_on_core(); // get the core on run this task
   * 
   *         while(true) { // infinity run
   *             // print the hello world message to the std output 
   *             // out on core: 0 and with id: 0: "[0 @ 0] Hello World!" 
   *             printf("[%d @ %d] Hello World!!\n", id, core );
   *         }
   *          
   *         return NULL; 
   *     }
   * };
   * extern "C" void app_main() {
   *     hello_world_task tasks[NUMBER_OF_TEST_TASK];
   * 
   *     for(int i = 0; i < NUMBER_OF_TEST_TASK; i++) {
   *         tasks[i].start( i % 2 );
   *     }
   *     mn_panic();
   * }
   * // Examplo output:
   * // [0 @ 0] Hello World!
   * // [1 @ 1] Hello World!
   * // [0 @ 0] Hello World!
   * // ...
   * @endcode
   * 
   * @author Amber-Sophia Schröck
   * @ingroup task
   */
  class  basic_task {
  public:
    /**
     * @brief Task priority 
     */
    enum  priority {
      PriorityIdle = MN_THREAD_CONFIG_CORE_PRIORITY_IDLE,           /*!< Priority for no real time operations - idle task */
      PriorityLow = MN_THREAD_CONFIG_CORE_PRIORITY_LOW,             /*!< Priority for low operation  */
      PriorityNormal = MN_THREAD_CONFIG_CORE_PRIORITY_NORM,         /*!< Priority for tasks for normal operatins - user interfaces for example */
      PriorityHalfCritical = MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT,/*!< Priority for tasks with normal deadlines and not a lot of processings*/
      PriorityUrgent = MN_THREAD_CONFIG_CORE_PRIORITY_URGENT,       /*!< priority for tasks with short deadlines and a lot of processings */
      PriorityCritical = MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL    /*!< Priority for critical tasks - the highest priority  */
    };

    /** Task states returned by get_state(). */
    enum state {
      StateRunning = 0,	  /*!< A task is querying the state of itself, so must be running. */
      StateReady,			    /*!< The task being queried is in a read or pending ready list. */
      StateBlocked,		    /*!< The task being queried is in the Blocked state. */
      StateSuspended,		  /*!< The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
      StateDeleted		    /*!< The task being queried has been deleted, but its TCB has not yet been freed. */
    };
    /** The using events for wait() and join(). */
    enum event { 
      EventStarted = 1 << 24,     /*!< Event for task started */ 
      EventJoin = 1 << 25         /*!< Event for task joined */ 
    };

  public:
    /**
     * Basic Constructor for this task.
     * The priority is PriorityNormal and use 2048 for the stack size
     */
    basic_task() : basic_task(" ", PriorityNormal, 2048) { }

    /**
     * Constructor for this task.
     *
     * @param strName Name of the Task. Only useful for debugging.
     * @param uiPriority FreeRTOS priority of this Task.
     * @param usStackDepth Number of "words" allocated for the Task stack. default 2048
     */
    explicit basic_task(std::string strName, basic_task::priority uiPriority = PriorityNormal,
        unsigned short  usStackDepth = MN_THREAD_CONFIG_MINIMAL_STACK_SIZE);

    basic_task(const basic_task&) = delete;
    basic_task& operator=(const basic_task&) = delete;

    /**
     *  Our destructor. Delete the task
     */
    virtual ~basic_task();

    /**
     * Create and starts the Task.
     *
     * This is the API call that actually starts the Task running.
     * It creates a backing FreeRTOS task. By separating object creation
     * from starting the Task, it solves the pure virtual fuction call
     * failure case. Call after creating the Task the function on_start
     *
     * @param uiCore If the value is MN_THREAD_CONFIG_CORE_IFNO, the created task is not
     * pinned to any CPU, and the scheduler can run it on any core available.
     * Other values indicate the index number of the CPU which the task should
     * be pinned to. Specifying values larger than (portNUM_PROCESSORS - 1) will
     * cause the function to fail.
     *
     * @return 
     *  - ERR_TASK_OK The task are creating, 
     *  - ERR_TASK_CANTINITMUTEX on error creating the using LockObjets, the task is not created, 
     *  - ERR_TASK_ALREADYRUNNING the Task is allready running
     *  - ERR_TASK_CANTSTARTTHREAD can't create the task
     */
    virtual int           start(int uiCore = MN_THREAD_CONFIG_DEFAULT_CORE);

    /**
     * Destroy and delete the task and call the function 'on_kill'
     *
     * @return 
     *  - ERR_TASK_OK The task are destroyed 
     *  - ERR_TASK_NOTRUNNING The task was not running
     */
    int                   kill();

    /**
     * join the task
     * Wait in a other task to end this task 
     * 
     * @param timeval The maximum amount of time to wait.
     * 
     * @note call never in this task, then wait this task to end this task
     */ 
    void                  join(const struct timeval *abs_time);

    /**
     * join the task
     * Wait in a other task to end this task 
     * 
     * @param timeval The maximum amount of time (specified in 'ticks') to wait.
     * 
     * @note call never in this task, then wait this task to end this task
     */
    void                  join(unsigned int timeout = portMAX_DELAY);

    /**
     * Is the Task  running?
     *
     * @return true If the task  running, false If not
     */
    bool                  is_running();

    /**
     * Wait for start the task
     * @param timeval The maximum amount of time to wait.
     */ 
    void                  wait(unsigned int timeout);
    /**
     * Wait for start the task
     * @param timeval The maximum amount of time to wait.
     */ 
    void                  wait(const struct timeval *abs_time);

    /**
     * Get the debug name of this task
     *
     * @return The name of this task
     */
    std::string          get_name();
    /**
     * Get the priority of this task
     *
     * @return The priority
     */
    basic_task::priority get_priority();
    /**
     * Get the stack depth of this task
     *
     * @return The stack depth
     */
    unsigned short        get_stackdepth();
    /**
     * Accessor to get the task's backing task handle.
     * There is no setter, on purpose.
     *
     * @return FreeRTOS task handle.
     */
    xTaskHandle           get_handle();

    /**
     * Get the return value of this task  - after run
     *
     * @return The return value
     */
    void*                 get_return_value();
    /**
     * Get the time since start of this task
     *
     * @return The time since start of this task
     */
    uint32_t              get_time_since_start();
    /**
     * Get the FreeRTOS task Numberid of this task
     *
     * @return The FreeRTOS task   Number
     */
    int32_t              get_id();
    /**
     * Get the core number of this task  run
     *
     * @return The core number
     */
    int32_t              get_on_core();

    /**
     *  Set the priority of this task.
     *
     *  @param uiPriority The task's new priority.
     */
    void                  set_priority(basic_task::priority uiPriority);

    /**
     *  Suspend this task.
     *
     *  @note While a task can suspend() itself, it cannot resume()
     *  itself, becauseit's suspended.
     */
    void                  suspend();
    /**
     *  Resume a specific task.
     */
    void                  resume();

    /**
     * This virtual function call on creating, use for user code
     * It is optional whether you implement this or not.
     */
    virtual void          on_start() {  }
    /**
     * This virtual function call on kill, use for user code
     * It is optional whether you implement this or not.
     */
    virtual void          on_kill()   {  }

    /**
     * Implementation of your actual task code.
     * You must override this function.
     *
     * @return Your return your task function, get with get_return_value()
     */
    virtual void*         on_task() { return NULL; }

    /**
     *  Called on exit from your on_task() routine.
     *
     *  It is optional whether you implement this or not.
     *
     *  If you allow your task to exit its on_task method,
     */
    virtual void          on_cleanup() { }

    /**
     * Get the root task of this task list
     *
     * @return The root task
     */
    basic_task*         get_root();
    /**
     * Get the child task of this task
     *
     * @return The child task
     */
    basic_task*         get_child();

    /**
     * Get the state of the task
     * 
     * @return The state of the task at the time the function was called.
     */ 
    state               get_state();

    /**
     * Add a child task to this task.
     *
     * @return True The child tasx are add and false when not
     *
     * @note For example this task for the WiFi connection and the child the TCP Connection
     * on signal or broadcast this task, will signal and broadcast the child too.
     */
    bool                  add_child_task(basic_task* task);

    /**
     * Operator to get the task's backing task handle.
     * @return FreeRTOS task handle.
     */
    operator xTaskHandle () { return (xTaskHandle)get_handle(); }

    /**
       * Operator to get the ID assigned to the task.
       * @return The ID assigned to the task being queried.
       */ 
    operator int () { return get_id(); }
  public:
    bool operator == (const basic_task &r) const {
      return m_pHandle == r.m_pHandle;
    }

    bool operator != (const basic_task &r) const {
      return !operator==(r);
    }

    bool operator < (const basic_task &r) const {
      return m_pHandle < r.m_pHandle;
    }

    bool operator > (const basic_task &r) const {
      return m_pHandle > r.m_pHandle;
    }
  public:
    /**
     * Suspend the given task.
     * 
     * @param t The given task to suspend
     *
     * @note While a task can suspend() itself, it cannot resume()
     * itself, becauseit's suspended.
     */
    static void suspend(basic_task *t)  { t->suspend(); }

    /**
     * Resume the given task.
     *
     * @param t The given task to resume
     */
    static void resume(basic_task *t)   {   t->resume(); }

    /**
     *  Yield the scheduler.
     */
    static void yield()                   { taskYIELD(); }
    /**
     *  sleep this task for n seconds
     *
     *  @param secs How long seconds to sleep the task.
     */
    static void sleep(unsigned int secs)     { ::mn::sleep(secs); }
    /**
     *  sleep this task for n micro seconds
     *
     *  @param secs How long micro seconds to sleep the task.
     */
    static void usleep(unsigned int usec)     { ::mn::usleep(usec); }
    /**
     * pause execution for a specified time
     * @note see Linux nanosleep function
    */
    static void nsleep(const struct timespec *req, struct timespec *rem)     {
      ::mn::nsleep(req, rem);
    }

    /**
     * Get current number of tasks
     * 
     * @return The number of tasks that the real time kernel is currently managing.
     * This includes all ready, blocked and suspended tasks.  A task that
     * has been deleted but not yet freed by the idle task will also be
     * included in the count.
     */ 
    static uint32_t get_tasks();

    /**
     * Is the given task the current running task ?
     * 
     */ 
    static bool is_current(basic_task* task) {
      return basic_task::get_self()->m_pHandle == task->m_pHandle;
    }

    /**
     * Get the current task
     * 
     * @return The current task
     */ 
    static basic_task* get_self();
  protected:
    /**
     *  Adapter function that allows you to write a class
     *  specific on_task() function that interfaces with FreeRTOS.
     */
    static void runtaskstub(void* parm);
  protected:
    /**
     * Lock Objekt for task safty
     */
    LockType_t m_runningMutex, m_contextMutext, m_continuemutex;
  protected:
    /**
     *  The name of this task.
     */
    std::string m_strName;
    /**
     *  A saved / cached copy of what the task's priority is.
     */
    basic_task::priority m_uiPriority;
    /**
     *  Stack depth of this task, in words.
     */
    unsigned short m_usStackDepth;
    /**
     * The return value from user task routine
     */
    void* m_retval;
    /**
     *  Flag whether or not the task was started.
     */
    bool m_bRunning;
    /**
     * The FreeRTOS task Number
     */
    int32_t m_iID;
    /**
     * A saved / cached copy of which core this task is running on
     */
    int32_t m_iCore;
    /**
     *  Reference to the underlying task handle for this task.
     *  Can be obtained from get_handle().
     */
    xTaskHandle m_pHandle;

    /**
     * The child task pointer 
     */
    basic_task *m_pChild;
    /**
     * The parent task pointer 
     */
    basic_task *m_pParent;

    /**
     * The event group for this task 
     * used or wait() and join()
     */ 
    event_group_t m_event;

    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
      StaticTask_t m_TaskBuffer;
      StackType_t  m_stackBuffer[MN_THREAD_CONFIG_STACK_DEPTH];
    #endif
  };

  using task_t = basic_task;
}



#endif
