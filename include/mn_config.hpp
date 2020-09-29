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

#ifndef __MINLIB_MNTHREAD_CONFIG_H__
#define __MINLIB_MNTHREAD_CONFIG_H__

/**
 * All cunfig properties can override in your sdkconfig.h 
 */ 

#include "sdkconfig"
#include "freertos/FreeRTOS.h"

///@brief Major Version, the first number
#define MN_THREAD_MAJOR_VERSION 1
/// @brief Minor Version, the 2. number
#define MN_THREAD_MINOR_VERSION 91
/// @brief  Prozent for next minor jump
#define MN_THREAD_DEBUG_VERSION 23 

/// @brief The mini thread version as string
#define MN_THREAD_VERSION_STRING          "1.91-23"

/// @brief Pre defined helper values for config items - Use a mutex
#define MN_THREAD_CONFIG_MUTEX                1
/// @brief Pre defined helper value for config items - Use a counting semaphore
#define MN_THREAD_CONFIG_COUNTING_SEMAPHORE   2
/// @brief Pre defined values for config items - Use a binary semaphore
#define MN_THREAD_CONFIG_BINARY_SEMAPHORE     3

/// @brief Pre defined helper values for config items - Use for aktivating
#define MN_THREAD_CONFIG_YES        1
/// @brief Pre defined helper values for config items - Use for deaktivating
#define MN_THREAD_CONFIG_NO         2

/// @brief The max number of usable cores 
#define MN_THREAD_CONFIG_CORE_MAX   (portNUM_PROCESSORS - 1)

/** 
 * @brief Pre defined values for config items -
 * Use for indicating the task has no affinity core 
 */  
#define MN_THREAD_CONFIG_CORE_IFNO  tskNO_AFFINITY
/** 
 * @brief Pre defined values for config items -
 * Use for run the config task on core 0
 */  
#define MN_THREAD_CONFIG_CORE_ONE   (MN_THREAD_CONFIG_CORE_IFNO + 1)
/** 
 * @brief Pre defined values for config items -
 * Use for run the config task on core 1
 */ 
#define MN_THREAD_CONFIG_CORE_TWO   (MN_THREAD_CONFIG_CORE_ONE + 1)

  
/** 
 * @brief Pre defined on which core must run the task, can override in the create 
 * function
 */ 
#define MN_THREAD_CONFIG_DEFAULT_CORE               MN_THREAD_CONFIG_CORE_IFNO
/** 
 * @brief Pre defined on which core must run the work queue task, 
 * can override in the create function
 */ 
#define MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE     MN_THREAD_CONFIG_CORE_TWO   

/*
Start the config part
*/

#ifndef configMAX_PRIORITIES 
    /**
     * The max priority for FreeRTOS Task priority
     */
    #define configMAX_PRIORITIES 25
#endif
 
 
#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_IDLE
    /**
     * @brief Task priority for basic_task::PriorityIdle 
     */
    #define MN_THREAD_CONFIG_CORE_PRIORITY_IDLE     tskIDLE_PRIORITY
#endif
 
#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_LOW
    /**
     * @brief  Task priority for basic_task::PriorityLow 
     */
    #define MN_THREAD_CONFIG_CORE_PRIORITY_LOW      1
#endif

#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_NORM
    /**
     * @brief  Task priority for basic_task::PriorityNormal 
     */
    #define MN_THREAD_CONFIG_CORE_PRIORITY_NORM     2
#endif

#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT
    /**
     * @brief Task priority for basic_task::PriorityHalfCritical 
     */ 
    #define MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT  (configMAX_PRIORITIES-1)/2  
#endif

#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_URGENT  
    /**
     * @brief Task priority for basic_task::PriorityUrgent 
     */
    #define MN_THREAD_CONFIG_CORE_PRIORITY_URGENT   (configMAX_PRIORITIES-2)
#endif

#ifndef MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL
    /**
     * @brief Task priority for basic_task::PriorityCritical 
     */ 
    #define MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL (configMAX_PRIORITIES-1)
#endif


#ifndef MN_THREAD_CONFIG_LOCK_TYPE
    /**
     * Default lock type using in the thread class 
     * MN_THREAD_CONFIG_MUTEX:      using the mutex as default lock type
     * MN_THREAD_CONFIG_BINARY_SEMAPHORE using the binary semaphore as default lock type
     * MN_THREAD_CONFIG_COUNTING_SEMAPHORE: using the counting semaphore as default lock type
     * default: MN_THREAD_CONFIG_BINARY_SEMAPHORE 
     */
    #define MN_THREAD_CONFIG_LOCK_TYPE MN_THREAD_CONFIG_BINARY_SEMAPHORE
#endif

#ifndef MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT
    /**
     * Condition variable support for this libary
     *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO' 
     * default: MN_THREAD_CONFIG_YES
     */
    #define MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES
#endif


#ifndef MN_THREAD_CONFIG_MSGTASK_MAX_MESSAGES
    /**
     * How many message can stored in the basic_message_task queue - default: 5
     * 
     * @note for message task support must be Condition variable support enabled 
     * @see MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT
     */ 
    #define MN_THREAD_CONFIG_MSGTASK_MAX_MESSAGES   5
#endif


#ifndef MN_THREAD_CONFIG_FOREIGIN_TASK_SUPPORT
    /**
     * Foreign support for this libary. When MN_THREAD_CONFIG_YES then 
     * can with the class foreign_thread_t foreign tasks handled
     *  
     *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO' 
    * default: MN_THREAD_CONFIG_YES
    */
    #define MN_THREAD_CONFIG_FOREIGIN_TASK_SUPPORT MN_THREAD_CONFIG_YES
#endif


#ifndef MN_THREAD_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT
    /**
     * Very Importend !!! 
     * Handled the timeout for get_next_job timeout in the work queue 
     * Only in this config medificated
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT  512
#endif
 
#ifndef MN_THREAD_CONFIG_WORKQUEUE_SINGLE_MAXITEMS
    /**
     * How many work items to queue in the workqueue single-threaded default: 8
     */
    #define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_MAXITEMS      8
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_SINGLE_STACKSIZE
    /**
     * Stak size for the workqueue single-threaded thread 
     * default: (configMINIMAL_STACK_SIZE * 2) 
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_STACKSIZE     (configMINIMAL_STACK_SIZE * 2)
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY
    /**
     * Default Priority for the workqueue single-threaded thread 
     * default: basic_thread::PriorityLow 
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY      basic_task::PriorityLow
#endif



#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER
    /**
     * How many worker threads run in the workqueue multi-threaded
     * default: 4
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER         4
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS
    /**
     * How many work items to queue in the workqueue multi-threaded
     * default: 16
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS      8
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE
    /**
     * Stak size for the workqueue multi-threaded for all worked thread 
     * default: (configMINIMAL_STACK_SIZE * 2) 
     */
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE     (configMINIMAL_STACK_SIZE * 2)
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY
    /**
     * Default Priority for the workqueue multi-threaded for all worked thread 
     * default: basic_thread::PriorityLow
     */
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY      basic_task::PriorityLow
#endif



#ifndef MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT
    /**
     * The default initial timeout for semaphore lock (counting semaphore and binary semaphore)
     * Can override
     * default  (unsigned int) 0xffffffffUL   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT  (unsigned int) 0xffffffffUL   
#endif          



#ifndef MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT
    /**
     * Default min initial count for counting semaphore, can override in constuctor default  1  
     */ 
    #define MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT       1  
#endif
 
#ifndef MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT
    /**
     * Default max initial count for counting semaphore, can override in constuctor
     * default  0x7fffffff  
     */ 
    #define MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT       0x7fffffff
#endif
 
#ifndef MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT
    /**
     * The default initial timeout for mutex lock (recursive and normal)
     * Can override
     * default  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT      (unsigned int) 0xffffffffUL 
#endif 

#ifndef MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT
    /**
     * The default initial timeout for all queues 
     * Can override
     * default  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT      (unsigned int) 0xffffffffUL  
#endif

#ifndef MN_THREAD_CONFIG_TIMEOUT_COROUTINE_DEFAULT  
    /**
     * The default initial timeout for coroutine (recursive and normal)
     * Can override
     * default  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_COROUTINE_DEFAULT  (unsigned int) 0xffffffffUL 
#endif


#ifndef MN_THREAD_CONFIG_PREVIEW_FUTURE
    /**
     * Whenn MN_THREAD_CONFIG_YES then activate develop unsafe future for next major version 
     * not for production use
     * 
     * default  MN_THREAD_CONFIG_NO  
     */
    #define MN_THREAD_CONFIG_PREVIEW_FUTURE              MN_THREAD_CONFIG_YES
#endif


#ifndef MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING
    /**
     * When MN_THREAD_CONFIG_YES then cheaking for recursive mutex
     * and when FreeRTOS recursive mutex supported then can you use thes
     */ 
    #define MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING     MN_THREAD_CONFIG_YES   
#endif
/** Ab hier nichts verändern | DO NOT EDIT AFTER THIS LINE!!!
 * =================================================================
 */

#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES
    /// Use for beta configuration, the preview major version
    #define MN_THREAD_BETA_MAJOR_VERSION 2
    /// Use for beta configuration, the preview minor version
    #define MN_THREAD_BETA_MINOR_VERSION 0
    /// Use for beta configuration, the preview version as string
    #define MN_THREAD_BETA_VERSION_STRING    "-beta2.0"
#else 
    #define MN_THREAD_BETA_VERSION_STRING  
    #define MN_THREAD_BETA_MAJOR_VERSION 0
    #define MN_THREAD_BETA_MINOR_VERSION 0  
#endif // MN_THREAD_CONFIG_PREVIEW_FUTURE

/**
 * Supported the FeeRTOS RECURSIVE_MUTEXES ?
 * @note Don't edit
 */
#if (configUSE_RECURSIVE_MUTEXES == 1) 
    #if (MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING == MN_THREAD_CONFIG_YES)
            /**
             * Enabled Build with RECURSIVE_MUTEXES support
             */ 
            #define MN_THREAD_CONFIG_RECURSIVE_MUTEX MN_THREAD_CONFIG_YES
        #else 
            /**
             * Diabled Build with RECURSIVE_MUTEXES support
             */ 
            #define MN_THREAD_CONFIG_RECURSIVE_MUTEX MN_THREAD_CONFIG_NO
            #warning ("recursive mutex are not supported")
    #endif
#endif

#endif
