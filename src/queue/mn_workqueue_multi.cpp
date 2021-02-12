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

namespace mn {
    namespace queue {
        //-----------------------------------
        //  constructor
        //-----------------------------------
        basic_work_queue_multi::basic_work_queue_multi( basic_task::priority uiPriority,
                        uint16_t usStackDepth, uint8_t uiMaxWorkItems, uint8_t uiMaxWorkers) 

            : basic_work_queue(uiPriority, usStackDepth, uiMaxWorkItems) {

            m_uiMaxWorkers = uiMaxWorkers;

            char name[32];

            for (int i = 0; i < m_uiMaxWorkers; i++) {
                sprintf(name, "work_multi_%d", i);

                work_queue_task *pWorker = new work_queue_task(name,
                                                                m_uiPriority, 
                                                                m_usStackDepth, 
                                                                this);

                if(pWorker)
                    m_Workers.push_back(pWorker);  
            }
        }

        //-----------------------------------
        //  create_engine
        //-----------------------------------
        int basic_work_queue_multi::create_engine(int iCore) {
            automutx_t lock(m_ThreadStatus);

            bool _errorOnCreate = false;
            bool _oneNoError = false;

            if(m_bRunning) { 
                return ERR_WORKQUEUE_ALREADYINIT;
            }

            m_bRunning = true;

            for(int i = 0; i < get_num_worker(); i++) {
                if(m_Workers[i]->start(iCore) != ERR_TASK_OK) {
                    _errorOnCreate = true;
                } else {
                    _oneNoError = true;
                }
            }
            if( (_errorOnCreate && _oneNoError) || 
                (_oneNoError && get_num_worker() == 1) ) {
                return ERR_WORKQUEUE_WARNING;
            }
            else if( (_errorOnCreate && !_oneNoError) || 
                    (!_errorOnCreate && !_oneNoError)) {
                return ERR_WORKQUEUE_CANTCREATE;
            }
            
            return ( m_uiMaxWorkers == get_num_worker() ) ? ERR_WORKQUEUE_OK : ERR_WORKQUEUE_WARNING;
        }

        //-----------------------------------
        //  destroy_engine
        //-----------------------------------
        void basic_work_queue_multi::destroy_engine() {
            for(int i = 0; i < get_num_worker(); i++) {
                m_Workers[i]->kill();
            }
            m_Workers.clear();
            
        }

        //-----------------------------------
        //  get_num_worker
        //-----------------------------------
        uint8_t basic_work_queue_multi::get_num_worker() const  {
        return m_Workers.size();
        }

        //-----------------------------------
        //  get_num_max_worker
        //-----------------------------------
        uint8_t basic_work_queue_multi::get_num_max_worker() const   {
            return m_uiMaxWorkers;
        }

        //-----------------------------------
        //  workers
        //-----------------------------------
        std::vector<work_queue_task*>& basic_work_queue_multi::workers() {
            return m_Workers;
        }
    }
}