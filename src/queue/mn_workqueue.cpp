/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2020 Amber-Sophia Schroeck
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
#include "mn_config.hpp"
#include "mn_thread.hpp"
#include "queue/mn_queue.hpp"
#include "queue/mn_workqueue.hpp"

basic_work_queue::basic_work_queue(const char * strName,
                unsigned int uiPriority,
                uint16_t usStackDepth,
                unsigned int uiMaxWorkItems) : m_created(false) {

    m_pWorkItemQueue = new queue_t(uiMaxWorkItems, sizeof(work_queue_item_t *));
    m_pWorker = new work_queue_thread(strName, uiPriority, usStackDepth, this);
}
int basic_work_queue::create(int iCore) {
    if(!m_bMutexInit) {
        m_pComplete = new basic_binary_semaphore();
        m_pRunningMutex = new basic_binary_semaphore();

        if(m_pRunningMutex->create() != ERR_MUTEX_OK)
            return ERR_THREAD_CANTINITMUTEX;

        if(m_pComplete->create() != ERR_MUTEX_OK)
            return ERR_THREAD_CANTINITMUTEX;
    }

    m_pRunningMutex->lock();

    if(!m_created) {
        m_pComplete->lock();
        m_pWorker->create(iCore);
    }
    
    m_pRunningMutex->unlock();
    
    return 0;
}