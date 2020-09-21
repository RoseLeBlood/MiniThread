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
#include <vector>
#include "mn_config.hpp"
#include "mn_thread.hpp"



#include "queue/mn_workqueue.hpp"

basic_work_queue::basic_work_queue(unsigned int uiPriority,
                    uint16_t usStackDepth,
                    uint8_t uiMaxWorkItems) {

    m_pWorkItemQueue = new queue_t(uiMaxWorkItems, sizeof(work_queue_item_t *));
    m_uiPriority = uiPriority;
    m_usStackDepth = usStackDepth;
    m_bRunning = false;
    m_bMutexInit = false;
    m_uiNumWorks = 0;
    m_uiErrorsNumWorks = 0;
    m_uiMaxWorkItems = uiMaxWorkItems;
    
}

basic_work_queue::~basic_work_queue() {
    destroy();
}

int basic_work_queue::create(int iCore) {
    if(!m_bMutexInit) {
        if(m_ThreadStatus.create() != ERR_MUTEX_OK)
            return ERR_WORKQUEUE_CANTINITMUTEX;

        if(m_ThreadJob.create() != ERR_MUTEX_OK)
            return ERR_WORKQUEUE_CANTINITMUTEX;

        m_bMutexInit = true;
    }
    
    m_ThreadJob.lock();

    int ret = on_create(iCore);

    if( ret != NO_ERROR) {
        m_ThreadJob.unlock();
        return ret;
    }

    m_ThreadJob.unlock();
    
    
    return ERR_WORKQUEUE_OK;
}

void basic_work_queue::destroy() {
    m_ThreadStatus.lock();
    m_bRunning = false;
    m_ThreadStatus.unlock();

    on_destroy();

    if(m_bMutexInit) {
        m_ThreadStatus.destroy();
        m_ThreadJob.destroy();
        
        m_bMutexInit = false;
    }
}

int basic_work_queue::queue(work_queue_item_t *work, unsigned int timeout) {
    automutx_t lock(m_ThreadJob);

    int ret = m_pWorkItemQueue->enqueue(work, timeout);

    return ret == 0 ? ERR_WORKQUEUE_OK : ERR_WORKQUEUE_ADD;
}

work_queue_item* basic_work_queue::get_next_item(unsigned int timeout) {
    automutx_t lock(m_ThreadJob);

    work_queue_item_t* job = 0;

    if(m_pWorkItemQueue->is_empty()) return NULL;
    
    m_pWorkItemQueue->dequeue(&job, timeout);

    return job;
}

uint8_t basic_work_queue::get_num_items_worked() { 
    automutx_t lock(m_ThreadStatus);

    return m_uiNumWorks; 
}
uint8_t basic_work_queue::get_num_items_error() { 
    automutx_t lock(m_ThreadStatus);

    return m_uiErrorsNumWorks;
}
bool basic_work_queue::is_ready() {
    automutx_t lock(m_ThreadStatus);

    uint8_t ready = m_uiNumWorks + m_uiErrorsNumWorks;

    return (ready == m_uiMaxWorkItems) && m_pWorkItemQueue->is_empty();
}
