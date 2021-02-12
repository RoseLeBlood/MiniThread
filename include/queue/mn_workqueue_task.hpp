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

#ifndef MINLIB_ESP32_WORK_QUEUE_THREAD_
#define MINLIB_ESP32_WORK_QUEUE_THREAD_

#include "../mn_task.hpp"

namespace mn {
    namespace queue {
        class basic_work_queue;

        /**
         * The basic worker task for single task and multi task workqueue engines
         * 
         * @ingroup queue
         */ 
        class work_queue_task : public basic_task {
            public:
                /**
                 * Constructor for this workqueue task.
                 *
                 * @param strName Name of the task. Only useful for debugging.
                 * @param uiPriority FreeRTOS priority of this Task.
                 * @param usStackDepth Number of "words" allocated for the Task stack. 
                 * @param parent The base work_queue for this worker Task
                 */
                work_queue_task(char const* strName, basic_task::priority uiPriority,
                                    unsigned short  usStackDepth,
                                    basic_work_queue* parent);

                virtual ~work_queue_task();

            protected:
                /**
                 * Implementation of your actual work queue working code ( Omg ...)
                 * @return The pointer of m_uiNumWorks - @see get_num_works()
                 */ 
                virtual void* on_task();
            private:
                /**
                 * Holder of the base work_queue for this worker thread
                 */
                basic_work_queue* m_parentWorkQueue;
        };
    }
}

#endif