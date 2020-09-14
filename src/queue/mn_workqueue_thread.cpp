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



basic_work_queue::work_queue_thread::work_queue_thread(char const* strName, 
                                                       unsigned int uiPriority,
                                                       unsigned short  usStackDepth, 
                                                       basic_work_queue *parent)
    : basic_thread(strName, uiPriority, usStackDepth), m_parentWorkQueue(parent) { }



basic_work_queue::work_queue_thread::~work_queue_thread() { }


void* basic_work_queue::work_queue_thread::on_thread() {
    basic_thread::on_thread();

    work_queue_item *work_item;

    m_uiNumWorks = 0;

    while (true) {
        m_parentWorkQueue->m_pRunningSem->lock();
        m_parentWorkQueue->m_pWorkItemQueue->dequeue(&work_item);
        m_parentWorkQueue->m_pRunningSem->unlock();

        if (work_item != NULL) {
                work_item->on_work();
                work_item->destroy();

            if (work_item->can_delete()) {
                delete work_item; work_item = NULL;
            }
            m_uiNumWorks++;
        } else {
            break;
        }
    }
    m_parentWorkQueue->m_pRunningSem->lock();
    m_parentWorkQueue->m_pComplete->unlock();
    m_parentWorkQueue->m_pRunningSem->unlock();

    return &m_uiNumWorks;
}
