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
 * @ingroup base
 */

/**
 * All cunfig properties can override in your sdkconfig.h 
 */ 
#ifdef ESP_BUILD_WITH_SDKCONFIG
#include "sdkconfig.h"
#endif

//#include "freertos/FreeRTOS.h"

//Start helper section - do not edit!
//==================================

///@brief Major Version, the first number
#define MN_THREAD_MAJOR_VERSION 2
/// @brief Minor Version, the 2. number
#define MN_THREAD_MINOR_VERSION 0
/// @brief  Debug version
#define MN_THREAD_DEBUG_VERSION 1

/// @brief The mini thread version as string
#define MN_THREAD_VERSION_STRING          "2.0.1"

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
#define MN_THREAD_CONFIG_CORE_ONE   0
/** 
 * @brief Pre defined values for config items -
 * Use for run the config task on core 1
 */ 
#define MN_THREAD_CONFIG_CORE_TWO   1

/** 
 * @brief Pre defined values for config items -
 * @note corrently use for MN_THREAD_CONFIG_BOARD 
 * Set board type to esp32 
 */ 
#define MN_THREAD_CONFIG_ESP32      0

/** 
 * @brief Pre defined values for config items -
 * @note corrently use for MN_THREAD_CONFIG_BOARD 
 * Set board type to no spezial use, currently no esp32 spezial apis to use
 */ 
#define MN_THREAD_CONFIG_BOARD_NODEFS   99

/** 
 * @brief Pre defined values for config items -
 * @note corrently use for MN_THREAD_CONFIG_BOARD 
 * Set board type to other - not work 
 */ 
#define MN_THREAD_CONFIG_OTHER      1

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
    #define MN_THREAD_CONFIG_STACK_DEPTH 8192
#endif

#ifndef MN_THREAD_CONFIG_BOARD    
    #define MN_THREAD_CONFIG_BOARD  MN_THREAD_CONFIG_BOARD_NODEFS
#endif
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


/** 
 * @brief For variable MN_THREAD_CONFIG_ALLOCATOR_DEFAULT, with this use 
 * the normel system as defualt allocator
 */ 
#define MN_THREAD_CONFIG_ALLOCATOR_SYSTEM          1
/** 
 * @brief For variable MN_THREAD_CONFIG_ALLOCATOR_DEFAULT, with this use 
 * the spiram system as defualt allocator
 */
#define MN_THREAD_CONFIG_ALLOCATOR_SPIRAM          2

//Start Config section
//==================================
#ifndef MN_THREAD_CONFIG_SIZE_TYPE 
    /**
     * The type for mn::size_t 
     */ 
    #define MN_THREAD_CONFIG_SIZE_TYPE          long unsigned int
#endif


#ifndef MN_THREAD_CONFIG_USE_EXCEPTIONS 
    /**
     * Use for lock objects exceptions?
     * default: MN_THREAD_CONFIG_NO
     */ 
    #define MN_THREAD_CONFIG_USE_EXCEPTIONS MN_THREAD_CONFIG_NO
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
     * @note default: lock type using in the thread class 
     * MN_THREAD_CONFIG_MUTEX:      using the mutex as default lock type
     * MN_THREAD_CONFIG_BINARY_SEMAPHORE using the binary semaphore as default lock type
     * MN_THREAD_CONFIG_COUNTING_SEMAPHORE: using the counting semaphore as default lock type
     * @note default: MN_THREAD_CONFIG_BINARY_SEMAPHORE 
     */
    #define MN_THREAD_CONFIG_LOCK_TYPE MN_THREAD_CONFIG_BINARY_SEMAPHORE
#endif

#ifndef MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT
    /**
     * Condition variable support for this libary
     *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO' 
     * @note default: MN_THREAD_CONFIG_YES
     */
    #define MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES
#endif


#ifndef MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
    /**
     * The minimal stack size for a mn task
     * @note default: 2048
     */ 
    #define MN_THREAD_CONFIG_MINIMAL_STACK_SIZE		2048
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
    * @note default: MN_THREAD_CONFIG_YES
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
     * @note default: MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_STACKSIZE     MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY
    /**
     * @note default: Priority for the workqueue single-threaded thread 
     * @note default: basic_thread::PriorityLow 
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY      basic_task::PriorityLow
#endif



#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER
    /**
     * How many worker threads run in the workqueue multi-threaded
     * @note default: 4
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER         4
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS
    /**
     * How many work items to queue in the workqueue multi-threaded
     * @note default: 16
     */ 
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS      8
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE
    /**
     * Stak size for the workqueue multi-threaded for all worked thread 
     * @note default: MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
     */
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE     MN_THREAD_CONFIG_MINIMAL_STACK_SIZE
#endif

#ifndef MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY
    /**
     * @note default: Priority for the workqueue multi-threaded for all worked thread 
     * @note default: basic_thread::PriorityLow
     */
    #define MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY      basic_task::PriorityLow
#endif



#ifndef MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT
    /**
     * The default initial timeout for semaphore lock (counting semaphore and binary semaphore)
     * Can override
     * @note default:  (unsigned int) 0xffffffffUL   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT  (unsigned int) 0xffffffffUL   
#endif          



#ifndef MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT
    /**
     * @note default: min initial count for counting semaphore, can override in constuctor default  1  
     */ 
    #define MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT       1  
#endif
 
#ifndef MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT
    /**
     * @note default: max initial count for counting semaphore, can override in constuctor
     * @note default:  0x7fffffff  
     */ 
    #define MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT       0x7fffffff
#endif
 
#ifndef MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT
    /**
     * The default initial timeout for mutex lock (recursive and normal)
     * Can override
     * @note default:  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT      (unsigned int) 0xffffffffUL 
#endif 

#ifndef MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT
    /**
     * The default initial timeout for all queues 
     * Can override
     * @note default:  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT      (unsigned int) 0xffffffffUL  
#endif

#ifndef MN_THREAD_CONFIG_TIMEOUT_COROUTINE_DEFAULT  
    /**
     * The default initial timeout for coroutine (recursive and normal)
     * Can override
     * @note default:  (unsigned int) 0xffffffffUL)   
     */ 
    #define MN_THREAD_CONFIG_TIMEOUT_COROUTINE_DEFAULT  (unsigned int) 0xffffffffUL 
#endif


#ifndef MN_THREAD_CONFIG_PREVIEW_FUTURE
    /**
     * Whenn MN_THREAD_CONFIG_YES then activate develop unsafe future for next major version 
     * not for production use
     * 
     * @note default:  MN_THREAD_CONFIG_NO  
     */
    #define MN_THREAD_CONFIG_PREVIEW_FUTURE              MN_THREAD_CONFIG_NO
#endif


#ifndef MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING
    /**
     * When MN_THREAD_CONFIG_YES then cheaking for recursive mutex
     * and when FreeRTOS recursive mutex supported then can you use thes
     */ 
    #define MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING     MN_THREAD_CONFIG_YES   
#endif

#ifndef MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST
    /**
     * Add a Task automatic to basic_task_list?
     */ 
    #define MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST          MN_THREAD_CONFIG_NO
#endif

#ifndef MN_THREAD_CONFIG_DEBUG
    #define  MN_THREAD_CONFIG_DEBUG     MN_THREAD_CONFIG_YES
#endif

#ifndef MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING  
    /// Using pre defines (usings) for basic types for the shared objects
    #define MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING MN_THREAD_CONFIG_YES
#endif //MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING

#ifndef MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC  
    /// Use the magic guard for detect heap memory corruption - default: MN_THREAD_CONFIG_NO
    #define MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC MN_THREAD_CONFIG_NO
#endif //MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING

#ifndef MN_THREAD_CONFIG_MEMPOOL_MAGIC_START
    ///The start magic guard byte for detect heap memory corruption, not use in simple version
    #define MN_THREAD_CONFIG_MEMPOOL_MAGIC_START   0x6d //109
#endif

#ifndef MN_THREAD_CONFIG_MEMPOOL_MAGIC_END
    ///The end magic guard byte for detect heap memory corruption, not use in simple version
    #define MN_THREAD_CONFIG_MEMPOOL_MAGIC_END     0xa8 //168 
#endif

#ifndef MN_THREAD_CONFIG_MEMPOOL_USETIMED     
    /**
     * Use for mempool the timed version and xTicksToWait
     * When true than is the class name basic_mempool_%name%_timed 
     * and If false then basic_mempool_%name%
     */
    #define MN_THREAD_CONFIG_MEMPOOL_USETIMED     MN_THREAD_CONFIG_YES
#endif

#ifndef MN_THREAD_CONFIG_ALLOCATOR_DEFAULT
    /**
     * Which allocator use for default_allocator_t
     * default: MN_THREAD_CONFIG_ALLOCATOR_SYSTEM
     */ 
    #define MN_THREAD_CONFIG_ALLOCATOR_DEFAULT        MN_THREAD_CONFIG_ALLOCATOR_SYSTEM
#endif


//Alignment
#ifndef MN_THREAD_CONFIG_BASIC_ALIGNMENT
    #define MN_THREAD_CONFIG_BASIC_ALIGNMENT     sizeof(unsigned char*)
#endif

#ifndef MN_THREAD_CONFIG_TICKHOOK_MAXENTRYS         
    ///The max entrys are hold the tickhook queue
    #define MN_THREAD_CONFIG_TICKHOOK_MAXENTRYS     10 
#endif


#ifndef MN_THREAD_CONFIG_STACK_TYPE       
    ///The stack using type
    #define MN_THREAD_CONFIG_STACK_TYPE     unsigned long
#endif


/** 
 * Ab hier nichts verändern | DO NOT EDIT AFTER THIS LINE!!!
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
    #endif // (MN_THREAD_CONFIG_RECURSIVE_MUTEX_CHEAKING == MN_THREAD_CONFIG_YES)

#endif //configUSE_RECURSIVE_MUTEXES


#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE
    ///The mempool object is not used
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE 1
#endif

#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED
    ///The mempool object is used, not free
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED 0
#endif

#ifndef MN_THREAD_CONFIG_USE_TRACE
    #define MN_THREAD_CONFIG_USE_TRACE configUSE_TRACE_FACILITY
#endif

#include "mn_defines.hpp"

#endif //__MINLIB_MNTHREAD_CONFIG_H__
