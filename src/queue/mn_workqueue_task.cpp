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
#include "mn_task.hpp"
#include "queue/mn_queue.hpp"

#include "queue/mn_workqueue_task.hpp"
#include "queue/mn_workqueue.hpp"

namespace mn {
    namespace queue {
        //-----------------------------------
        //  constructor
        //-----------------------------------
        work_queue_task::work_queue_task(char const* strName, 
                                            basic_task::priority uiPriority,
                                            unsigned short  usStackDepth, 
                                            basic_work_queue* parent)

            : basic_task(strName, uiPriority, usStackDepth), m_parentWorkQueue(parent) { 

        }
        //-----------------------------------
        //  deconstructor
        //-----------------------------------
        work_queue_task::~work_queue_task() { }

        //-----------------------------------
        //  on_task
        //-----------------------------------
        void* work_queue_task::on_task() {
            basic_task::on_task();

            work_queue_item *work_item = NULL;


            while ( m_parentWorkQueue->running() ) {
                work_item = m_parentWorkQueue->get_next_item(MN_THREAD_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT);

                if (work_item == NULL) { 
                    break;
                }

                m_parentWorkQueue->m_ThreadStatus.lock();

                    if(work_item->on_work())
                        m_parentWorkQueue->m_uiNumWorks++;
                    else
                        m_parentWorkQueue->m_uiErrorsNumWorks++;

                m_parentWorkQueue->m_ThreadStatus.unlock();

                if (work_item->can_delete()) {
                    delete work_item; work_item = NULL;
                }
            }
            
            return 0;
        }
    }
}
