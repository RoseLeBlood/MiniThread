/**
 * @file
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_THREAD_
#define MINLIB_ESP32_THREAD_

#include "mn_config.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <string>

#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_basic_timespan.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"
#include "mn_eventgroup.hpp"

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
  class  basic_task : MN_ONSIGLETN_CLASS {
  public:
    /**
     * @brief Task priority
     */
    enum  class priority {
      Idle = MN_THREAD_CONFIG_CORE_PRIORITY_IDLE,           /*!< Priority for no real time operations - idle task */
      Low = MN_THREAD_CONFIG_CORE_PRIORITY_LOW,             /*!< Priority for low operation  */
      Normal = MN_THREAD_CONFIG_CORE_PRIORITY_NORM,         /*!< Priority for tasks for normal operatins - user interfaces for example */
      HalfCritical = MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT,/*!< Priority for tasks with normal deadlines and not a lot of processings*/
      Urgent = MN_THREAD_CONFIG_CORE_PRIORITY_URGENT,       /*!< priority for tasks with short deadlines and a lot of processings */
      Critical = MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL    /*!< Priority for critical tasks - the highest priority  */
    };

    /** Task states returned by get_state(). */
    enum class state {
      Running = 0,	  /*!< A task is querying the state of itself, so must be running. */
      Ready,			    /*!< The task being queried is in a read or pending ready list. */
      Blocked,		    /*!< The task being queried is in the Blocked state. */
      Suspended,		  /*!< The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
      Deleted		    /*!< The task being queried has been deleted, but its TCB has not yet been freed. */
    };
  public:
    /**
     * Basic Constructor for this task.
     * The priority is PriorityNormal and use MN_THREAD_CONFIG_MINIMAL_STACK_SIZE for the stack size
     */
    basic_task() : basic_task(" ", priority::Normal, MN_THREAD_CONFIG_MINIMAL_STACK_SIZE) { }
    /**
     * Constructor for this task.
     *
     * @param strName Name of the Task. Only useful for debugging.
     * @param uiPriority FreeRTOS priority of this Task.
     * @param usStackDepth Number of "words" allocated for the Task stack. default MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
     */
    explicit basic_task(std::string strName, basic_task::priority uiPriority = basic_task::priority::Normal,
        unsigned short  usStackDepth = MN_THREAD_CONFIG_MINIMAL_STACK_SIZE);


    /**
     * @brief Our destructor. Delete the task
     */
    virtual ~basic_task();

    /**
     * @brief Create and starts the Task.
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
     *  - ERR_TASK_CANTINITMUTEX on error creating the using LockObjets, the task is not created.
     *  - ERR_TASK_ALREADYRUNNING the Task is allready running.
     *  - ERR_TASK_CANTSTARTTHREAD can't create the tas.
     *	- ERR_TASK_CANTCREATEEVENTGROUP can't create the event group, the task is not created.
     */
    virtual int           start(int uiCore = MN_THREAD_CONFIG_DEFAULT_CORE);

    /**
     * @brief Destroy and delete the task and call the function 'on_kill'
     *
     * @return
     *  - ERR_TASK_OK The task are destroyed
     *  - ERR_TASK_NOTRUNNING The task was not running
     */
    int                   kill();


    /**
     * @brief Is the Task  running?
     *
     * @return true If the task  running, false If not
     */
    bool                  is_running();

    /**
     * @brief Get the debug name of this task
     *
     * @return The name of this task
     */
    std::string          get_name();
    /**
     * @brief Get the priority of this task
     *
     * @return The priority
     */
    basic_task::priority get_priority();
    /**
     * @brief Get the stack depth of this task
     *
     * @return The stack depth
     */
    unsigned short        get_stackdepth();
    /**
     * @brief Accessor to get the task's backing task handle.
     * There is no setter, on purpose.
     *
     * @return FreeRTOS task handle.
     */
    xTaskHandle           get_handle();

    /**
     * @brief Get the return value of this task  - after run
     *
     * @return The return value
     */
    int                  get_return_value();
    /**
     * @brief Get the time since start of this task
     *
     * @return The time since start of this task
     */
    timespan_t           get_time_since_start() const;
    /**
     * @brief Get the FreeRTOS task Numberid of this task
     *
     * @return The FreeRTOS task   Number
     */
    int32_t              get_id();
    /**
     * @brief Get the core number of this task  run
     *
     * @return The core number
     */
    int32_t              get_on_core();

    /**
     * @brief Set the priority of this task.
     *
     * @param uiPriority The task's new priority.
     */
    void                  set_priority(basic_task::priority uiPriority);

    /**
     * @brief Suspend this task.
     *
     * @note While a task can suspend() itself, it cannot resume()
     * itself, becauseit's suspended.
     */
    void                  suspend();
    /**
     * @brief Resume a specific task.
     */
    void                  resume();

    /**
     * @brief join the task, Wait in other task to end this task.
     * @param xTickTimeout The maximum amount of ticks to wait.
     * @note call never in the this task, then wait this task to end this task.
     * @return
	 *		- ERR_TASK_NOTRUNNING Call start first.
	 *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
	 *		- NO_ERROR No error
     */
    int 				  join(unsigned int xTickTimeout = portMAX_DELAY);

   /**
     * @brief join the task, Wait in other task to end this task.
     * @param time The maximum amount of time to wait.
     * @note call never in the this task, then wait this task to end this task.
     * @return
	 *		- ERR_TASK_NOTRUNNING Call start first.
	 *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
	 *		- NO_ERROR No error
     */
    int				  	join(timespan_t time);

    /**
     * @brief Wait for start the task.
     * @param xTickTimeout The maximum amount of ticks to wait.
     * @note call never in the this task, then wait this task to start this task.
     * @return
	 *		- ERR_TASK_NOTRUNNING Call start first.
	 *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
	 *		- NO_ERROR No error
     */
    int				  	wait(unsigned int xTimeOut);

    /**
     * @brief Wait for start the task.
     * @param xTickTimeout The maximum amount of time to wait.
     * @note call never in the this task, then wait this task to start this task.
     * @return
	 *		- ERR_TASK_NOTRUNNING Call start first.
	 *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
	 *		- NO_ERROR No error
     */
    int				  	wait(timespan_t time);

    /**
     * @brief This virtual function call on creating, use for user code
     * It is optional whether you implement this or not.
     */
    virtual void          on_start() {  }
    /**
     * @brief This virtual function call on kill, use for user code
     * It is optional whether you implement this or not.
     */
    virtual void          on_kill()   {  }

    /**
     * @brief Implementation of your actual task code.
     * You must override this function.
     *
     * @return Your return your task function, get with get_return_value()
     */
    virtual int         on_task() { return ERR_TASK_OK; }

    /**
     * @brief Called on exit from your on_task() routine.
     *
     * @note It is optional whether you implement this or not.
     * If you allow your task to exit its on_task method,
     */
    virtual void          on_cleanup() { }

    /**
     * @brief Get the state of the task
     *
     * @return The state of the task at the time the function was called.
     */
    state               get_state();

    /**
     *  @brief Operator to get the task's backing task handle.
     * @return FreeRTOS task handle.
     */
    operator xTaskHandle () { return (xTaskHandle)get_handle(); }

    /**
	 * @brief Operator to get the ID assigned to the task.
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
     * @brief Suspend the given task.
     *
     * @param t The given task to suspend
     *
     * @note While a task can suspend() itself, it cannot resume()
     * itself, becauseit's suspended.
     */
    static void suspend(basic_task *t)  { t->suspend(); }

    /**
     * @brief Resume the given task.
     *
     * @param t The given task to resume
     */
    static void resume(basic_task *t)   {   t->resume(); }

    /**
     * @brief Yield the scheduler.
     */
    static void yield()                   { taskYIELD(); }
    /**
     * @brief sleep this task for n seconds
     *
     * @param secs How long seconds to sleep the task.
     */
    static void sleep(unsigned int secs)     { mn::delay(timespan_t(0, 0, 0, secs)); }
    /**
     * @brief sleep this task for n micro seconds
     *
     * @param secs How long micro seconds to sleep the task.
     */
    static void usleep(unsigned int usec)     { mn::delay(timespan_t(0, 0, 0, 0, usec)); }
    /**
     * @brief pause execution for a specified time
     * @note see Linux nanosleep function
     */
    static void nsleep(const timespan_t& req, timespan_t* rem)     {
      	mn::ndelay(req, rem);
    }

    /**
     * @brief Get current number of tasks
     *
     * @return The number of tasks that the real time kernel is currently managing.
     * This includes all ready, blocked and suspended tasks.  A task that
     * has been deleted but not yet freed by the idle task will also be
     * included in the count.
     */
    static uint32_t get_tasks();

    /**
     * @brief Is the given task the current running task ?
     *
     */
    static bool is_current(basic_task* task) {
      return basic_task::get_self()->m_pHandle == task->m_pHandle;
    }

    /**
     * @brief Get the current task
     *
     * @return The current task
     */
    static basic_task* get_self();
  protected:
    /**
     * @brief Adapter function that allows you to write a class
     * specific on_task() function that interfaces with FreeRTOS.
     */
    static void runtaskstub(void* parm);
  protected:
    /**
     * @brief Lock Objekt for task safty
     */
    mutable LockType_t m_runningMutex, m_contextMutext, m_continuemutex;
  protected:
    /**
     * @brief The name of this task.
     */
    std::string m_strName;
    /**
     * @brief A saved / cached copy of what the task's priority is.
     */
    basic_task::priority m_uiPriority;
    /**
     *@brief  Stack depth of this task, in words.
     */
    unsigned short m_usStackDepth;
    /**
     * @brief The return value from user task routine
     */
    int m_retval;
    /**
     *  @brief Flag whether or not the task was started.
     */
    bool m_bRunning;
    /**
     * @brief The FreeRTOS task Number
     */
    int32_t m_iID;
    /**
     * @brief A saved / cached copy of which core this task is running on
     */
    int32_t m_iCore;
    /**
     * @brief Reference to the underlying task handle for this task.
     * @note Can be obtained from get_handle().
     */
    xTaskHandle m_pHandle;

    event_group_t m_eventGroup;

    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
      StaticTask_t m_TaskBuffer;
      StackType_t  m_stackBuffer[MN_THREAD_CONFIG_STACK_DEPTH];
    #endif
  };
  /**
   * @brief using the basic_task as task_t type
   */
  using task_t = basic_task;
}



#endif
