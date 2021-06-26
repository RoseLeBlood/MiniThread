/**
 * @file
 * @brief A list of all error codes in this lib.
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
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
 * @ingroup base
 */
#ifndef MINLIB_ESP32_ERROR_
#define MINLIB_ESP32_ERROR_

#include "mn_config.hpp"

/**
 * Begin the helper macro to switch ESP_ERRORS to MNTHREAD_ERROR errors
 * @param _error The holder of the error value
 *
 * @code
 *  int _error = esp_netif_set_mac(m_pNetIf, mac);
 * MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
 * MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
 * MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_ESP_NETIF_IF_NOT_READY, ERR_MNTHREAD_NETIF_NOT_INIT);
 * MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_NOT_SUPPORTED, ERR_MNTHREAD_NOT_SUPPORTED);
 * MN_ESP2MNTHREAD_ERROR_END(_error);
 * @endcode
 */
#define MN_ESP2MNTHREAD_ERROR_BEGIN(_error)  			switch(_error) {
/**
 * the helper macro to switch ESP_ERRORS to MNTHREAD_ERROR errors
 * @param _error The holder of the error value and holder of the MNTHREAD_ERROR code
 * @param esp The esp value to find
 * @param mnt The MNTHREAD_ERROR to set
 */
#define MN_ESP2MNTHREAD_ERROR_VAL(_error, esp, mnt) 	case (esp): _error = (mnt); break;
/**
 * End the helper macro to switch ESP_ERRORS to MNTHREAD_ERROR errors
 * @param _error The holder of the error value - set to ERR_MNTHREAD_UNKN
 */
#define MN_ESP2MNTHREAD_ERROR_END(_error) 				default:  _error = (ERR_MNTHREAD_UNKN); break; }

#define NO_ERROR                        	0x0000 	/*!< No Error */
#define ERR_MNTHREAD_TIMEOUT                0x0001	/*!< Timed out */
#define ERR_MNTHREAD_NULL                   0x0002	/*!< The given param was null or error in malloc */
#define ERR_MNTHREAD_NOT_SUPPORTED          0x0005	/*!< The function is not supported */
#define ERR_MNTHREAD_UNKN                   0x0003	/*!< Currently not defined error */
#define ERR_MNTHREAD_INVALID_ARG			0x0004	/*!< Invalig Arguments */
#define ERR_MNTHREAD_NETIF_NOT_INIT			0x0006	/*!< ERR_MNTHREAD_NETIF_NOT_INIT */
#define ERR_MNTHREAD_OUTOFMEM               0x0007	/*!< Out of mem */

#define ERR_SYSTEM_OK                       NO_ERROR	/*!< Basis error - no error */
#define ERR_SYSTEM_LOCK                     0x0008		/*!< Basis error - error can not lock */
#define ERR_SYSTEM_UNLOCK                   0x0009 		/*!< Basis error - error can not unlock */
#define ERR_SYSTEM_NO_RETURN                0x000A 		/*!< Basis error - error in called functions */

#define ERR_MUTEX_OK                    	NO_ERROR/*!< No Error in one of the mutex function */
#define ERR_MUTEX_ALREADYINIT				0x1001	/*!< The mutex allready created */
#define ERR_MUTEX_NOTINIT					0x1002	/*!< The mutex is not created, please call create first */
#define ERR_MUTEX_CANTCREATEMUTEX			0x1003	/*!< The mutex can't create */
#define ERR_MUTEX_LOCK                  	0x1004	/*!< It timed out on lock the mutex - mutex are not locked */
#define ERR_MUTEX_UNLOCK                	0x1005	/*!< Error to unlock the mutex, call lock first */

#define ERR_SPINLOCK_OK                 	NO_ERROR/*!< No Error in one of the spinlock or semaphore function */
#define ERR_SPINLOCK_ALREADYINIT			0x2001	/*!< The spinlock or semaphore allready created */
#define ERR_SPINLOCK_NOTINIT				0x2002	/*!< The spinlock or semaphore is not created, please call create first */
#define ERR_SPINLOCK_CANTCREATESPINLOCK		0x2003 	/*!< The spinlock or semaphore can't create */
#define ERR_SPINLOCK_LOCK               	0x2004	/*!< It timed out on lock the spinlock or semaphore - not locked */
#define ERR_SPINLOCK_UNLOCK             	0x2005	/*!< Error to unlock the spinlock or semaphore, call lock first */
#define ERR_SPINLOCK_BAD_INITIALCOUNT   	0x2006	/*!< Count is bigger are the max count - spinlock not created */

// --------------------------------

/**
 * No Error in one of the Thread function
 */
#define ERR_TASK_OK                   		NO_ERROR
/**
 * The Lock objects are maked error
 */
#define ERR_TASK_CANTINITMUTEX		    	0x3001
/**
 * The thread (i.e. task) can not start unknown error
 */
#define ERR_TASK_CANTSTARTTHREAD			0x3002
/**
 * The threah function is null
 */
#define ERR_TASK_THREADFUNCNOTSET			0x3003
/**
 * The thread is not running
 */
#define ERR_TASK_NOTRUNNING			    	0x3004
/**
 * The thread can not start, becourse the thread is allready started
 */
#define ERR_TASK_ALREADYRUNNING		    	0x3005
/**
 * Call task function wait and join from this task - this will not work!
 */
#define ERR_TASK_CALLFROMSELFTASK			0x3006
/**
 * The Lock objects are maked error
 */
#define ERR_TASK_CANTCREATEEVENTGROUP		0x3007

// --------------------------------

/**
 * No Error in one of the Tasklet function
 */
#define ERR_COROUTINE_OK                   NO_ERROR
/**
 * Can not create the lock object for the tasklet, tasklet not created
 */
#define ERR_COROUTINE_CANTINITLOCKT  		0x4001
/**
 * Tasklet can not created
 */
#define ERR_COROUTINE_CANSHEDULE   		    0x4002

// --------------------------------

/**
 * No Error in one of the Queue function
 */
#define ERR_QUEUE_OK                    	NO_ERROR
/**
 * The queue is allready Created
 */
#define ERR_QUEUE_ALREADYINIT				0x5001
/**
 * The queue can't created
 */
#define ERR_QUEUE_CANTCREATE				0x5002
/**
 * The queue is not created
 */
#define ERR_QUEUE_NOTCREATED            	0x5003
/**
 * The item can not enqueued in the queue
 */
#define ERR_QUEUE_ADD                   	0x5004
/**
 * The item can not remove from the queue
 */
#define ERR_QUEUE_REMOVE                	0x5005
/**
 * No items in the queue
 */
#define ERR_QUEUE_PEEK                  	0x5006
/**
 * Can not create the lock object for the blocking queue
 */
#define ERR_QUEUE_CANTCREATE_LOCK       	0x5007

/**
 * No Error in one of the Timer function
 */
#define ERR_TIMER_OK                    	NO_ERROR
/**
 * The timer is allready Created
 */
#define ERR_TIMER_ALREADYINIT				0x6001
/**
 * The timer can't created
 */
#define ERR_TIMER_CANTCREATE				0x6002
/**
 * The timer is not created
 */
#define ERR_TIMER_NOTCREATED            	0x6003
/**
 * The timer can not start
 */
#define ERR_TIMER_AKTIVATE              	0x6004
/**
 * The timer can not stop
 */
#define ERR_TIMER_INAKTIVATE            	0x6005
/**
 * The timer can not reset
 */
#define ERR_TIMER_RESET                 	0x6006

#define ERR_WORKQUEUE_OK                    NO_ERROR	/*!< No Error in one of the workqueue function */
#define ERR_WORKQUEUE_ALREADYINIT			0x7001		/*!< The workqueue is allready Created */
#define ERR_WORKQUEUE_CANTCREATE			0x7002		/*!< The workqueue can't created */
#define ERR_WORKQUEUE_WARNING               0x7003		/*!< Warning */
#define ERR_WORKQUEUE_CANTINITMUTEX         0x7004		/*!< The mutex can not init */
#define ERR_WORKQUEUE_ADD                   0x7005		/*!< The item can not add to the workqueue */

#define ERR_MEMPOOL_OK                    	NO_ERROR 	/*!< No error*/
#define ERR_MEMPOOL_BADALIGNMENT          	0x8003 		/*!< The given ligent im mempool are bad */
#define ERR_MEMPOOL_CREATE                	0x8004 		/*!< The mempool can not create */
#define ERR_MEMPOOL_MIN                   	0x8005 		/*!< Reserve */

#define ERR_TICKHOOK_OK                   	NO_ERROR	/*!< No Error in one of the tickhook function */
#define ERR_TICKHOOK_ADD                  	0x9001 		/*!< Error to add a new tickhook*/
#define ERR_TICKHOOK_ENTRY_NULL          	0x900A 		/*!< The entry is null */

#define ERR_MN_WIFI_OK          		  	NO_ERROR
#define ERR_MN_WIFI_NOT_INIT    			0xA001   /*!< WiFi driver was not installed by esp_wifi_init */
#define ERR_MN_WIFI_NOT_STARTED 			0xA002   /*!< WiFi driver was not started by esp_wifi_start */
#define ERR_MN_WIFI_NOT_STOPPED 			0xA003   /*!< WiFi driver was not stopped by esp_wifi_stop */
#define ERR_MN_WIFI_IF          			0xA004   /*!< WiFi interface error */
#define ERR_MN_WIFI_MODE        			0xA005   /*!< WiFi mode error */
#define ERR_MN_WIFI_STATE       			0xA006   /*!< WiFi internal state error */
#define ERR_MN_WIFI_CONN        			0xA007   /*!< WiFi internal control block of station or soft-AP error */
#define ERR_MN_WIFI_NVS         			0xA008   /*!< WiFi internal NVS module error */
#define ERR_MN_WIFI_MAC         			0xA009   /*!< MAC address is invalid */
#define ERR_MN_WIFI_SSID        			0xA00A   /*!< SSID is invalid */
#define ERR_MN_WIFI_PASSWORD    			0xA00B   /*!< Password is invalid */
#define ERR_MN_WIFI_TIMEOUT     			0xA00C   /*!< Timeout error */
#define ERR_MN_WIFI_WAKE_FAIL   			0xA00D   /*!< WiFi is in sleep state(RF closed) and wakeup fail */
#define ERR_MN_WIFI_WOULD_BLOCK 			0xA00E   /*!< The caller would block */
#define ERR_MN_WIFI_NOT_CONNECT 			0xA00F   /*!< Station still in disconnect status */
#define ERR_MN_WIFI_CREATE_NETIF			0xA011
#define ERR_MN_WIFI_REGISTER_EVENT			0xA012
#define ERR_MN_WIFI_SET_MODE				0xA013

#define ERR_MN_WIFI_POST        			0xA022   /*!< Failed to post the event to WiFi task */
#define ERR_MN_WIFI_INIT_STATE  			0xA023   /*!< Invalid WiFi state when init/deinit is called */
#define ERR_MN_WIFI_STOP_STATE  			0xA024   /*!< Returned when WiFi is stopping */


#define ERR_MN_USER1_BASE					0xD500
#define ERR_MN_USER2_BASE					0xE500
#define ERR_MN_USER3_BASE					0xFF00

#endif
