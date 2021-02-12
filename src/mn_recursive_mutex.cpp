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

#if MN_THREAD_CONFIG_RECURSIVE_MUTEX == MN_THREAD_CONFIG_YES

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "esp_attr.h"


#include "mn_recursive_mutex.hpp"

namespace mn {
    //-----------------------------------
    //  construtor
    //-----------------------------------
    recursive_mutex::recursive_mutex() 
        : basic_mutex() {

        #if( configSUPPORT_STATIC_ALLOCATION == 1 )
            m_pSpinlock = xSemaphoreCreateRecursiveMutexStatic(&m_SemaphoreBasicBuffer);
        #else
            m_pSpinlock = xSemaphoreCreateRecursiveMutex();
        #endif

        if (m_pSpinlock) {
            unlock();
        } else {
            THROW_LOCK_EXP(ERR_MUTEX_CANTCREATEMUTEX);
        }
    }
    //-----------------------------------
    //  lock
    //-----------------------------------
    int recursive_mutex::lock(unsigned int timeout) {
        
        if(xSemaphoreTakeRecursive(m_pSpinlock, timeout) != pdTRUE) {
            return ERR_MUTEX_LOCK;
        } 
        return ERR_MUTEX_OK; 
    }

    //-----------------------------------
    //  unlock
    //-----------------------------------
    int recursive_mutex::unlock() {

        if(xSemaphoreGiveRecursive(m_pSpinlock) != pdTRUE) {
            return ERR_MUTEX_UNLOCK;
        } 
        return ERR_MUTEX_OK; 
    }
}

#endif