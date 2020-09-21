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
#ifndef MINLIB_ESP32_WORK_QUEUE_SINGLE_
#define MINLIB_ESP32_WORK_QUEUE_SINGLE_

#include "mn_workqueue.hpp"


/**
 * This class is the single thread "engine" for work_queue_items.
 */
class basic_work_queue_single : public basic_work_queue {
public:
    /**
     * Our constructor.
     * @param Name Name of the thread internal to the WorkQueue. 
     * @param uiPriority FreeRTOS priority of this Thread.
     * @param usStackDepth Number of "words" allocated for the Thread stack.
     * @param uiMaxWorkItems Maximum number of WorkItems this WorkQueue can hold.
     */
    basic_work_queue_single(unsigned int uiPriority = MN_THREAD_CONFIG_WORKQUEUE_SINGLE_PRIORITY,
                uint16_t usStackDepth = MN_THREAD_CONFIG_WORKQUEUE_SINGLE_STACKSIZE,
                uint8_t uiMaxWorkItems = MN_THREAD_CONFIG_WORKQUEUE_SINGLE_MAXITEMS);

    /**
     * Our destructor.
     */
    ~basic_work_queue_single();
protected:
    /**
     * Create the work_queue_t.
     *
     * @param iCore run on whith core
     * @return TODO
     */
    int on_create(int iCore);

    /**
     * Destroy the work_queue_t.
     */
    void on_destroy();
private:
    /**
     *  Pointer to our WorkerThread.
     */
    work_queue_thread *m_pWorker;
};

using single_engine_workqueue_t = basic_work_queue_single;

#endif