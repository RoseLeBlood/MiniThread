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
#define ERR_THREAD_OK                   NO_ERROR
/**
 * The Lock objects are maked error 
 */ 
#define ERR_THREAD_CANTINITMUTEX		-4
/**
 * The thread (i.e. task) can not start unknown error
 */
#define ERR_THREAD_CANTSTARTTHREAD		-5
/**
 * The threah function is null
 */
#define ERR_THREAD_THREADFUNCNOTSET		-6
/**
 * The thread is not running
 */
#define ERR_THREAD_NOTRUNNING			-7
/**
 * The thread can not start, becourse the thread is allready started
 */
#define ERR_THREAD_ALREADYRUNNING		-8

// --------------------------------

/**
 * No Error in one of the Tasklet function
 */
#define ERR_TASKLET_OK                   NO_ERROR
/**
 * Can not create the lock object for the tasklet, tasklet not created
 */
#define ERR_TASKLET_CANTINITLOCKT  		-4
/**
 * Tasklet can not created 
 */ 
#define ERR_TASKLET_CANTSTART   		-5

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

#endif
