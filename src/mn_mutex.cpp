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
#include "mn_mutex.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>


#include "esp_attr.h"

basic_mutex::basic_mutex() : m_pmutex(NULL) { }

int basic_mutex::create() { 
  if (m_pmutex != NULL)
    return ERR_MUTEX_ALREADYINIT;

#if MN_THREAD_CONFIG_MUTEX_CLASS == MN_THREAD_CONFIG_BINARY_SEMAPHORE 
  m_pmutex = xSemaphoreCreateBinary();
#elif MN_THREAD_CONFIG_MUTEX_CLASS == MN_THREAD_CONFIG_MUTEX 
  m_pmutex = xSemaphoreCreateMutex();
#endif

  if (m_pmutex) {
    unlock();
    return ERR_MUTEX_OK;
  }

  return ERR_MUTEX_CANTCREATEMUTEX;
}
int basic_mutex::destroy() {
  if (m_pmutex == NULL)
    return ERR_MUTEX_NOTINIT;

  vSemaphoreDelete(m_pmutex);

  return ERR_MUTEX_OK;
}
int basic_mutex::lock(unsigned int timeout) {
  BaseType_t success;

  if (m_pmutex == NULL)
    return ERR_MUTEX_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreTakeFromISR( m_pmutex, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
    success = xSemaphoreTake(m_pmutex, timeout);
   }

  return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_LOCK;
}
int basic_mutex::unlock() {
  BaseType_t success;

  if (m_pmutex == NULL)
    return ERR_MUTEX_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreGiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
			success = xSemaphoreGive(m_pmutex);
  }
  return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_UNLOCK;
}
bool basic_mutex::try_lock() {
  return (lock(0) == ERR_MUTEX_OK);
}
