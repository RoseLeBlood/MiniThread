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
#ifndef MINLIB_ESP32_BLOCKING_QUEUE_
#define MINLIB_ESP32_BLOCKING_QUEUE_

#include "../mn_autolock.hpp"
#include "mn_queue.hpp"

/**
 * Extends the basic_queue with thread (i.e. task) satly
 */
class basic_blocking_queue : public basic_queue {
public:
    /**
     *  ctor
     * 
     *  @param maxItems Maximum number of items this queue can hold.
     *  @param itemSize Size of an item in a queue.
     */
    basic_blocking_queue(unsigned int maxItems, unsigned int itemSize);

    /**
     * Create the queue
     * 
     *  @return 'ERR_QUEUE_OK': the queue was created
     *          'ERR_QUEUE_ALREADYINIT': the queue is allready created
     *          'ERR_QUEUE_CANTCREATE': queue can not created
     *          'ERR_QUEUE_CANTCREATE_LOCK': locx object can't created
     * 
     */ 
    virtual int create();

    /**
     * Destroy the Queue
     * 
     *  @return 'ERR_QUEUE_OK' the queue was destroyed 
     *          'ERR_QUEUE_NOTCREATED' the queue is not created
     */
    virtual int destroy();
    /**
     *  Add an item to the back of the queue.
     *
     *  @param item The item you are adding.
     *  @param timeout How long to wait to add the item to the queue 
     *  @return 'ERR_QUEUE_OK' the item was added, 'ERR_QUEUE_ADD' on an error
     *          and 'ERR_QUEUE_NOTCREATED' when the queue not created
     */
    virtual int enqueue(void *item, 
                    unsigned int timeout = (unsigned int) 0xffffffffUL);


    /**
     *  Make a copy of an item from the front of the queue. 
     *  This will not remove it
     *
     *  @param item Where the item you are removing will be returned to.
     *  @param timeout How long to wait 
     *  @return 'ERR_QUEUE_OK' if an item was copied, 'ERR_QUEUE_PEEK' on error 
     *  and 'ERR_QUEUE_NOTCREATED' when the queue not created
     */
    virtual int peek(void *item,
                    unsigned int timeout = (unsigned int) 0xffffffffUL);

    /**
     *  Remove an item from the front of the queue.
     *
     *  @param item Where the item you are removing will be returned to.
     *  @param timeout How long to wait to remove an item to the queue.
     *  @return 'ERR_QUEUE_OK' the item was removed, 'ERR_QUEUE_REMOVE' on an error
     *          and 'ERR_QUEUE_NOTCREATED' when the queue not created
     */
    virtual int dequeue(void *item, 
                       unsigned int timeout = (unsigned int) 0xffffffffUL);

    /**
     *  Is the queue empty?
     *  @return true the queue is empty and false when not
     */
    virtual bool is_empty() { 
        autobinsemp_t autolock(m_semp);
        return basic_queue::is_empty();
    }
    /**
     *  Is the queue full?
     *  @return true the queue is full and false when not
     */
    virtual bool is_full() { 
        autobinsemp_t autolock(m_semp);
        return basic_queue::is_full();
    }
    
    /**
     *  Remove all objects from the queue.
     */
    virtual int clear() { 
        autobinsemp_t autolock(m_semp);
        return basic_queue::clear();
    }

    /**
     *  How many items are currently in the queue.
     *  @return the number of items in the queue.
     */
    virtual unsigned int get_num_items() { 
        autobinsemp_t autolock(m_semp);
        return basic_queue::get_num_items();
    }

    /**
     *  How many empty spaves are currently left in the queue.
     *  @return the number of remaining spaces.
     */
    virtual unsigned int get_left() { 
        autobinsemp_t autolock(m_semp);
        return basic_queue::get_left();
    }
protected:
    binary_semaphore_t m_semp;
};

#endif