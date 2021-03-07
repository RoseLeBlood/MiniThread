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
#ifndef _MINILIB_19dfc60c_16c2_4ec9_8b2d_8c9f1617631a_H_
#define _MINILIB_19dfc60c_16c2_4ec9_8b2d_8c9f1617631a_H_

#include "../mn_config.hpp"
#include "../queue/mn_deque.hpp"
#include "../mn_algorithm.hpp"

namespace mn {
	namespace container {
        template <class T, class TQUEUE> 
        class basic_deque_iterator {
        public:
            using iterator_category = mn::forward_iterator_tag;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;

            using deque_type = TQUEUE;
            using self_type = basic_deque_iterator<T, TQUEUE>;
            using pointer_deque  = TQUEUE*;

            explicit basic_deque_iterator(pointer_deque _pQueue, bool isEnd = false) 
                : m_pValue(NULL), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            explicit basic_deque_iterator(pointer value, pointer_deque _pQueue, bool isEnd = false) 
                : m_pValue(value), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

            template<class U, class UQUEUE> 
            basic_deque_iterator(const basic_deque_iterator<U, UQUEUE>& rhs)
                : m_pValue(rhs.m_pValue), m_pQueue(rhs._pQueue), m_bIsEnd(rhs.isEnd) { }

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
            pointer_deque m_pQueue;
            bool          m_bIsEnd;
        };

        /**
         * @brief A basic wrapper for the MiniLib deque's as Template deque
         * 
         * @tparam T         The type of an element
         * @tparam TMAXITEMS Maximal items can queue 
         * @tparam TITEMSIZE A size of a item
         */
        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE = sizeof(T) > 
        class basic_deque {
        public:
            using deque_type = mn::queue::deque_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using const_reference = const T&;
            using self_type = basic_deque<T, TMAXITEMS, TITEMSIZE>;
            using iterator = basic_deque_iterator<T, deque_type>;
            using const_iterator = const iterator;
            using size_type = mn::size_t;

            /**
             * @brief Construct a new basic queue object
             */
            basic_deque() : m_deque(TMAXITEMS, TITEMSIZE) { m_deque.create(); }

            basic_deque(const self_type& other) 
                : m_deque(other.m_deque), m_pEnd(other.m_pEnd), m_pFront(other.m_pFront) { }
                
            /**
             * @brief Destroy the basic queue object
             */
            ~basic_deque() { m_deque.destroy(); }

            
            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            iterator begin() {
                return basic_deque_iterator( intern_getfront(), m_deque);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            iterator end() {
                return basic_deque_iterator(m_pEnd, &m_deque, true);
            }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            const_iterator begin() const {
                return basic_deque_iterator( intern_getfront(), m_deque);
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            const_iterator end() const {
                return basic_deque_iterator(m_pEnd, &m_deque, true);
            }
            const_reference front() const  { 
                assert(!empty()); return *m_pFront; }

            reference front() { 
                assert(!empty()); return *m_pFront; }

            const_reference back() const {
                 assert(!empty()); return *m_pEnd; }
                 
            reference back() { 
                assert(!empty()); return *m_pEnd; }

            /**
             * @brief Push a element to the end of the queue
             * @param pElement The element 
             */
            void push_back(reference _Element) {
                if( m_deque.enqueue(&_Element) == ERR_QUEUE_OK) {
                    m_pEnd = &_Element;
                }
                intern_getfront();
            }
            /**
             * @brief Push a element to the front of the queue
             * @param pElement The element 
             */
            void push_front(reference _Element) {
                if( m_deque.enqueue_front(&_Element) == ERR_QUEUE_OK) {
                    m_pFront = &_Element;
                }
            }
            /**
             * @brief Pop the oldest entry from the queue.
             * @return The oldest entry from the queue  
             */
            inline pointer pop_front() { 
                pointer tmp = NULL;
                m_deque.dequeue(tmp);
                
                intern_getfront();

                return tmp;
            }
            /**
             * @brief Clear the queue
             */
            inline void clear() {
                m_deque.clear();
            }
            /**
             * @brief Check, if queue is empty.
             * 
             * @return true The queue is empty and false when not
             */
            inline const bool empty() const {
                return m_deque.is_empty();
            }
            /**
             * @brief How many items can queue
             * @return The maximal number of entries can queue
             */
            inline const  mn::size_t length() const {
                return TMAXITEMS;
            }
            /**
             *  How many items are currently in the queue.
             *  @return the number of items in the queue.
             */
            inline const mn::size_t size() const {
                return m_deque.get_num_items();
            }

            /**
             *  How many empty spaves are currently left in the queue.
             *  @return the number of remaining spaces.
             */
            inline const mn::size_t left() const {
                return m_deque.get_left();
            }

            void swap(const self_type& other) {
				self_type _temp(this);
				other = this;
				this = _temp;
			}
            
            self_type& operator = (const self_type& other) {
                m_deque = other.m_deque;
                m_pEnd = other.m_pEnd;
                m_pFront = other.m_pFront;
                return *this;
            }
        private:
            T* intern_getfront() {
                m_deque.peek(m_pFront, MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
                return m_pFront;
            }
        private:
            deque_type m_deque;
            pointer m_pEnd;
            pointer m_pFront;
        };

        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE> 
        void swap(basic_deque<T, TMAXITEMS, TITEMSIZE>& a, 
                  basic_deque<T, TMAXITEMS, TITEMSIZE>& b) {
            a.swap(b);
        }

        template <class T, mn::size_t TMAXITEMS, mn::size_t TITEMSIZE = sizeof(T)> 
        using deque = basic_deque<T, TMAXITEMS, TITEMSIZE>;
    }
}

#endif