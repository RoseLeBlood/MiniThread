/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "mn_tasklet.hpp"

//-----------------------------------
//  create
//-----------------------------------
int basic_tasklet::create(uint32_t parameter, TickType_t timeout) {
    BaseType_t success;

    if(m_ssLock.create() != NO_ERROR)
        return ERR_COROUTINE_CANTINITLOCKT;

    m_ssLock.lock(); 

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xTimerPendFunctionCallFromISR( runcorostub,
                                        this, parameter, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
                    
    } else {
        success = xTimerPendFunctionCall(runcorostub,
                                    this, parameter, timeout);
    }

    if (success == pdPASS)
        return ERR_COROUTINE_OK;
    
    m_ssLock.unlock();
    return ERR_COROUTINE_CANTSTART;
}

//-----------------------------------
//  destroy
//-----------------------------------
int basic_tasklet::destroy() {
    m_ssLock.lock( portMAX_DELAY );
    m_ssLock.destroy();

    return ERR_COROUTINE_OK;
}

//-----------------------------------
//  runcorostub
//-----------------------------------
void basic_tasklet::runcorostub(void* xHandle, uint32_t parameter) {
    basic_tasklet *tasklet = static_cast<basic_tasklet *>(xHandle);

    for(;;) {
        if (!tasklet->on_coroutine(parameter) )
            break;
    } 

    tasklet->m_ssLock.unlock();
}