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
#include "queue/mn_workqueue_single.hpp"

namespace mn {
    namespace queue {
        //-----------------------------------
        //  constructor
        //-----------------------------------
        basic_work_queue_single::basic_work_queue_single( basic_task::priority uiPriority,
                                                        uint16_t usStackDepth, uint8_t uiMaxWorkItems) 

            : basic_work_queue(uiPriority, usStackDepth, uiMaxWorkItems) {

            m_pWorker = new work_queue_task("single_workqueue_thread", uiPriority, usStackDepth, this);
        }

        //-----------------------------------
        //  create_engine
        //-----------------------------------
        int basic_work_queue_single::create_engine(int iCore) {
            automutx_t lock(m_ThreadStatus);

            if(m_bRunning) { 
                return ERR_WORKQUEUE_ALREADYINIT;
            }
            m_bRunning = true;

            return (m_pWorker->start(iCore) == NO_ERROR) ? ERR_WORKQUEUE_OK : ERR_WORKQUEUE_CANTCREATE;

        }

        //-----------------------------------
        //  destroy_engine
        //-----------------------------------
        void basic_work_queue_single::destroy_engine() {
            m_pWorker->kill();
        }
    }
}

