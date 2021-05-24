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
#ifndef __MINLIB_ATOMIC_QUEUE_H__
#define __MINLIB_ATOMIC_QUEUE_H__

#include "../mn_atomic.hpp"

namespace mn {
	namespace container {

		/**
         * @brief A basic lockfree atomic queue
         *
         * @tparam T         The type of an element
         * @tparam TMAXITEMS Maximal items can queue
         */
        template <class T, mn::size_t TMAXITEMS >
        class basic_atomic_queue {
		public:
			struct node {
				T data;
				node * next;
			};
		public:
			using value_type = T;
			using reference = T&;
			using lreference = T&&;
			using pointer = node*;

			using const_value_type = const T;
			using const_reference = const T&;

			using self_type = basic_atomic_queue<T, TMAXITEMS>;

			basic_atomic_queue()
				: m_atHead(), m_curItems(0) { }

			basic_atomic_queue(const self_type& other) = delete;
			basic_atomic_queue(const self_type&& other) = delete;

			/**
             * @brief Push a element to the queue
             * @param pElement The element
             */
            bool push(reference _Element) noexcept {
            	if(full()) return false;

				node * n = new node;
				n->data = _Element;

				node* stale_head = m_atHead.load(memory_order::Release);
				m_curItems++;

				do {
					n->next = stale_head;
				} while (!m_atHead.compare_exchange_weak(stale_head, n, memory_order::Release));

				return true;
            }

            pointer pop_all(void) noexcept {
				T* last = m_atHead.exchange(0, memory_order::Consume), * first = nullptr;
				while(last) {
					T * tmp = last;
					last = last->next;
					tmp->next = first;
					first = tmp;
				}
				return first;
			}
            /**
             * @brief Clear the queue
             */
            constexpr void clear() noexcept {
                m_queue.clear();
            }
            /**
             * @brief Check, if queue is empty.
             *
             * @return true The queue is empty and false when not
             */
            constexpr bool empty() noexcept {
                return m_curItems.load() == 0;
            }

            constexpr bool full() noexcept {
				return m_curItems.load() == TMAXITEMS;
            }
            /**
             * @brief How many items can queue
             * @return The maximal number of entries can queue
             */
            constexpr  mn::size_t length() noexcept {
                return TMAXITEMS;
            }
            /**
             *  How many items are currently in the queue.
             *  @return the number of items in the queue.
             */
            constexpr  mn::size_t size() noexcept {
                return m_curItems.load();
            }

            /**
             *  How many empty spaves are currently left in the queue.
             *  @return the number of remaining spaces.
             */
            constexpr mn::size_t left() noexcept {
            	if(full()) return 0;
                return TMAXITEMS - m_curItems.load();
            }

            void swap(const self_type& other)  {
				mn::swap(m_atHead, other.m_atHead);
				mn::swap(m_curItems, other.m_curItems);
			}

            self_type& operator = (const self_type& other) {
				m_atHead = other.m_atHead;
				m_curItems = other.m_curItems;

				return *this;
            }
		protected:
			mn::_atomic<pointer> m_atHead;
			mn::atomic_size_t m_curItems;
        };

		template <class T, mn::size_t TMAXITEMS = 64>
        using atomic_queue =  basic_atomic_queue<T, TMAXITEMS>;
	}
}

#endif // __MINLIB_ATOMIC_QUEUE_H__
