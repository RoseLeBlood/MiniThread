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

#include "queue/mn_binaryqueue.hpp"
#include "mn_error.hpp"

namespace mn {
    namespace queue {
        //-----------------------------------
        //  enqueue
        //-----------------------------------
        int basic_binaryqueue::enqueue(void *item, unsigned int timeout) {
            (void)timeout;

            if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

            if (xPortInIsrContext()) {
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;

                (void)xQueueOverwriteFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

                if(xHigherPriorityTaskWoken)
                    _frxt_setup_switch();
            } else {
                (void)xQueueOverwrite(m_pHandle, item);
            }
            return ERR_QUEUE_OK;
        }
    }
}