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
            using iterator_category = mn::forward_iterator_tag;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;
            
            using queue_type = TQUEUE;
            using self_type = basic_queue_iterator<T, TQUEUE>;
            using pointer_queue = TQUEUE*;

            explicit basic_queue_iterator(pointer_queue _pQueue, bool isEnd = false) 
                : m_pValue(NULL), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            explicit basic_queue_iterator(pointer value, pointer_queue _pQueue, bool isEnd = false) 
                : m_pValue(value), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            template<class U, class UQUEUE> 
            basic_queue_iterator(const basic_queue_iterator<U, UQUEUE>& rhs)
                : m_pValue(rhs.m_pValue), m_pQueue(rhs._pQueue), m_bIsEnd(rhs.isEnd) { }

            pointer get() const { return m_pValue; }

            reference operator*() const { return *m_pValue; }
            pointer operator->() const  { return m_pValue; }

            self_type& operator++()     { 
                if(!m_bIsEnd) { m_pQueue.dequeue(m_pValue); } return *this; }

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
         * @tparam TITEMSIZE A size of a item
         */
        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE = sizeof(T) > 
        class basic_queue {
        public:
            using queue_type = mn::queue::queue_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using const_reference = const T&;
            using self_type = basic_queue<T, TMAXITEMS, TITEMSIZE>;
            using iterator = basic_queue_iterator<T, queue_type>;
            using const_iterator = const iterator;
            using size_type = mn::size_t;

            /**
             * @brief Construct a new basic queue object
             */
            basic_queue() : m_queue(TMAXITEMS, TITEMSIZE) { m_queue.create(); }

            basic_queue(const self_type& other) 
                : m_queue(other.m_queue), m_pEnd(other.m_pEnd), m_pFront(other.m_pFront) { }

            /**
             * @brief Destroy the basic queue object
             */
            ~basic_queue() { m_queue.destroy(); }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            iterator begin() {
                return iterator( intern_getfront(), m_queue);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            iterator end() {
                return iterator(m_pEnd, &m_queue, true);
            }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            const_iterator begin() const {
                return iterator( intern_getfront(), m_queue);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            const_iterator end() const {
                return iterator(m_pEnd, &m_queue, true);
            }
            const_reference front() const  { 
                assert(!empty()); return *intern_getfront(); }

            reference front() { 
                assert(!empty()); return *intern_getfront(); }

            const_reference back() const {
                 assert(!empty()); return *m_pEnd; }
                 
            reference back() { 
                assert(!empty()); return *m_pEnd; }

            /**
             * @brief Push a element to the queue
             * @param pElement The element 
             */
            void push(reference _Element) {
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
            inline bool empty() {
                return m_queue.is_empty();
            }
            /**
             * @brief How many items can queue
             * @return The maximal number of entries can queue
             */
            inline  mn::size_t length()  {
                return TMAXITEMS;
            }
            /**
             *  How many items are currently in the queue.
             *  @return the number of items in the queue.
             */
            inline  mn::size_t size() {
                return m_queue.get_num_items();
            }

            /**
             *  How many empty spaves are currently left in the queue.
             *  @return the number of remaining spaces.
             */
            inline  mn::size_t left()  {
                return m_queue.get_left();
            }

            void swap(const self_type& other) {
				self_type _temp(this);
				other = this;
				this = _temp;
			}

            self_type& operator = (const self_type& other) {
                m_queue = other.m_queue;
                m_pEnd = other.m_pEnd;
                m_pFront = other.m_pFront;
                return *this;
            }

        private:
            T* intern_getfront() {
                m_queue.peek(m_pFront, MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
                return m_pFront;
            }
        private:
            queue_type m_queue;
            pointer m_pEnd;
            pointer m_pFront;
        };

        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE> 
        void swap(basic_queue<T, TMAXITEMS, TITEMSIZE>& a, 
                  basic_queue<T, TMAXITEMS, TITEMSIZE>& b) {
            a.swap(b);
        }

        /**
         * @tparam T The type of an Item to queue
         * @tparam TMAXITEMS Maximal items can queue 
         * @tparam TITEMSIZE A size of a item
         */
        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE = sizeof(T)>
        using queue = basic_queue<T, TMAXITEMS, TITEMSIZE>;

    }
}



#endif