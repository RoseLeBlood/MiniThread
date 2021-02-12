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
#include "mn_basic_semaphore.hpp"
#include "mn_error.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

#include "esp_attr.h"

#include "mn_micros.hpp"

namespace mn {
  //-----------------------------------
  //  construtor
  //-----------------------------------
  basic_semaphore::basic_semaphore() 
    : m_pSpinlock(NULL) { }

  basic_semaphore::basic_semaphore(const basic_semaphore& other)
    : m_pSpinlock(other.m_pSpinlock) { 

    if(!is_initialized()) {
      THROW_LOCK_EXP(ERR_MUTEX_NOTINIT);
    }
  }

  //-----------------------------------
  //  lock
  //-----------------------------------
  int basic_semaphore::lock(unsigned int timeout) {
    BaseType_t success;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        success = xSemaphoreTakeFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
          _frxt_setup_switch();
    } else {
      success = xSemaphoreTake(m_pSpinlock, timeout);
    }
    if(success != pdTRUE) {
      return ERR_SPINLOCK_LOCK;
    }
    return ERR_SPINLOCK_OK;
  }

  //-----------------------------------
  //  unlock
  //-----------------------------------
  int basic_semaphore::unlock() {
    BaseType_t success;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        success = xSemaphoreGiveFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
          _frxt_setup_switch();
    } else {
        success = xSemaphoreGive(m_pSpinlock);
    }
    if(success != pdTRUE) {
      return ERR_SPINLOCK_UNLOCK;
    }
    
    return ERR_SPINLOCK_OK;
  }

  //-----------------------------------
  //  time_lock(const struct timespec *abs_time)
  //-----------------------------------
  int basic_semaphore::time_lock(const struct timespec *timeout) {
    struct timespec currtime;
    clock_gettime(CLOCK_REALTIME, &currtime);

    TickType_t _time = ((timeout->tv_sec - currtime.tv_sec)*1000 +
                      (timeout->tv_nsec - currtime.tv_nsec)/1000000)/portTICK_PERIOD_MS;

    return lock(_time);
  }
}
