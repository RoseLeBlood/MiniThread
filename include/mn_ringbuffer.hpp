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

#ifndef MINLIB_ESP32_RINGBUFFER_
#define MINLIB_ESP32_RINGBUFFER_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mn_config.hpp"
#include "mn_autolock.hpp"
#include "mn_allocator.hpp"


namespace mn {

    namespace container {

        /**
         * @brief ring_buffer_iterator
         * 
         * @tparam T The ringbuffer type
         * @tparam E 
         * 
         */
        template<typename T, typename E = typename T::value_type>
        class basic_ring_buffer_iterator 
        {
        public:
            using value_type = T;
            using size_type = size_t;
            using difference_type = ptrdiff_t; 
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using self_type = basic_ring_buffer_iterator<T>;

            using ringbuffer_iterator = typename T::iterator; 
            using const_ringbuffer_iterator = typename T::const_iterator;
            
            basic_ring_buffer_iterator(pointer b, size_t start_pos)
                : m_buf(b), m_pos(start_pos) { }
            basic_ring_buffer_iterator(const basic_ring_buffer_iterator& it)
                : m_buf(it.m_buf), m_pos(it.m_pos) { }
            
            basic_ring_buffer_iterator& operator = (const basic_ring_buffer_iterator& it) {
                m_buf = it.m_buf; m_pos = it.m_pos; return *this;
            }
        
            E &operator*() {
                return (*m_buf)[m_pos]; 
            }
            E *operator->() { 
                return &(operator*()); 
            }
            self_type &operator++() {
                ++m_pos;
                return *this;
            }
            self_type &operator--() {
                --m_pos;
                return *this;
            }
            self_type operator ++ (int) {
                self_type tmp(*this);
                ++(*this);
                return tmp;
            }
            self_type operator -- (int) {
                self_type tmp(*this);
                --(*this);
                return tmp;
            }
            self_type operator + (difference_type n) {
                self_type tmp(*this);
                tmp.m_pos += n;
                return tmp;
            }
            self_type operator - (difference_type n) {
                self_type tmp(*this);
                tmp.m_pos -= n;
                return tmp;
            }
            self_type &operator += (difference_type n) {
                m_pos += n;
                return *this;
            }
            self_type &operator -= (difference_type n) {
                m_pos -= n;
                return *this;
            }
            bool operator == (const self_type &other) const {
                return (this->m_pos == other.m_pos);
            }
            bool operator != (const self_type &other) const {
                return (this->m_pos != other.m_pos);
            }
            value_type& operator [] (difference_type n) const {
                return *(*this + n); 
            }
            bool operator < (const self_type& it) const {
                    return m_pos < it.m_pos;
            }
            bool operator > (const self_type& it) const {
                return   it < *this;  
            }
            bool operator <= (const self_type& it) const { 
                return !(it < *this); 
            }
            bool operator >= (const self_type& it) const { 
                return !(*this < it); 
            }
        private:
            value_type *m_buf;
            size_t     m_pos;  
        };

        /**
         * @brief A basic ring buffer with iterator 
         * @note A ring buffer is a FIFO (first-in, first-out) container which acts
	     * much like a queue. The difference is that a ring buffer is implemented
	     * via chasing pointers around a container and moving the read and write
	     * positions forward (and possibly wrapping around) as the container is 
	     * read and written via pop_front and push_back.
         * 
         * @tparam T          Type of element. Required to be a complete type.
         * @tparam TCAPACITY  The maximal capacity of elements, 
         * @tparam TLOCK      The lock object for task saftly
         */
        template <class T, size_t TCAPACITY = 100, typename TLOCK = LockType_t >
        class basic_ring_buffer {
        public:
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using size_type = size_t;
            using difference_type = ptrdiff_t; 
            using lock_type = TLOCK;
            using lock_guard = basic_autolock<TLOCK>;
            using self_type = basic_ring_buffer<T, TCAPACITY, TLOCK>;
            
            using iterator = basic_ring_buffer_iterator<self_type, value_type> ; 
            using const_iterator = basic_ring_buffer_iterator<self_type, const value_type>;

            basic_ring_buffer() :  m_Head(0), m_Tail(0), m_ContentsSize(0) { }
        
            /**
             * @brief Clear the ringbuffer and set read/write positoin to 0
             */
            void clear()  { 
                lock_guard lock(m_lockObject);
                m_Head = m_Tail = m_ContentsSize = 0; 
                memset(m_Array, 0, TCAPACITY);
            }
            /**
             * @brief Set read/write positoin to 0, for clear use function clear()
             */
            void reset() {
                lock_guard lock(m_lockObject);
                m_Head = m_Tail = 0;
            }
            
            /**
             * @brief push a value to the end of the buffer
             * @param value The value to add
             */
            void push_back(const value_type &value) {
                lock_guard lock(m_lockObject);

                inc_tail();
                if (m_ContentsSize == TCAPACITY)
                    inc_head();

                m_Array[m_Tail] = value;
            }
            /**
             * @brief pop (read) the element from the front 
             * and remove it from buffer
             * 
             * @return The poped value  
             */
            reference pop_front() { 
                lock_guard lock(m_lockObject);

                inc_head(); 
                return m_Array[m_Head];
            }
            /**
             * @brief pop (read) the element from the front 
             * and remove it from buffer
             * 
             * @return The poped value  
             */
            const_reference pop_front() const { 
                lock_guard lock(m_lockObject);

                inc_head(); 
                return m_Array[m_Head];
            }
      
            iterator begin() { 
                lock_guard lock(m_lockObject);
                return iterator(this, TCAPACITY == 0 ? 0 : m_Array[m_Head] ); 
            }
            iterator end() { 
                lock_guard lock(m_lockObject);
                return iterator(this, TCAPACITY); 
            }
            const_iterator cbegin() {
                lock_guard lock(m_lockObject);
                return const_iterator(this, 0); 
            }
            const_iterator cend() { 
                lock_guard lock(m_lockObject);
                return const_iterator(this, TCAPACITY); 
            }
            reference       rfront() {
                lock_guard lock(m_lockObject); 
                return m_Array[m_Head];
            }
            const_reference crfront() const { 
                lock_guard lock(m_lockObject);
                return m_Array[m_Head]; 
            }
            const_reference crback() const  { 
                lock_guard lock(m_lockObject);
                return m_Array[m_Tail]; 
            }
            reference       rback()  { 
                lock_guard lock(m_lockObject);
                return m_Array[m_Tail]; 
            }
            
            /**
             * @brief Get the size of the ringbuffer
             * 
             * @return The size of the ringbuffer 
             */
            size_type size() const {
                lock_guard lock(m_lockObject);
                return TCAPACITY;
            }
            /**
             * @brief Get the number of stored elements in the buffer
             * 
             * @return The number of stored elements in the buffer 
             */
            size_type capacity() const {
                lock_guard lock(m_lockObject);
                return m_ContentsSize;
            }
            /**
             * @brief Is the buffer empty?
             * 
             * @return true The buffer is empty
             * @return false The buffer is not empty
             */
            bool empty() const {
                lock_guard lock(m_lockObject);
                return TCAPACITY == 0; 
            }
            /**
             * @brief Is the buffer full?
             * 
             * @return true The buffer is full
             * @return false The buffer is not full
             */
            bool full() const {
                lock_guard lock(m_lockObject);
                return TCAPACITY == m_ContentsSize;
            }
            size_type max() const {
                lock_guard lock(m_lockObject);
                return size_type(-1) / sizeof(value_type);
            }

            /**
             * Get the write position (head)
             * @return The write position (head)
             */
            size_t get_head() {
                lock_guard lock(m_lockObject);
                return m_Head;
            }
            /**
             * Get the read position index  (head)
             * @return The read position  (head)
             */
            size_t get_tail() {
                lock_guard lock(m_lockObject);
                return m_Tail;
            }
            
            
        private:
            void inc_tail() {
                ++m_Tail;
                ++m_ContentsSize;

                if (m_Tail == TCAPACITY)  m_Tail = 0;
            }
            void inc_head() {
                ++m_Head;
                --m_ContentsSize;

                if (m_Head == TCAPACITY) m_Head = 0;
            }
        private:
            T           m_Array[TCAPACITY];
            size_t      m_Head;
            size_t      m_Tail;
            size_t      m_ContentsSize;
            lock_type   m_lockObject;
        };

        template <class T, size_t TCAPACITY = 100, typename TLOCK = LockType_t >
        using ringbuffer_t = basic_ring_buffer<T, TCAPACITY, TLOCK>;

#ifdef __EXPERT 
        template<class TRingBuffer, class TARRAY >
        inline int write(TRingBuffer& rng, TARRAY& _array) {
            using size_type = typename TRingBuffer::size_type;
            size_type t = 0;

            for(; t < _array.size(); t++) {
                rng.push_back(_array[t]);
            }

            return t;
        }

        template<class TRingBuffer, 
                 typename value_type = typename TRingBuffer::value_type, 
                 typename size_type = typename TRingBuffer::size_type>
        inline int write(TRingBuffer& rng, const value_type* tarray, const size_type size ) {
            size_type t = 0;

            for(; t < size; t++) {
                rng.push_back(_array[t]);
            }
            return t;
        }
#endif
    }

}

#endif // MINLIB_ESP32_RINGBUFFER_