#ifndef MINLIB_ESP32_ERROR_
#define MINLIB_ESP32_ERROR_

#define NO_ERROR                        0

#define ERR_MUTEX_OK                    NO_ERROR
#define ERR_MUTEX_ALREADYINIT			-1
#define ERR_MUTEX_NOTINIT				-2
#define ERR_MUTEX_CANTCREATEMUTEX		-3
#define ERR_MUTEX_LOCK                  -4
#define ERR_MUTEX_UNLOCK                -5

#define ERR_SPINLOCK_OK                 NO_ERROR
#define ERR_SPINLOCK_ALREADYINIT		-1
#define ERR_SPINLOCK_NOTINIT			-2
#define ERR_SPINLOCK_CANTCREATESPINLOCK	-3
#define ERR_SPINLOCK_LOCK               -4
#define ERR_SPINLOCK_UNLOCK             -5

#define ERR_SPINLOCK_BAD_INITIALCOUNT   -6


#define ERR_THREAD_OK                   NO_ERROR
#define ERR_THREAD_CANTINITMUTEX		-4
#define ERR_THREAD_CANTSTARTTHREAD		-5
#define ERR_THREAD_THREADFUNCNOTSET		-6
#define ERR_THREAD_NOTRUNNING			-7
#define ERR_THREAD_ALREADYRUNNING		-8

#define ERR_TASKLET_OK                   NO_ERROR
#define ERR_TASKLET_CANTINITLOCKT  		-4
#define ERR_TASKLET_CANTSTART   		-5

#define ERR_QUEUE_OK                    NO_ERROR
#define ERR_QUEUE_ALREADYINIT			-1
#define ERR_QUEUE_CANTCREATE			-2
#define ERR_QUEUE_NOTCREATED            -3
#define ERR_QUEUE_ADD                   -4
#define ERR_QUEUE_REMOVE                -5
#define ERR_QUEUE_PEEK                  -6

#endif
