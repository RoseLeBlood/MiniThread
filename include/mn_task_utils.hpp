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
#ifndef MINLIB_ESP32_THREAD_UTILS_
#define MINLIB_ESP32_THREAD_UTILS_


#include "mn_autolock.hpp"

namespace mn {
  class basic_task;

  /**
   * FreeRTOS wrapper for notify and broadcast a task
   * \ingroup task
   */ 
  class task_utils {
  public:
    /** Actions that can be performed when task_utils::notify() is called. */
    enum action {
      no_action = eNoAction,				                /*!< Notify the task without updating its notify value. */
      set_bits = eSetBits,					                /*!< Set bits in the task's notification value. */
      increment = eIncrement,			                  /*!< Increment the task's notification value. */
      set_value_overwrite = eSetValueWithOverwrite, /*!< Set the task's notification value to a specific value even if the previous value has not yet been read by the task. */
      set_value	= eSetValueWithoutOverwrite         /*!< Set the task's notification value if the previous value has been read by the task. */
    };

    /**
     * Send task notification.
     * 
     * @param pTaskToNotify The task being notified.
     *
     * @param ulValue Data that can be sent with the notification. How the data is
     * used depends on the value of the eAction parameter.
     *
     * @param eAction Specifies how the notification updates the task's notification
     * value, if at all.
     * 
     * @return true If task notify and false If not
     */ 
    static bool notify(basic_task* pTaskToNotify, uint32_t ulValue, task_utils::action eAction);

    /**
     * Recive a task notification.
     * 
     * @param bClearCountOnExit if is false then the task's
     * notification value is decremented when the function exits.
     * If true then the task's notification value is cleared to zero when the
     * function exits.
     * 
     * @param xTicksToWait The maximum amount of time that the task should wait in
     * the Blocked state for the task's notification value to be greater than zero,
     * should the count not already be greater than zero when
     * notify_take() was called. The task will not consume any processing
     * time while it is in the Blocked state. This is specified in kernel ticks,
     * the function ms_to_ticks( value_in_ms ) can be used to convert a time
     * specified in milliseconds to a time specified in ticks.
     * 
     * 
     * @return The task's notification count before it is either cleared to zero or
     * decremented 
     */ 
    static uint32_t notify_take(bool bClearCountOnExit, TickType_t xTicksToWait); 

    /**
     * Send a task notification.
     *
     * 
     * @param pTaskToNotify The task being notified.
     * 
     * @return true if give and else if not 
     */
    static bool notify_give(basic_task* pTaskToNotify);

    /**
     * Wait for task notification
     * 
     * A task can use notify_wait() to block
     * to wait for its notification value to have a non-zero value.
     * The task does not consume any CPU time while it is in the Blocked state.
     *
     * @param ulBitsToClearOnEntry Bits that are set in ulBitsToClearOnEntry value
     * will be cleared in the calling task's notification value before the task
     * checks to see if any notifications are pending, and optionally blocks if no
     * notifications are pending.
     * 
     * @param ulBitsToClearOnExit If a notification is pending or received before
     * the calling task exits the notify_wait() function then the task's
     * notification value is passed out using the pulNotificationValue parameter.
     *
     * @param pulNotificationValue Used to pass the task's notification value out
     * of the function.  Note the value passed out will not be effected by the
     * clearing of any bits caused by ulBitsToClearOnExit being non-zero.
     *
     * @param xTicksToWait The maximum amount of time that the task should wait in
     * the Blocked state for a notification to be received, should a notification
     * not already be pending when notify_wait() was called.
     *
     * @return If a notification was received then true is returned. 
     * Otherwise false is returned.
     */ 
    static bool notify_wait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit,
                            uint32_t *pulNotificationValue, TickType_t xTicksToWait );

    #if( configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0 )
    /**
     * Set local storage pointer specific to the given task.
     *
     * The kernel does not use the pointers itself, so the application writer
     * can use the pointers for any purpose they wish.
     *
     * @param task  Task to set thread local storage pointer for
     * @param index The index of the pointer to set, from 0 to
     *               configNUM_THREAD_LOCAL_STORAGE_POINTERS - 1.
     * @param value  Pointer value to set.
     */
    static void set_storage_pointer(basic_task* task, unsigned short index, void* value);
    /**
     * Get local storage pointer specific to the given task.
     * 
     * The kernel does not use the pointers itself, so the application writer
     * can use the pointers for any purpose they wish.
     *
     * @param task  Task to set thread local storage pointer for
     * @param index The index of the pointer to set, from 0 to
     *               configNUM_THREAD_LOCAL_STORAGE_POINTERS - 1.
     * @return  Pointer value
     */
    static void* get_storage_pointer(basic_task* task, unsigned short index); 
    #endif                       
  };

  using task_utils_t = task_utils;
}
#endif