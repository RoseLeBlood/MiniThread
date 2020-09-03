#ifndef __MINLIB_MNTHREAD_CONFIG_H__
#define __MINLIB_MNTHREAD_CONFIG_H__

#include "freertos/FreeRTOS.h"

#define MN_THREAD_MAJOR_VERSION 1
#define MN_THREAD_MINOR_VERSION 5
#define MN_THREAD_DEBUG_VERSION 0

#define MN_THREAD_CONFIG_MUTEX      1
#define MN_THREAD_CONFIG_SPINLOCK   2
#define MN_THREAD_CONFIG_BINSPHORE  3

#define MN_THREAD_CONFIG_YES        1
#define MN_THREAD_CONFIG_NO         2


/**
 * MN_THREAD_CONFIG_BINSPHORE  mutex created with "xSemaphoreCreateBinary" (default)
 * MN_THREAD_CONFIG_MUTEX mutex created with  "xSemaphoreCreateMutex"
 */
#define MN_THREAD_CONFIG_MUTEX_CLASS MN_THREAD_CONFIG_BINSPHORE   

/**
 * Default lock type using in the thread class
 * MN_THREAD_CONFIG_MUTEX:      using the mutex as default lock type
 * MN_THREAD_CONFIG_SPINLOCK:   using the spinlock as default lock type
 */
#define MN_THREAD_CONFIG_AUTOLOCK MN_THREAD_CONFIG_MUTEX

/**
 * Condition variable support for this libary
 *'MN_THREAD_CONFIG_YES' or 'MN_THREAD_CONFIG_NO'
 */
#define MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES

/** Ab hier nichts verändern | DO NOT EDIT AFTER THIS LINE!!!
 * =================================================================
 */

/**
 * Usind Thread Lock Type
 * @note Don't edit
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
