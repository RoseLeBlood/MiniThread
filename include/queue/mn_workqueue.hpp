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

#ifndef MINLIB_ESP32_WORK_QUEUE_BASE_
#define MINLIB_ESP32_WORK_QUEUE_BASE_

#include "mn_queue.hpp"
#include "mn_workqueue_item.hpp"
#include "mn_workqueue_task.hpp"

namespace mn {
    namespace queue {
        /**
         * This abstract class is the base "engine" class  for all work_queues.
         * basic_work_queue pull work_queue_item off of a FIFO queue and 
         * run them sequentially.
         * 
         * This is an abstract base class.
         * To use this, you need to subclass it. All of your basic_work_queue should
         * be derived from the basic_work_queue class. Then implement the virtual create_engine
         * and destroy_engine functions.
         * 
         * @ingroup queue
         */
        class basic_work_queue {
            friend class work_queue_task;
        public:
            /**
             * Our constructor.
             * @param Name Name of the task internal to the WorkQueue. 
             * @param uiPriority FreeRTOS priority of this WorkQueue task.
             * @param usStackDepth Number of "words" allocated for the WorkQueue task stack.
             */
            basic_work_queue(basic_task::priority uiPriority,
                            uint16_t usStackDepth,
                            uint8_t uiMaxWorkItems);

            /**
             * Our destructor.
             */
            virtual ~basic_work_queue();

            /**
             * Create and start the work queue engine
             * 
             * @return 'ERR_WORKQUEUE_OK' The workqueue are created,
             *         'ERR_WORKQUEUE_ALREADYINIT' the workqueue was allready created,
             *         'ERR_WORKQUEUE_CANTCREATE' error to create the workqueue and
             *         'ERR_WORKQUEUE_WARNING' not all threads are created in the multi engine workqueue
             * 
             * @note call internal on_create, for your real engine creating
             */ 
            virtual int create(int iCore = MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE);
            /**
             * Destroy and set the running flag to false
             * @note call internal on_destroy, for your real engine creating
             */ 
            virtual void destroy();

            /**
             * Send a work_queue_item_t off to be executed.
             *
             * @param work Pointer to a work_queue_item_t.
             * @note This function may block if the basic_work_queue is presently full.
             * 
             * @return 
             *  - ERR_WORKQUEUE_OK The work_queue_item_t are added 
             *  - ERR_WORKQUEUE_ADD If The work_queue_item_t are not added 
             */ 
            virtual int queue(work_queue_item_t *work,
                            unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);

            /**
             * Is the workqueue running?
             * 
             * @return true If the workqueue running, false If not
             */ 
            volatile bool& running() { return m_bRunning; }

            /**
             * How many items / jobs are sucessfull worked
             */ 
            uint8_t get_num_items_worked();
            /**
             * How many items/jobs are not sucessfull worked
             */ 
            uint8_t get_num_items_error();

            /**
             * Is the workqueue ready, all jobs/items are worked?
             * 
             * @return true If workqueue is ready, false If not
             */ 
            bool is_ready();
        protected:
            /**
             * Get the next item / job from queue
             * 
             * @param timeout How long to wait to get an item / job from the queue.
             * @return The next item / job from list and NULL when list empty or timeout
             */ 
            virtual work_queue_item* get_next_item(unsigned int timeout);

            /**
             * Implementation of your actual create code.
             * You must override this function.
             *
             * @param iCore uiCore on which core run this thread (i. e. task ), -1 for current core
             * 
             * @note Don't forget to set the running flag to true
             */
            virtual int create_engine(int iCore) = 0;
            /**
             * Implementation of your actual destroy code.
             * You must override this function.
             */ 
            virtual void destroy_engine() = 0;
        protected:
            /**
             * The job queue for all workqueues
             */ 
            queue_t* m_pWorkItemQueue;
            /**
             * Lock Objekt for thread safty
             * Mutex lock for status and flags changes
             */ 
            mutex_t  m_ThreadStatus;
            /**
            * Lock Objekt for thread safty
            * Mutex lock for the job queue
            */ 
            mutex_t  m_ThreadJob;
            
            basic_task::priority m_uiPriority ;
            uint16_t m_usStackDepth; 
            uint8_t m_uiMaxWorkItems;

            /**
            * Holder of num works are successfull run
            */ 
            volatile uint8_t m_uiNumWorks;
            /**
            * Holder of num works are not successfull run
            */ 
            volatile uint8_t m_uiErrorsNumWorks;
            /**
            * Flag whether or not the workqueue was started.
            */ 
            volatile bool m_bRunning;
        };
    }
}

#endif