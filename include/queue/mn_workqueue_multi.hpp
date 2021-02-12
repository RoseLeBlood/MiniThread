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
#ifndef MINLIB_ESP32_WORK_QUEUE_MULTI_
#define MINLIB_ESP32_WORK_QUEUE_MULTI_

#include "mn_workqueue.hpp"
#include <vector>

namespace mn {
    namespace queue {
        /**
         * This class is the multi task "engine" for work_queue_items.
         * 
         * @ingroup queue
         */
        class basic_work_queue_multi : public basic_work_queue {
        public:
            /**
             * Our constructor.
             * 
             * @param uiPriority FreeRTOS priority of this task.
             * @param usStackDepth Number of "words" allocated for the task stack.
             * @param uiMaxWorkItems Maximum number of WorkItems this WorkQueue can hold.
             * @param uiMaxWorkers How many Worker tasks run with this workqueue
             */
            basic_work_queue_multi(basic_task::priority uiPriority = MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY,
                        uint16_t usStackDepth = MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE,
                        uint8_t uiMaxWorkItems = MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS,
                        uint8_t uiMaxWorkers = MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER);

            /**
             * Our destructor.
             */
            ~basic_work_queue_multi();

            /**
             * Get the real num worker tasks for this workqueue engine
             * @return The real num worker threads for this workqueue engine
             */ 
            uint8_t get_num_worker() const;
            /**
             * Get the num worker tasks for this workqueue engine
             * @return The num worker threads for this workqueue engine
             */ 
            uint8_t get_num_max_worker() const;
            /**
             * Get tde reference ot all workqueue tasks
             */ 
            std::vector<work_queue_task*>& workers();
        protected:
            /**
             * Create this multi tasked work queue
             *
             * @param iCore run on whith core
             * @return 
             *   - ERR_WORKQUEUE_OK The engine is created
             *   - ERR_WORKQUEUE_WARNING Not all worker tasks are created
             *   - ERR_WORKQUEUE_CANTCREATE The engine can not created
             */
            int create_engine(int iCore);

            /**
             * Destroy this multi tasked work queue
             */
            void destroy_engine();

        private:
            /**
             * Vector for all workqueue threads
             */ 
            std::vector<work_queue_task*> m_Workers;
            /**
             * Holder of num worker threads for this workqueue engine
             */ 
            uint8_t m_uiMaxWorkers;
        };

        using multi_engine_workqueue_t = basic_work_queue_multi;
    }
}

#endif