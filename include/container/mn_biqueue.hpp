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
#ifndef _MINILIB_a7a12e6f_55af_455f_8086_826c7dc83357_H_
#define _MINILIB_a7a12e6f_55af_455f_8086_826c7dc83357_H_

#include "../mn_config.hpp"
#include "../queue/mn_binaryqueue.hpp"
#include "../mn_algorithm.hpp"

namespace mn {
	namespace container {
        template<class T>
        class basic_binary_queue_iterator  {
        public:
            using iterator_category = forward_iterator_tag;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using self_type = basic_binary_queue_iterator<T>;
            using difference_type = ptrdiff_t;

            /// One of the @link iterator_tags tag types@endlink.
            using iterator_category = TCategory ;
            /// The type "pointed to" by the iterator.
            using value_type = T;
            /// Distance between iterators is represented as this type.
            using difference_type = ptrdiff_t; 
            /// This type represents a pointer-to-value_type.
            using pointer = value_type*;
            /// This type represents a reference-to-value_type.
            using reference = value_type& ;

            explicit basic_binary_queue_iterator(T* pValue)
                : m_pValue(pValue) { }

            template<class U> 
            basic_binary_queue_iterator(const basic_binary_queue_iterator<U>& rhs)
                : m_pValue(rhs.m_pValue) { }

            pointer get() const { return m_pValue; }

            reference operator*() const { return *m_pValue; }
            pointer operator->() const  { return m_pValue; }

            bool operator == (const self_type& rhs) const {
                return rhs.m_pValue == m_pValue; }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); }
        private:
            pointer m_pValue;
        }; 
        /**
         * @brief This queue can only hold one item. 
         * If sucessive push operations are called, that item is overwritten
         * with whatever the last item was.
         * 
         * @tparam T The type of the value
         * @tparam TITEMSIZE The size of one element
         */
        template<class T, mn::size_t TITEMSIZE = sizeof(T) >
        class basic_binary_queue {
        public:
            using queue_type = mn::queue::binaryqueue_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using const_reference = const T&;
            using self_type = basic_binary_queue<T, TITEMSIZE>;
            using iterator = basic_binary_queue_iterator<T>;
            using const_iterator = const iterator;
            

            basic_binary_queue() 
                : m_BinQueue(TITEMSIZE) { m_BinQueue.create(); }

            basic_binary_queue(const self_type& other) 
                : m_BinQueue(other.m_BinQueue) { }

            ~basic_binary_queue() { m_BinQueue.destroy(); }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            iterator begin() {
                return basic_binary_queue_iterator(intern_get());
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            iterator end() {
                return basic_binary_queue_iterator(intern_get());
            }

            /**
             * @brief Get the begin iterator
             * @return The begin iterator 
             */
            const_iterator begin() const {
                return basic_binary_queue_iterator(intern_get());
            }
            /**
             * @brief Get the end iterator
             * @return The end iterator 
             */
            const_iterator end() const {
                return basic_binary_queue_iterator(intern_get());
            }
            
            const_reference item() const  { 
                assert(!empty()); return *intern_get(); }

            reference item() { 
                assert(!empty()); return *intern_get(); }


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
            inline const bool empty() const {
                return m_queue.is_empty();
            }

            void swap(const self_type& other) {
				self_type _temp(this);
				other = this;
				this = _temp;
			}

            self_type& operator = (const self_type& other) {
                m_BinQueue = other.m_BinQueue:
                return *this;
            }
        private:
            T* intern_get() {
                pointer _pItem;
                m_queue.peek(_pItem, MN_THREAD_CONFIG_TIMEOUT_QUEUE_DEFAULT);
                return _pItem;
            }
        private:
            queue_type m_BinQueue;
        };

        template<class T, mn::size_t TITEMSIZE>
        void swap(basic_binary_queue<T, TITEMSIZE>& a, 
                  basic_binary_queue<T, TITEMSIZE>& b) {
            a.swap(b);
        }

        template<class T, mn::size_t TITEMSIZE = sizeof(T)>
        using biqueue = basic_binary_queue<T>;
    }
}

#endif