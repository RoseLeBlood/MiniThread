/** This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
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
**/

#ifndef __MINLIB_MNTHREAD_CONFIG_H__
#define __MINLIB_MNTHREAD_CONFIG_H__

#include "freertos/FreeRTOS.h"

#define MN_THREAD_MAJOR_VERSION 1
#define MN_THREAD_MINOR_VERSION 6
#define MN_THREAD_DEBUG_VERSION 0

#define MN_THREAD_CONFIG_MUTEX                1
#define MN_THREAD_CONFIG_COUNTING_SEMAPHORE   2
#define MN_THREAD_CONFIG_BINARY_SEMAPHORE     3

#define MN_THREAD_CONFIG_YES        1
#define MN_THREAD_CONFIG_NO         2

#define MN_THREAD_CONFIG_CORE_NO    -1
#define MN_THREAD_CONFIG_CORE_ONE   0
#define MN_THREAD_CONFIG_CORE2_TWO  1

//CONFIG_PTHREAD_TASK_CORE_DEFAULT

#define MN_THREAD_CONFIG_MAX_CPUS                   2 
#define MN_THREAD_CONFIG_DEFAULT_CORE               MN_THREAD_CONFIG_CORE_NO
#define MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE     MN_THREAD_CONFIG_CORE2_TWO   


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
 * How many work items to queue in the work queue engine 
 * default: 8
 */ 
#define MN_THREAD_CONFIG_WORK_QUEUE_MAX_WORK_ITEMS   8
/**
 * Stak size for the work queue thread 
 * default: (configMINIMAL_STACK_SIZE * 2) 
 */ 
#define MN_THREAD_CONFIG_WORK_QUEUE_STACK_SIZE       (configMINIMAL_STACK_SIZE * 2)
/**
 * Default Priority for the work queue thread 
 * default: (tskIDLE_PRIORITY + 1) 
 */ 
#define MN_THREAD_CONFIG_WORK_QUEUE_PRIORITY         (tskIDLE_PRIORITY + 1)

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
