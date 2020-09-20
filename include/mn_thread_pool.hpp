/*
This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
Copyright (c) 2020 Amber-Sophia Schroeck
  
The Mini Thread Library is free software; you can redistribute it and/or modify  
it under the terms of the GNU Lesser General Public License as published by  
the Free Software Foundation, version 3, or (at your option) any later version.
 
The Mini Thread Library is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public
License along with the Mini Thread  Library; if not, see
<https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_ESP32_THREAD_POOL_
#define MINLIB_ESP32_THREAD_POOL_

#include <vector>
#include "mn_config.hpp"
#include "mn_thread.hpp"

#include "queue/mn_blocking_queue.hpp"

/**
 * 
 * @example
 * virtual void on_work() {
 *	    static int nCount = 0;
 *	    basic_thread::sleep(1);
 *      {
 *          autolock_t lock(*locker);
 *	        printf("\n%d", ++nCount);
 *      }
 *   } 
 */
class basic_task_pool_job {
public:
	basic_task_pool_job(bool deleteAffter = false) 
        : m_bCanDelete(deleteAffter) { }

    bool can_delete() { return m_bCanDelete; }
protected:
    virtual void on_work() = 0;
private:
    bool m_bCanDelete;
};

class basic_task_pool {
    /**
     * An internal derived basic_thread class, in which we do our real work.
     */
    class basic_task_pool_worker : public basic_thread {
    public:
        basic_task_pool_worker(unsigned int uiPriority,
                            unsigned short  usStackDepth, basic_task_pool& parent)
            : basic_thread("basic_task_pool_worker", uiPriority, usStackDepth), 
            m_pParent(parent) { }

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
         * Implementation of your actual basic_task_pool_worker code
         * get a new job from the task pool and execute the job
         * @return The pointer of m_uiNumWorks - @see get_num_works()
         */ 
        virtual void*  on_thread();
    private:
        basic_task_pool& m_pParent;
    };
public:
    /**
     * Our constructor.
     * @param uiPriority FreeRTOS priority of the internal  worker threads
     * @param usStackDepth Number of "words" allocated for the Thread stack.
     * @param uiMaxJobs Maximum number of basic_task_pool_jobs this basic_task_pool can hold.
     * @param uiMaxWorkers Maximum number of internal basic_task_pool_worker threads
     */
    basic_task_pool(unsigned int uiPriority = MN_THREAD_CONFIG_THREAD_POOL_PRIORITY,
                    unsigned short usStackDepth = MN_THREAD_CONFIG_THREAD_POOL_STACK_SIZE,
                    unsigned int uiMaxJobs = MN_THREAD_CONFIG_THREAD_POOL_MAX_JOBS, 
                    unsigned int uiMaxWorkers = MN_THREAD_CONFIG_THREAD_POOL_WORKERS);

    /**
     * Send a basic_task_pool_job off to be executed.
     *
     * @param job Pointer to a basic_task_pool_job.
     * @param timeout How long to wait to add the item to the job queue 
     * @note This function may block if the basic_blocking_queue is presently full.
     */
    virtual int queue(basic_task_pool_job* job
                      unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);

    /**
     * Create the worker threads and start the pool 
     *
     * @param iCore On which core run the worker threads 
     * @return TODO
     */
    virtual int create(int iCore = -1);
    /**
     * Stop the worker threads - don't destroy the worker
     * 
     * @return TODO
     */ 
    virtual int stop();

    volatile bool& running() { return m_bRunning; }
protected:
    basic_task_pool_job* get_next_job();
private:
    std::vector<basic_task_pool_worker*> m_Workers;
	basic_blocking_queue* m_Jobs;
    mutex_t* m_ThreadStatus;
	mutex_t* m_ThreadJob;
    
    unsigned int m_uiMaxWorkers;
    unsigned int m_uiPriority ;
    unsigned short m_usStackDepth;

	volatile bool m_bRunning;

	friend class basic_task_pool_worker;
};

#endif

#endif