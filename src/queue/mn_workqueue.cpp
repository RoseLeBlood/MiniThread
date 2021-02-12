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
#include "mn_task.hpp"



#include "queue/mn_workqueue.hpp"

namespace mn {
    namespace queue {
        //-----------------------------------
        //  constructor
        //-----------------------------------
        basic_work_queue::basic_work_queue(basic_task::priority uiPriority,
                            uint16_t usStackDepth,
                            uint8_t uiMaxWorkItems) :
            m_pWorkItemQueue(NULL),
            m_ThreadStatus(),
            m_ThreadJob(),
            m_uiPriority(uiPriority),
            m_usStackDepth(usStackDepth),
            m_uiMaxWorkItems(uiMaxWorkItems),
            m_uiNumWorks(0),
            m_uiErrorsNumWorks(0),
            m_bRunning(false) { 

            m_pWorkItemQueue = new queue_t(uiMaxWorkItems, sizeof(work_queue_item_t *));
        }

        //-----------------------------------
        //  deconstructor
        //-----------------------------------
        basic_work_queue::~basic_work_queue() {
            destroy();
        }

        //-----------------------------------
        //  create
        //-----------------------------------
        int basic_work_queue::create(int iCore) {
            m_ThreadJob.lock();

            int ret = create_engine(iCore);

            if( ret != NO_ERROR) {
                m_ThreadJob.unlock();
                return ret;
            }

            m_ThreadJob.unlock();
            
            
            return ERR_WORKQUEUE_OK;
        }

        //-----------------------------------
        //  destroy
        //-----------------------------------
        void basic_work_queue::destroy() {
            m_ThreadStatus.lock();
            m_bRunning = false;
            m_ThreadStatus.unlock();

            destroy_engine();
        }

        //-----------------------------------
        //  queue
        //-----------------------------------
        int basic_work_queue::queue(work_queue_item_t *work, unsigned int timeout) {
            automutx_t lock(m_ThreadJob);

            int ret = m_pWorkItemQueue->enqueue(work, timeout);

            return ret == 0 ? ERR_WORKQUEUE_OK : ERR_WORKQUEUE_ADD;
        }

        //-----------------------------------
        //  get_next_item
        //-----------------------------------
        work_queue_item* basic_work_queue::get_next_item(unsigned int timeout) {
            automutx_t lock(m_ThreadJob);

            work_queue_item_t* job = 0;

            if(m_pWorkItemQueue->is_empty()) return NULL;
            
            m_pWorkItemQueue->dequeue(&job, timeout);

            return job;
        }

        //-----------------------------------
        //  get_num_items_worked
        //-----------------------------------
        uint8_t basic_work_queue::get_num_items_worked() { 
            automutx_t lock(m_ThreadStatus);

            return m_uiNumWorks; 
        }

        //-----------------------------------
        //  get_num_items_error
        //-----------------------------------
        uint8_t basic_work_queue::get_num_items_error() { 
            automutx_t lock(m_ThreadStatus);

            return m_uiErrorsNumWorks;
        }

        //-----------------------------------
        //  is_ready
        //-----------------------------------
        bool basic_work_queue::is_ready() {
            automutx_t lock(m_ThreadStatus);

            uint8_t ready = m_uiNumWorks + m_uiErrorsNumWorks;

            return (ready == m_uiMaxWorkItems) && m_pWorkItemQueue->is_empty();
        }
    }
}