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
#include "freertos/queue.h"

#include "queue/mn_deque.hpp"
#include "mn_error.hpp"

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