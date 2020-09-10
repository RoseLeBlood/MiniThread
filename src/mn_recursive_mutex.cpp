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
#include "mn_config.hpp"

#if (configUSE_RECURSIVE_MUTEXES == 1)

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "esp_attr.h"


#include "mn_recursive_mutex.hpp"


recursive_mutex::recursive_mutex() : basic_mutex() {

}


int recursive_mutex::create()  {
    if (m_pmutex != NULL)
        return ERR_MUTEX_ALREADYINIT;
  
    m_pmutex = xSemaphoreCreateRecursiveMutex();

    if (m_pmutex) {

        unlock();
        return ERR_MUTEX_OK;
    } else {
        return ERR_MUTEX_CANTCREATEMUTEX;
    }
}
int recursive_mutex::lock(unsigned int timeout) {
    BaseType_t success;

    if (m_pmutex == NULL)
        return ERR_MUTEX_NOTINIT;

    success = xSemaphoreTakeRecursive(m_pmutex, timeout);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_LOCK;
}
int recursive_mutex::unlock() {
    BaseType_t success;

    if (m_pmutex == NULL)
        return ERR_MUTEX_NOTINIT;
    success = xSemaphoreGiveRecursive(m_pmutex);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_UNLOCK;
}


#endif