/** This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
**/
#include "mn_config.hpp"
#include "mn_thread.hpp"
#include "queue/mn_queue.hpp"
#include "queue/mn_workqueue.hpp"

basic_work_queue::basic_work_queue(const char * strName,
                unsigned int uiPriority,
                uint16_t usStackDepth,
                unsigned int uiMaxWorkItems) : m_running(false) {

    m_pWorkItemQueue = new queue_t(uiMaxWorkItems, sizeof(work_queue_item_t *));
    m_pWorker = new work_queue_thread(strName, uiPriority, usStackDepth, this);
}
int basic_work_queue::create(int iCore) {
    if(!m_bMutexInit) {
        m_pComplete = new basic_binary_semaphore();
        m_pRunningSem = new basic_binary_semaphore();

        if(m_pRunningSem->create() != ERR_MUTEX_OK)
            return ERR_THREAD_CANTINITMUTEX;

        if(m_pComplete->create() != ERR_MUTEX_OK)
            return ERR_THREAD_CANTINITMUTEX;

        m_bMutexInit = true;
    }

    m_pRunningSem->lock();

    if(!m_running) {
        m_pComplete->lock();
        m_running = (m_pWorker->create(iCore) == ERR_THREAD_OK);
    }
    
    m_pRunningSem->unlock();
    
    return 0;
}
void basic_work_queue::destroy() {
    m_pWorker->kill();

    if(m_pComplete->try_lock() && m_bMutexInit) {
        m_pRunningSem->destroy();

        m_pComplete->unlock();
        m_pComplete->destroy();

        m_bMutexInit = false;
        m_running = false; 
    }
}
int basic_work_queue::queue(work_queue_item_t *work) {
    autobinsemp_t autolock(*m_pRunningSem);

    int ret = work->create();

    if(ret == 0)
        return m_pWorkItemQueue->enqueue(&work);
    else 
        return ret;
}
