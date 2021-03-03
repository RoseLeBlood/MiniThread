/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2021 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.  
*/
#ifndef _MINILIB_51e64b00_707b_44f3_93dc_0b4c69dfd91c_H_
#define _MINILIB_51e64b00_707b_44f3_93dc_0b4c69dfd91c_H_

#include "../mn_config.hpp"
#include "../queue/mn_queue.hpp"
#include "../mn_algorithm.hpp"

namespace mn {
	namespace container {
        template <class T, class TQUEUE> 
        class basic_queue_iterator {
        public:
            using value_type = T;
            using pointer = T*;
            using queue_type = TQUEUE;
            using self_type = basic_queue_iterator<T, TQUEUE>;
            using pointer_queue = TQUEUE*;
            

            explicit basic_queue_iterator(pointer_queue _pQueue, bool isEnd = false) 
                : m_pValue(NULL), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            explicit basic_queue_iterator(pointer value, pointer_queue _pQueue, bool isEnd = false) 
                : m_pValue(value), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            template<class U, class UQUEUE> 
            basic_queue_iterator(const basic_queue_iterator<UNodePtr, UQUEUE>& rhs)
                : m_pValue(rhs.value), m_pQueue(rhs._pQueue), m_bIsEnd(rhs.isEnd) { }

            pointer get() const { return m_pValue; }

            reference operator*() const { return *m_pValue; }
            pointer operator->() const  { return m_pValue; }

            self_type& operator++()     { 
                if(m_bIsEnd) return *this; _pQueue.dequeue(m_pValue); return *this; }

            self_type operator++(int)   { 
                self_type copy(*this); ++(*this); return copy; }

            bool operator == (const self_type& rhs) const {
                return rhs.m_pValue == m_pValue; }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); }
        private:
            pointer*      m_pValue;
            pointer_queue m_pQueue;
            bool          m_bIsEnd;
        };


        /**
         * @brief A basic wrapper for the MiniLib queue's as Template queue
         * 
         * @tparam T         The type of an element
         * @tparam TMAXITEMS Maximal items can queue 
         * @tparam TQUEUE    The class wrapper of the FreeRTOS queue
         */
        template <class T, mn::size_t TMAXITEMS, class TQUEUE> 
        class basic_queue {
        public:
            using queue_type = TQUEUE;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using self_type = basic_queue<T, TMAXITEMS, TQUEUE>;
            using iterator = basic_queue_iterator<T, queue_type>;
            using const_iterator = const T*;
            using size_type = mn::size_t;

            /**
             * @brief Construct a new basic queue object
             */
            basic_queue() : m_queue(TMAXITEMS, sizeof(T)) { m_queue.create(); }

            /**
             * @brief Destroy the basic queue object
             */
            ~basic_queue() { m_queue.destroy(); }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            iterator begin() {
                m_queue.peek(m_pFront, MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
                return basic_queue_iterator(m_pFront, m_queue);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            iterator end() {
                return basic_queue_iterator(m_pEnd, &m_queue, true);
            }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            const_iterator begin() const {
                m_queue.peek(m_pFront, MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
                return basic_queue_iterator(m_pFront);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            const_iterator end() const {
                return basic_queue_iterator(m_pEnd, &m_queue, true);
            }

            /**
             * @brief Push a element to the queue
             * @param pElement The element 
             */
            void push(contst reference _Element) {
                if( m_queue.enqueue(&_Element) == ERR_QUEUE_OK) {
                    m_pEnd = &_Element;
                }
            }
            /**
             * @brief Pop the oldest entry from the queue.
             * @return The oldest entry from the queue  
             */
            inline pointer pop() { 
                pointer tmp = NULL;
                m_queue.dequeue(tmp);

                return tmp;
            }
            /**
             * @brief Clear the queue
             */
            inline void clear() {
                m_queue.clear();
            }
            /**
             * @brief Check, if queue is empty.
             * 
             * @return true The queue is empty and false when not
             */
            inline bool is_empty() {
                return m_queue.is_empty();
            }
            /**
             * @brief How many items can queue
             * @return The maximal number of entries can queue
             */
            inline  mn::size_t length() {
                return TMAXITEMS;
            }
            /**
             *  How many items are currently in the queue.
             *  @return the number of items in the queue.
             */
            inline mn::size_t size() {
                return m_queue.get_num_items();
            }

            /**
             *  How many empty spaves are currently left in the queue.
             *  @return the number of remaining spaces.
             */
            inline mn::size_t left() {
                return m_queue.get_left();
            }

        private:
            queue_type m_queue;
            pointer m_pEnd;
            pointer m_pFront;
        };

        /**
         * @tparam T The type of an Item to queue
         * @tparam TMAXITEMS Maximal items can queue 
         */
        template <class T, mn::size_t TMAXITEMS>
        using queue = basic_queue<T, TMAXITEMS, mn::queue::queue_t>;

        /**
         * @brief This queue can only hold one item.
         * If sucessive Enqueue operations are called, that item is overwritten
         * with whatever the last item was.
         * 
         * @tparam T The type of an Item to queue
         */
        template <class T>
        using binaryqueue = basic_queue<T, 1, mn::queue::binaryqueue_t>;

    }
}



#endif