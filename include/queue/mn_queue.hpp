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
#ifndef MINLIB_ESP32_QUEUE_
#define MINLIB_ESP32_QUEUE_

#include "mn_config.hpp"

namespace mn {
    namespace queue {
        /**
         *  @brief Queue class wrapper for FreeRTOS queues
         *  @note can use at the ISR Context, too. This wrapper switch automatic 
         * 
         * @ingroup queue
         */
        class basic_queue {
        public:
            /**
             *  ctor
             * 
             *  @param maxItems Maximum number of items this queue can hold.
             *  @param itemSize Size of an item in a queue.
             */
            basic_queue(unsigned int maxItems, unsigned int itemSize);

            /**
             *  dtor
             */
            virtual ~basic_queue();

            /**
             * Create the queue
             * 
             *  @return 'ERR_QUEUE_OK': the queue was created
             *          'ERR_QUEUE_ALREADYINIT': the queue is allready created
             *          'ERR_QUEUE_CANTCREATE': queue can not created
             * 
             *  @note FreeRTOS queues use a memcpy / fixed size scheme for queues.
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
                            unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);


            /**
             *  Make a copy of an item from the front of the queue. 
             *  This will not remove it
             *
             *  @param item Where the item you are getting will be returned to.
             *  @param timeout How long to wait 
             *  @return 'ERR_QUEUE_OK' if an item was copied, 'ERR_QUEUE_PEEK' on error 
             *  and 'ERR_QUEUE_NOTCREATED' when the queue not created
             */
            virtual int peek(void *item,
                            unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);

            /**
             *  Remove an item from the front of the queue.
             *
             *  @param item Where the item you are removing will be returned to.
             *  @param timeout How long to wait to remove an item to the queue.
             *  @return 'ERR_QUEUE_OK' the item was removed, 'ERR_QUEUE_REMOVE' on an error
             *          and 'ERR_QUEUE_NOTCREATED' when the queue not created
             */
            virtual int dequeue(void *item, 
                            unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);

            /**
             *  Is the queue empty?
             *  @return true the queue is empty and false when not
             */
            virtual bool is_empty();
            /**
             *  Is the queue full?
             *  @return true the queue is full and false when not
             */
            virtual bool is_full();
            
            /**
             *  Remove all objects from the queue.
             */
            virtual int clear();

            /**
             *  How many items are currently in the queue.
             *  @return the number of items in the queue.
             */
            virtual unsigned int get_num_items();

            /**
             *  How many empty spaves are currently left in the queue.
             *  @return the number of remaining spaces.
             */
            virtual unsigned int get_left();

            /**
             *  get the FreeRTOS queue handle
             *  @return the FreeRTOS handle
             */
            void*  get_handle() { return m_pHandle; }
        protected:
            /**
             *  FreeRTOS queue handle.
             */
            void*  m_pHandle;

            unsigned int m_imaxItems;
            unsigned int m_iitemSize;
        };

        using queue_t = basic_queue;
    }
}

#endif