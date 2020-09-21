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

#include "../mn_thread.hpp"

class basic_work_queue;

/**
 * The basic worker thread for single threaded and multi threaded workqueue engines
 */ 
class work_queue_thread : public basic_thread {
    public:
        /**
         * Constructor for this thread.
         *
         * @param strName Name of the thread. Only useful for debugging.
         * @param uiPriority FreeRTOS priority of this Thread.
         * @param usStackDepth Number of "words" allocated for the Thread stack. 
         * @param parent The base work_queue for this worker thread
         */
        work_queue_thread(char const* strName, unsigned int uiPriority,
                            unsigned short  usStackDepth,
                            basic_work_queue* parent);

        virtual ~work_queue_thread();

    protected:
        /**
         * Implementation of your actual work queue working code ( Omg ...)
         * @return The pointer of m_uiNumWorks - @see get_num_works()
         */ 
        virtual void* on_thread();
    private:
        /**
         * Holder of the base work_queue for this worker thread
         */
        basic_work_queue* m_parentWorkQueue;
};

#endif