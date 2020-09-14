/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2020 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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
     * An internal derived Thread class, in which we do our real work.
     */
    class work_queue_thread : public basic_thread {

        public:
            work_queue_thread(char const* strName, unsigned int uiPriority,
                              unsigned short  usStackDepth, basic_work_queue *parent);

            virtual ~work_queue_thread();

        protected:
            virtual void* on_thread();

        private:
            const basic_work_queue *m_parentWorkQueue;
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
     * 
     * @return TODO
     */
    int destroy();

    /**
     * Send a work_queue_item_t off to be executed.
     *
     * @param work Pointer to a work_queue_item_t.
     *  //@return true if it was queued, false otherwise.
     * @note This function may block if the work_queue_t is presently full.
     * 
     * TODO: ADD create function work_queue_item
     */ 
    int send(work_queue_item_t *work);
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
    binary_semaphore_t *m_pRunningMutex;
    
    bool m_created;

    bool m_bMutexInit;
};

using work_queue_t = basic_work_queue;

#endif