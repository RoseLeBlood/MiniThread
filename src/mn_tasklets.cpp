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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "mn_tasklets.hpp"


int basic_tasklet::create(uint32_t parameter, TickType_t timeout) {
    BaseType_t success;

    if(m_ssLock.create() != NO_ERROR)
        return ERR_TASKLET_CANTINITLOCKT;

    m_ssLock.lock(); 

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xTimerPendFunctionCallFromISR( runtaskletstub,
                                        this, parameter, &xHigherPriorityTaskWoken);
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
                    
    } else {
        success = xTimerPendFunctionCall(runtaskletstub,
                                    this, parameter, timeout);
    }

    if (success == pdPASS)
        return ERR_TASKLET_OK;
    
    m_ssLock.unlock();
    return ERR_TASKLET_CANTSTART;
}
int basic_tasklet::destroy() {
    m_ssLock.lock( portMAX_DELAY );
    m_ssLock.destroy();

    return ERR_TASKLET_OK;
}
void basic_tasklet::runtaskletstub(void* ref, uint32_t parameter) {
    basic_tasklet *tasklet = static_cast<basic_tasklet *>(ref);

    tasklet->on_tasklet(parameter);

    tasklet->m_ssLock.unlock();
}