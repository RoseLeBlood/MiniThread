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
#ifndef MINLIB_ESP32_WORK_QUEUE_
#define MINLIB_ESP32_WORK_QUEUE_

#include "mn_workqueue_item.hpp"


//using work_queue_item_t = work_queue_item;

/**
 *  This class is the "engine" for work_queue_item. Create one or more basic_work_queue
 *  to accept work_queue_item. basic_work_queue pull work_queue_item off of a FIFO queue and 
 *  run them sequentially.
 */
class basic_work_queue {
    /**
     * An internal derived basic_thread class, in which we do our real work.
     */
    class work_queue_thread : public basic_thread {

        public:
            work_queue_thread(char const* strName, unsigned int uiPriority,
                              unsigned short  usStackDepth, basic_work_queue *parent);

            virtual ~work_queue_thread();

            /**
             * Get number of worked items, after run
             * 
             * @return The number of worked items, after run
             * @note When the thread are running then return only 0;
             */ 
            unsigned int get_num_works() { 
                void* val = get_return_value();
                return *((unsigned int *)val);
            }
        protected:
            /**
             * Implementation of your actual work queue working code ( Omg ...)
             * @return The pointer of m_uiNumWorks - @see get_num_works()
             */ 
            virtual void* on_thread();

        private:
            const basic_work_queue *m_parentWorkQueue;
            unsigned int m_uiNumWorks;
    };
public:
    /**
     * Our constructor.
     * @param Name Name of the thread internal to the WorkQueue. 
     * @param uiPriority FreeRTOS priority of this Thread.
     * @param usStackDepth Number of "words" allocated for the Thread stack.
     * @param uiMaxWorkItems Maximum number of WorkItems this WorkQueue can hold.
     */
    basic_work_queue(const char * strName,
                unsigned int uiPriority = MN_THREAD_CONFIG_WORK_QUEUE_PRIORITY,
                uint16_t usStackDepth = MN_THREAD_CONFIG_WORK_QUEUE_STACK_SIZE,
                unsigned int uiMaxWorkItems = MN_THREAD_CONFIG_WORK_QUEUE_MAX_WORK_ITEMS);

    basic_work_queue(const basic_work_queue* rev) = delete;
    /**
     * Our destructor.
     */
    ~basic_work_queue();

    /**
     * Create the work_queue_t.
     *
     * @param iCore run on whith core
     * @return TODO
     */
    int create(int iCore = MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE);

    /**
     * Destroy the work_queue_t.
     */
    void destroy();

    /**
     * Send a work_queue_item_t off to be executed.
     *
     * @param work Pointer to a work_queue_item_t.
     * @note This function may block if the work_queue_t is presently full.
     */ 
    int queue(work_queue_item_t *work);
private:
    /**
     *  Pointer to our WorkerThread.
     */
    work_queue_thread *m_pWorker;

    /**
     *  Pointer to our work queue itself.
     */
    queue_t *m_pWorkItemQueue;

    /**
     *  Semaphore to support deconstruction without race conditions.
     */
    binary_semaphore_t *m_pComplete;
    binary_semaphore_t *m_pRunningSem;
    
    bool m_running;

    bool m_bMutexInit;
};

using work_queue_t = basic_work_queue;

#endif