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
#include "mn_semaphore.hpp"
#include "mn_error.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

#include "esp_attr.h"


basic_semaphore::basic_semaphore() : m_pSpinlock(NULL) {

}

int basic_semaphore::lock(unsigned int timeout) {
  BaseType_t success;

  if (m_pSpinlock == NULL)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreTakeFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
    success = xSemaphoreTake(m_pSpinlock, timeout);
   }
   return success == pdTRUE ? ERR_SPINLOCK_OK : ERR_SPINLOCK_LOCK;
}
int basic_semaphore::unlock() {
  BaseType_t success;

  if (m_pSpinlock == NULL)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreGiveFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
			success = xSemaphoreGive(m_pSpinlock);
  }
  return success == pdTRUE ? ERR_SPINLOCK_OK : ERR_SPINLOCK_UNLOCK;
}
bool basic_semaphore::try_lock() {
  return (lock( 0 ) == ERR_SPINLOCK_OK);
}

bool basic_semaphore::is_initialized() const { 
  return m_pSpinlock != NULL;
}

//-------------------------------

basic_counting_semaphore::basic_counting_semaphore(int count, int maxcount) 
  : basic_semaphore(), m_uiCount(count), m_uiMaxCount(maxcount) {
    
}

int basic_counting_semaphore::destroy() {
  vSemaphoreDelete(m_pSpinlock);
  m_pSpinlock = NULL;

  return ERR_SPINLOCK_OK;
}

int basic_counting_semaphore::create() {
  if (m_pSpinlock != NULL)
    return ERR_SPINLOCK_ALREADYINIT;

  if (m_uiMaxCount < m_uiCount)
    return ERR_SPINLOCK_BAD_INITIALCOUNT;

  if (m_uiMaxCount == 0) {
    m_pSpinlock = xSemaphoreCreateCounting(m_uiMaxCount, m_uiCount);



    if (m_pSpinlock) {
      unlock();
      return ERR_SPINLOCK_OK;
    }
  }
  return ERR_SPINLOCK_CANTCREATESPINLOCK;
}

int basic_counting_semaphore::get_count() const {
   return uxQueueMessagesWaiting(m_pSpinlock);
}
//------------------



basic_binary_semaphore::basic_binary_semaphore() : basic_semaphore() { }


int basic_binary_semaphore::create() {
  if (m_pSpinlock != NULL)
    return ERR_SPINLOCK_ALREADYINIT;

  m_pSpinlock = xSemaphoreCreateBinary();


  if (m_pSpinlock) {
    unlock();
    return ERR_SPINLOCK_OK;
  }
  return ERR_SPINLOCK_CANTCREATESPINLOCK;
}

int basic_binary_semaphore::destroy() {
  vSemaphoreDelete(m_pSpinlock);
  m_pSpinlock = NULL;

  return ERR_SPINLOCK_OK;
}