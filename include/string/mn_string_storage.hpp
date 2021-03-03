/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef _MINLIB_dd097dcc_4af0_4ae5_83bb_2756f1de38b1_H_
#define _MINLIB_dd097dcc_4af0_4ae5_83bb_2756f1de38b1_H_

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

namespace mn {
    namespace strings {

        template<typename E, class TAllocator> 
        class string_storage {
        public:
            using value_type = E;
            using size_type = size_t;
            using allocator_type = TAllocator;
            using const_iterator = const value_type*       ;
            static const size_type kGranularity = 32;

            explicit string_storage(const allocator_type& allocator)
        	    :  m_length(0), m_allocator(allocator) {
                m_data = construct_string(0, m_capacity);
            }

            string_storage(const value_type* str, const allocator_type& allocator)
                : m_allocator(allocator) {
                const int len = strlen(str);
                m_data = construct_string(len, m_capacity);
                memcpy(m_data, str, len*sizeof(value_type));
                m_length = len;
                m_data[len] = 0;
            }
            string_storage(const value_type* str, size_type len, const allocator_type& allocator)
                : m_allocator(allocator) {
                m_data = construct_string(len, m_capacity);
                memcpy(m_data, str, len*sizeof(value_type));
                m_length = len;
                m_data[len] = 0;
            }
            string_storage(size_type len, const allocator_type& allocator)
                : m_allocator(allocator) {
                m_data = construct_string(len, m_capacity);
                m_length = len;
                m_data[len] = 0;
            }
            string_storage(const string_storage& rhs, const allocator_type& allocator)
                : m_data(0), m_capacity(0), m_length(0), m_allocator(allocator) {
                assign(rhs.c_str(), rhs.length());
            }
            ~string_storage() {
                release_string();
            }

            string_storage& operator=(const string_storage& rhs) {
                if (m_data != rhs.c_str())  {
                    assign(rhs.c_str(), rhs.length());
                }
                return *this;
            }
            
            void assign(const value_type* str, size_type len) {
                if (m_capacity <= len + 1) {
                    release_string();
                    m_data = construct_string(len, m_capacity);
                }
                memcpy(m_data, str, len*sizeof(value_type));
                m_length = len;
                m_data[len] = 0;
            }
            void resize(size_type len) {
                const size_type prevLen = length();
                const size_type newLen = len;

                if (m_capacity <= newLen + 1) {
                    size_type newCapacity;
                    value_type* newData = construct_string(newLen, newCapacity);
                    memcpy(newData, m_data, prevLen * sizeof(value_type));
                    release_string();
                    m_data = newData;
                    m_capacity = newCapacity;
                }
                m_data[newLen] = 0;
                m_length = newLen;
                assert(invariant());
            }

            void append(const value_type* str, size_type len) {
                const size_type prevLen = length();
                const size_type newLen = prevLen + len;

                if (m_capacity <= newLen + 1) {
                    size_type newCapacity;
                    value_type* newData = construct_string(newLen, newCapacity);
                    memcpy(newData, m_data, prevLen * sizeof(value_type));
                    release_string();
                    m_data = newData;
                    m_capacity = newCapacity;
                }
                memcpy(m_data + prevLen, str, len * sizeof(value_type));
                m_data[newLen] = 0;
                m_length = newLen;
                assert(invariant());
            }

            inline const value_type* c_str() const {
                return m_data;
            }
            
            inline size_type length() const {
                return m_length;
            }
        
            inline size_type capacity() const { 
                return m_capacity; 
            }
        
            void clear()  {
                release_string();
                m_data = construct_string(0, m_capacity);
                m_length = 0;
            }
        
            const allocator_type& get_allocator() const     { return m_allocator; }

            void make_unique(size_type) {}
        
            value_type* get_data() { return m_data; }

        protected:
            bool invariant() const {
                assert(m_data);
                assert(m_length <= m_capacity);
                
                if (length() != 0)
                    assert(m_data[length()] == 0);
                return true;
            }
        private:
            value_type* construct_string(size_type capacity, size_type& out_capacity) {  
                value_type* data(0);

                if (capacity != 0) {
                    capacity = (capacity+kGranularity-1) & ~(kGranularity-1);
                    if (capacity < kGranularity)
                        capacity = kGranularity;

                    size_type toAlloc = sizeof(value_type)*(capacity + 1);
                    void* mem = m_allocator.alloc(toAlloc, __UINT32_MAX__);
                    data = static_cast<value_type*>(mem);
                } else {    // empty string, no allocation needed. Use our internal buffer.
                    data = &m_end_of_data;
                }
            
                out_capacity = capacity;
                *data = 0;
                return data;
            }
            void release_string() {
                if (m_capacity != 0) {
                    assert(m_data != &m_end_of_data);
                    m_allocator.free(m_data. __UINT32_MAX__);
                }
            }
        private:
            E*              m_data;
            E               m_end_of_data;
            size_type       m_capacity;
            size_type       m_length;
            TAllocator      m_allocator;
        };
    }
}

#endif