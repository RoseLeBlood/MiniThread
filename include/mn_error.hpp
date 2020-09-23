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
#ifndef MINLIB_ESP32_ERROR_
#define MINLIB_ESP32_ERROR_

/**
 * No Error
 */
#define NO_ERROR                        0

/**
 * No Error in one of the mutex function
 */
#define ERR_MUTEX_OK                    NO_ERROR
/**
 * The mutex allready created
 */ 
#define ERR_MUTEX_ALREADYINIT			-1
/**
 * The mutex is not created, please call create first
 */ 
#define ERR_MUTEX_NOTINIT				-2
/**
 * The mutex can't create
 */ 
#define ERR_MUTEX_CANTCREATEMUTEX		-3
/**
 * It timed out on lock the mutex - mutex are not locked
 */ 
#define ERR_MUTEX_LOCK                  -4
/**
 * Error to unlock the mutex, call lock first
 */ 
#define ERR_MUTEX_UNLOCK                -5


// -------------------------------- 

/**
 * No Error in one of the spinlock or semaphore function
 */
#define ERR_SPINLOCK_OK                 NO_ERROR
/**
 * The spinlock or semaphore allready created
 */ 
#define ERR_SPINLOCK_ALREADYINIT		-1
/**
 * The spinlock or semaphore is not created, please call create first
 */ 
#define ERR_SPINLOCK_NOTINIT			-2
/**
 * The spinlock or semaphore can't create
 */ 
#define ERR_SPINLOCK_CANTCREATESPINLOCK	-3
/**
 * It timed out on lock the spinlock or semaphore - spinlock or semaphore are not locked
 */ 
#define ERR_SPINLOCK_LOCK               -4
/**
 * Error to unlock the spinlock or semaphore, call lock first
 */ 
#define ERR_SPINLOCK_UNLOCK             -5
/**
 * Count is bigger are the max count - spinlock not created
 */ 
#define ERR_SPINLOCK_BAD_INITIALCOUNT   -6

// --------------------------------

/**
 * No Error in one of the Thread function
 */
#define ERR_TASK_OK                   NO_ERROR
/**
 * The Lock objects are maked error 
 */ 
#define ERR_TASK_CANTINITMUTEX		-4
/**
 * The thread (i.e. task) can not start unknown error
 */
#define ERR_TASK_CANTSTARTTHREAD		-5
/**
 * The threah function is null
 */
#define ERR_TASK_THREADFUNCNOTSET		-6
/**
 * The thread is not running
 */
#define ERR_TASK_NOTRUNNING			-7
/**
 * The thread can not start, becourse the thread is allready started
 */
#define ERR_TASK_ALREADYRUNNING		-8

// --------------------------------

/**
 * No Error in one of the Tasklet function
 */
#define ERR_COROUTINE_OK                   NO_ERROR
/**
 * Can not create the lock object for the tasklet, tasklet not created
 */
#define ERR_COROUTINE_CANTINITLOCKT  		-4
/**
 * Tasklet can not created 
 */ 
#define ERR_COROUTINE_CANTSTART   		-5

// --------------------------------

/**
 * No Error in one of the Queue function
 */
#define ERR_QUEUE_OK                    NO_ERROR
/**
 * The queue is allready Created
 */
#define ERR_QUEUE_ALREADYINIT			-1
/**
 * The queue can't created
 */
#define ERR_QUEUE_CANTCREATE			-2
/**
 * The queue is not created
 */
#define ERR_QUEUE_NOTCREATED            -3
/**
 * The item can not enqueued in the queue
 */
#define ERR_QUEUE_ADD                   -4
/**
 * The item can not remove from the queue
 */
#define ERR_QUEUE_REMOVE                -5
/**
 * No items in the queue
 */
#define ERR_QUEUE_PEEK                  -6
/**
 * Can not create the lock object for the blocking queue
 */ 
#define ERR_QUEUE_CANTCREATE_LOCK       -99

/**
 * No Error in one of the Timer function
 */
#define ERR_TIMER_OK                    NO_ERROR
/**
 * The timer is allready Created
 */
#define ERR_TIMER_ALREADYINIT			-1
/**
 * The timer can't created
 */
#define ERR_TIMER_CANTCREATE			-2
/**
 * The timer is not created
 */
#define ERR_TIMER_NOTCREATED            -3
/**
 * The timer can not start
 */
#define ERR_TIMER_AKTIVATE              -4
/**
 * The timer can not stop
 */
#define ERR_TIMER_INAKTIVATE            -4
/**
 * The timer can not reset
 */
#define ERR_TIMER_RESET                 -4

/**
 * No Error in one of the workqueue function
 */
#define ERR_WORKQUEUE_OK                    NO_ERROR
/**
 * The workqueue is allready Created
 */
#define ERR_WORKQUEUE_ALREADYINIT			-1
/**
 * The workqueue can't created
 */
#define ERR_WORKQUEUE_CANTCREATE			-2
/**
 * Warning 
 */
#define ERR_WORKQUEUE_WARNING               -3
/**
 * The mutex are not 
 */
#define ERR_WORKQUEUE_CANTINITMUTEX         -4
/**
 * The item can not add to the workqueue
 */
#define ERR_WORKQUEUE_ADD                   -5





#endif
