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
    if (m_pSpinlock != NULL)
        return ERR_MUTEX_ALREADYINIT;
  
    m_pSpinlock = xSemaphoreCreateRecursiveMutex();

    if (m_pSpinlock) {

        unlock();
        return ERR_MUTEX_OK;
    } else {
        return ERR_MUTEX_CANTCREATEMUTEX;
    }
}
int recursive_mutex::lock(unsigned int timeout) {
    BaseType_t success;

    if (m_pSpinlock == NULL)
        return ERR_MUTEX_NOTINIT;

    success = xSemaphoreTakeRecursive(m_pSpinlock, timeout);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_LOCK;
}
int recursive_mutex::unlock() {
    BaseType_t success;

    if (m_pSpinlock == NULL)
        return ERR_MUTEX_NOTINIT;
    success = xSemaphoreGiveRecursive(m_pSpinlock);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_UNLOCK;
}


#endif