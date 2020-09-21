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

#include "queue/mn_blocking_queue.hpp"
#include "mn_error.hpp"

basic_blocking_queue::basic_blocking_queue(unsigned int maxItems, unsigned int itemSize)
    : basic_queue(maxItems, itemSize)  { }

int basic_blocking_queue::create() {
    if(m_semp.create() != ERR_SPINLOCK_OK)
        return ERR_QUEUE_CANTCREATE_LOCK;
    
    //autobinsemp_t autolock(m_semp);
    return basic_queue::create();
}
int basic_blocking_queue::destroy() {
    if(m_semp.is_initialized() ) {
        m_semp.lock();
        if(basic_queue::destroy() == ERR_QUEUE_OK) {
            m_semp.unlock();
            if(m_semp.is_initialized() ) m_semp.destroy();
        }
    }
    return ERR_QUEUE_NOTCREATED;
}
int basic_blocking_queue::enqueue(void *item, unsigned int timeout) {
    autobinsemp_t autolock(m_semp);

    return basic_queue::enqueue(item, timeout);
}

int basic_blocking_queue::peek(void *item, unsigned int timeout) {
    autobinsemp_t autolock(m_semp);

    return basic_queue::peek(item, timeout);
}

int basic_blocking_queue::dequeue(void *item, unsigned int timeout) {
    autobinsemp_t autolock(m_semp);

    return basic_queue::dequeue(item, timeout);
}