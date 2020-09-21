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

/**
 * This class is the multi thread "engine" for work_queue_items.
 */
class basic_work_queue_multi : public basic_work_queue {
public:
    /**
     * Our constructor.
     * @param Name Name of the thread internal to the WorkQueue. 
     * @param uiPriority FreeRTOS priority of this Thread.
     * @param usStackDepth Number of "words" allocated for the Thread stack.
     * @param uiMaxWorkItems Maximum number of WorkItems this WorkQueue can hold.
     */
    basic_work_queue_multi(unsigned int uiPriority = MN_THREAD_CONFIG_WORKQUEUE_MULTI_PRIORITY,
                uint16_t usStackDepth = MN_THREAD_CONFIG_WORKQUEUE_MULTI_STACKSIZE,
                uint8_t uiMaxWorkItems = MN_THREAD_CONFIG_WORKQUEUE_MULTI_MAXITEMS,
                uint8_t uiMaxWorkers = MN_THREAD_CONFIG_WORKQUEUE_MULTI_WORKER);

    /**
     * Our destructor.
     */
    ~basic_work_queue_multi();

    /**
     * Get the real num worker threads for this workqueue engine
     * @return The real num worker threads for this workqueue engine
     */ 
    uint8_t get_num_worker() const;
    /**
     * Get the num worker threads for this workqueue engine
     * @return The num worker threads for this workqueue engine
     */ 
    uint8_t get_num_max_worker() const;
    /**
     * Get tde reference ot all workqueue threads
     */ 
    std::vector<work_queue_thread*>& workers();
protected:
    /**
     * Create this multi threaded work queue
     *
     * @param iCore run on whith core
     * @return TODO
     */
    int on_create(int iCore);

    /**
     * Destroy this multi threaded work queue
     */
    void on_destroy();

private:
    /**
     * Vector for all workqueue threads
     */ 
    std::vector<work_queue_thread*> m_Workers;
    /**
     * Holder of num worker threads for this workqueue engine
     */ 
    uint8_t m_uiMaxWorkers;
};

using multi_engine_workqueue_t = basic_work_queue_multi;

#endif