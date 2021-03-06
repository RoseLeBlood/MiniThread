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
#ifndef MINLIB_ESP32_BINQUEUE_
#define MINLIB_ESP32_BINQUEUE_

#include "mn_queue.hpp"

namespace mn {
    namespace queue {
        /**
         * @brief Binary queue with overwrite. This queue can only hold one item.
         * If sucessive Enqueue operations are called, that item is overwritten
         * with whatever the last item was.
         * 
         * @ingroup queue
         */
        class basic_binaryqueue : public basic_queue {
        public:
            /**
             * @brief Construct a new basic binaryqueue object
             * 
             * @param itemSize Size of an item in a queue.
             */
            explicit basic_binaryqueue(unsigned int itemSize) 
                        : basic_queue(1, itemSize) { }
            /**
             *  Add an item to the queue.
             *
             *  @param item The item you are adding.
             *  @param timeout not use 
             *  @return 
             *      - ERR_QUEUE_OK The item in the queue overwrited 
             *      - ERR_QUEUE_NOTCREATED The queue was not created, please call the function create first
             */
            virtual int enqueue(void *item,  unsigned int timeout = -1);
        };

        using binaryqueue_t = basic_binaryqueue;
    }
}

#endif