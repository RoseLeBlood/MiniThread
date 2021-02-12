/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_DEQUE_
#define MINLIB_ESP32_DEQUE_

#include "mn_queue.hpp"

namespace mn {
    namespace queue {
        /**
         *  A deque class that implements a double ended queue.
         * 
         * @ingroup queue
         */
        class basic_deque : public basic_queue {
        public:
            /**
             *  ctor
             * 
             *  @param itemSize Size of an item in a queue.
             */
            basic_deque(unsigned int maxItems, unsigned int itemSize) 
                        : basic_queue(maxItems, itemSize) { }

            
            /**
             *  Add an item to the front of the queue. This will result in
             *  the item being removed first, ahead of all of the items
             *  added by the base cal dequeue() function.
             *
             *  @param item The item you are adding.
             *  @param Timeout How long to wait to add the item to the queue if
             *         the queue is currently full.
             *  @return 
             *      - ERR_QUEUE_OK The item was added 
             *      - ERR_QUEUE_ADD The item was not added to dequeue
             *      - ERR_QUEUE_NOTCREATED The queue was not created
             */
            virtual int enqueue_front(void* item, unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
        };

        using deque_t = basic_deque;
    }
}
#endif