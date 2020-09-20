/*
This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
Copyright (c) 2020 Amber-Sophia Schroeck
  
The Mini Thread Library is free software; you can redistribute it and/or modify  
it under the terms of the GNU Lesser General Public License as published by  
the Free Software Foundation, version 3, or (at your option) any later version.
 
The Mini Thread Library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public
License along with the Mini Thread  Library; if not, see
<https://www.gnu.org/licenses/>.  
*/
#include "mn_thread_pool.hpp"

basic_task_pool::basic_task_pool(unsigned int uiPriority,
                           unsigned short  usStackDepth,
                           unsigned int uiMaxJobs, 
                           unsigned int uiMaxWorkers) {

    m_Jobs = new basic_blocking_queue(uiMaxJobs, sizeof(basic_task_pool_job*));
    m_ThreadStatus = new mutex_t();
    m_ThreadJob = new mutex_t();

    m_bRunning = false;

    m_uiMaxWorkers = uiMaxWorkers;
    m_uiPriority = uiPriority;
    m_usStackDepth = usStackDepth;
}
int basic_task_pool::queue(basic_task_pool_job* job, unsigned int timeout) {
    m_ThreadJob->lock();
    int ret = m_Jobs->enqueue(job, timeout);
    m_ThreadJob->unlock();

    return ret == 0 ? 0 : -1;
}
int basic_task_pool::create(int iCore) {
    m_ThreadStatus->create();
    m_ThreadJob->create();

    m_ThreadStatus->lock();
        m_bRunning = true;
    m_ThreadStatus->unlock();

    for (int i = 0; i < m_uiMaxWorkers; i++) {
        basic_task_pool_worker *pWorker = new basic_task_pool_worker( 
                                                m_uiPriority, 
                                                m_usStackDepth, 
                                                *this);
        if(pWorker) { 
            pWorker->create(iCore);
            m_Workers.push_back(pWorker);
        }
    }

    return 0;
}
int basic_task_pool::stop() {
    m_ThreadStatus->lock();
        m_bRunning = false;
    m_ThreadStatus->unlock();

    return 0;
}
basic_task_pool_job* basic_task_pool::get_next_job() {
    automutx_t lock(*m_ThreadJob);

    basic_task_pool_job* job = 0;
    m_Jobs->dequeue(&job);

    return job;
}


void* basic_task_pool::basic_task_pool_worker::on_thread() {

    basic_task_pool_job* job = 0;
    m_uiNumWorks = 0;

    while (m_pParent.running()) {
        job = m_pParent.get_next_job();

        if(job) {
            job->on_work();
            m_uiNumWorks++;
        }
	}
    return &m_uiNumWorks;
}