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
#include "mn_config.hpp"
#include "queue/mn_workqueue_multi.hpp"

basic_work_queue_multi::basic_work_queue_multi( unsigned int uiPriority,
                uint16_t usStackDepth, uint8_t uiMaxWorkItems, uint8_t uiMaxWorkers) 

    : basic_work_queue(uiPriority, usStackDepth, uiMaxWorkItems) {

    m_uiMaxWorkers = uiMaxWorkers;
}
int basic_work_queue_multi::on_create(int iCore) {
    automutx_t lock(m_ThreadStatus);

    if(m_bRunning) { 
        return ERR_WORKQUEUE_ALREADYINIT;
    }
    
    m_bRunning = true;

    char name[32];

    for (int i = 0; i < m_uiMaxWorkers; i++) {
        sprintf(name, "work_multi_%d", i);

        work_queue_thread *pWorker = new work_queue_thread(name,
                                                            m_uiPriority, 
                                                            m_usStackDepth, 
                                                            this);
        if(pWorker) { 
            pWorker->create(iCore);
            m_Workers.push_back(pWorker);
        } else {
            return get_num_worker() != 0 ? ERR_WORKQUEUE_CANTCREATE : ERR_WORKQUEUE_WARNING;
        }
    }
    return ERR_WORKQUEUE_OK;
}

void basic_work_queue_multi::on_destroy() {
    for(work_queue_thread *pWorker : m_Workers) {
        pWorker->kill();
    }
    m_Workers.clear();
    
}

uint8_t basic_work_queue_multi::get_num_worker() const  {
   return m_Workers.size();
}
uint8_t basic_work_queue_multi::get_num_max_worker() const   {
    return m_uiMaxWorkers;
}
std::vector<work_queue_thread*>& basic_work_queue_multi::workers() {
    return m_Workers;
}