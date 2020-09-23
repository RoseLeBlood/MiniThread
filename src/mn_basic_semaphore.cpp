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

