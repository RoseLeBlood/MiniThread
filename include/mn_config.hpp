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

#include "freertos/FreeRTOS.h"

#define MN_THREAD_MAJOR_VERSION 1
#define MN_THREAD_MINOR_VERSION 6
#define MN_THREAD_DEBUG_VERSION 1

#define MN_THREAD_CONFIG_MUTEX                1
#define MN_THREAD_CONFIG_COUNTING_SEMAPHORE   2
#define MN_THREAD_CONFIG_BINARY_SEMAPHORE     3

#define MN_THREAD_CONFIG_YES        1
#define MN_THREAD_CONFIG_NO         2

#define MN_THREAD_CONFIG_CORE_IFNO  tskNO_AFFINITY
#define MN_THREAD_CONFIG_CORE_ONE   0
#define MN_THREAD_CONFIG_CORE_TWO   1
#define MN_THREAD_CONFIG_CORE_MAX   (portNUM_PROCESSORS - 1)
  

#define MN_THREAD_CONFIG_DEFAULT_CORE               MN_THREAD_CONFIG_CORE_IFNO
#define MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE     MN_THREAD_CONFIG_CORE_TWO   

#ifndef configMAX_PRIORITIES 
#define configMAX_PRIORITIES 25
#endif

enum tt { test = ( 1UL | portPRIVILEGE_BIT ) };
 
#define MN_THREAD_CONFIG_CORE_PRIORITY_IDLE     tskIDLE_PRIORITY
#define MN_THREAD_CONFIG_CORE_PRIORITY_LOW      1
#define MN_THREAD_CONFIG_CORE_PRIORITY_NORM     2
#define MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT  (configMAX_PRIORITIES-1)/2     
#define MN_THREAD_CONFIG_CORE_PRIORITY_URGENT   (configMAX_PRIORITIES-2)
#define MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL (configMAX_PRIORITIES-1)


 /**
 * Default lock type using in the thread class 
 * MN_THREAD_CONFIG_MUTEX:      using the mutex as default lock type
 * MN_THREAD_CONFIG_BINARY_SEMAPHORE using the binary semaphore as default lock type
 * MN_THREAD_CONFIG_COUNTING_SEMAPHORE: using the counting semaphore as default lock type
 * default: MN_THREAD_CONFIG_BINARY_SEMAPHORE 
 */
#define MN_THREAD_CONFIG_LOCK_TYPE MN_THREAD_CONFIG_BINARY_SEMAPHORE

/**
 * Condition variable support for this libary
 *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO' 
 * default: MN_THREAD_CONFIG_YES
 */
#define MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES

/**
 * Foreign support for this libary. When MN_THREAD_CONFIG_YES then 
 * can with the class foreign_thread_t foreign tasks handled
 *  
 *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO' 
 * default: MN_THREAD_CONFIG_YES
 */
#define MN_THREAD_CONFIG_FOREIGIN_TASK_SUPPORT MN_THREAD_CONFIG_YES


#define MN_THREAD_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT  512
/**
 * How many work items to queue in the workqueue single-threaded
 * default: 8
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_MAXITEMS      8
/**
 * Stak size for the workqueue single-threaded thread 
 * default: (configMINIMAL_STACK_SIZE * 2) 
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_STACKSIZE     (configMINIMAL_STACK_SIZE * 2)
/**
 * Default Priority for the workqueue single-threaded thread 
 * default: basic_thread::PriorityLow 
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY      basic_task::PriorityLow


/**
 * How many worker threads run in the workqueue multi-threaded
 * default: 4
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER         4
/**
 * How many work items to queue in the workqueue multi-threaded
 * default: 16
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS      8
/**
 * Stak size for the workqueue multi-threaded for all worked thread 
 * default: (configMINIMAL_STACK_SIZE * 2) 
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE     (configMINIMAL_STACK_SIZE * 2)
/**
 * Default Priority for the workqueue multi-threaded for all worked thread 
 * default: basic_thread::PriorityLow
 */ 
#define MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY      basic_task::PriorityLow


/**
 * default  (unsigned int) 0xffffffffUL   
 */ 
#define MN_THREAD_CONFIG_TIMEOUT_SEMAPHORE_DEFAULT  (unsigned int) 0xffffffffUL             

// Counting Semaphore Default Config
/**
 * default  1  
 */ 
#define MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT       1  
/**
 * default  0x7fffffff  
 */   
#define MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT       0x7fffffff
/**
 * default  (unsigned int) 0xffffffffUL)   
 */   
#define MN_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT      (unsigned int) 0xffffffffUL  
/**
 * default  (unsigned int) 0xffffffffUL)   
 */   
#define MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT      (unsigned int) 0xffffffffUL  
/**
 * default  (unsigned int) 0xffffffffUL)   
 */   
#define MN_THREAD_CONFIG_TIMEOUT_COROUTINE_DEFAULT  (unsigned int) 0xffffffffUL 

/** Ab hier nichts verändern | DO NOT EDIT AFTER THIS LINE!!!
 * =================================================================
 */

/**
 * Supported the FeeRTOS RECURSIVE_MUTEXES ?
 * @note Don't edit
 */
#if (configUSE_RECURSIVE_MUTEXES == 1)
/**
 * Enabled Build with RECURSIVE_MUTEXES support
 */ 
#define MN_THREAD_CONFIG_RECURSIVE_MUTEX MN_THREAD_CONFIG_YES
#else 
/**
 * Diabled Build with RECURSIVE_MUTEXES support
 */ 
#define MN_THREAD_CONFIG_RECURSIVE_MUTEX MN_THREAD_CONFIG_NO
#endif


#endif
