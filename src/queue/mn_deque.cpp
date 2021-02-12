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
#include "freertos/queue.h"

#include "queue/mn_deque.hpp"
#include "mn_error.hpp"

namespace mn {
    namespace queue {
        //-----------------------------------
        //  enqueue_front
        //-----------------------------------
        int basic_deque::enqueue_front(void* item, unsigned int timeout) {
            if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

            BaseType_t success;

            if (xPortInIsrContext()) {
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;

                success = xQueueSendToFrontFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

                if(xHigherPriorityTaskWoken)
                    _frxt_setup_switch();
            } else {
                success = xQueueSendToFront(m_pHandle, item, timeout);
            }
            return success == pdTRUE ? ERR_QUEUE_OK : ERR_QUEUE_ADD;
        }
    }
}