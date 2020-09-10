/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_DEQUE_
#define MINLIB_ESP32_DEQUE_

#include "mn_queue.hpp"

/**
 *  A deque class that implements a double ended queue.
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
     *  @return 'ERR_QUEUE_OK' the item was added, 'ERR_QUEUE_ADD' on an error
     *          and 'ERR_QUEUE_NOTCREATED' when the queue not created
     */
    virtual int enqueue_front(void* item, unsigned int timeout = (unsigned int) 0xffffffffUL);
};

using deque_t = basic_deque;

#endif