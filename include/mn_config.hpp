/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MINLIB_MNTHREAD_CONFIG_H__
#define __MINLIB_MNTHREAD_CONFIG_H__

#include "freertos/FreeRTOS.h"

#define MN_THREAD_MAJOR_VERSION 1
#define MN_THREAD_MINOR_VERSION 5
#define MN_THREAD_DEBUG_VERSION 0

#define MN_THREAD_CONFIG_MUTEX                1
#define MN_THREAD_CONFIG_COUNTING_SEMAPHORE   2
#define MN_THREAD_CONFIG_BINARY_SEMAPHORE     3

#define MN_THREAD_CONFIG_YES        1
#define MN_THREAD_CONFIG_NO         2


/**
 * MN_THREAD_CONFIG_BINARY_SEMAPHORE  mutex created with "xSemaphoreCreateBinary" (default)
 * MN_THREAD_CONFIG_MUTEX mutex created with  "xSemaphoreCreateMutex" 
 * default: MN_THREAD_CONFIG_MUTEX 
 */
#define MN_THREAD_CONFIG_MUTEX_CLASS MN_THREAD_CONFIG_MUTEX   

/**
 * Default lock type using in the thread class 
 * MN_THREAD_CONFIG_MUTEX:      using the mutex as default lock type
 * MN_THREAD_CONFIG_COUNTING_SEMAPHORE: using the counting semaphore as default lock type
 * default: MN_THREAD_CONFIG_MUTEX 
 */
#define MN_THREAD_CONFIG_AUTOLOCK MN_THREAD_CONFIG_MUTEX

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
 * Usind Thread Lock Type
 * @note Don't edit
 * default: MN_THREAD_CONFIG_MUTEX 
 */ 
#define MN_THREAD_CONFIG_THREAD_LOCK MN_THREAD_CONFIG_MUTEX

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
