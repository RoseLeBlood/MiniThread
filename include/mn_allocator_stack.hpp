/*
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
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
#ifndef _MINLIB_ALLOCATOR_STACK_H_
#define _MINLIB_ALLOCATOR_STACK_H_

#include <assert.h>

namespace mn {
    namespace memory {
         /**
         * @brief Stack based allocator.
         * @note - operates on buffer of TBUFFERSIZE bytes of stack memory
         * @note - never frees memory
         * @note - cannot be copied
         * 
         * @author RoseLeBlood
         * @date 2021.02.21
         * @version 1.0
         * 
         * @tparam T                The value to allocate the allocator
         * @tparam TBUFFERSIZE      The size of the buffer
         */
        template<typename T, int TBUFFERSIZE> 
        class basic_allocator_stack {
        public:
            explicit basic_allocator_stack() : m_bufferTop(0)  { }

            T* alloc(unsigned int xTime) {
            if(is_empty()) return NULL;

                assert(m_bufferTop + sizeof(T) <= (TBUFFERSIZE * sizeof(T)) );
                char* ret = &m_aBuffer[0] + m_bufferTop;
                m_bufferTop += sizeof(T);
                return (T*)ret;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, T** buf, unsigned int xTime) {
                if(is_empty()) return 0;
            
                size_t size = n;
                if( (get_max() - get_allocated() < size) ) size = get_free();

                assert(m_bufferTop + (sizeof(T)*size) <= (TBUFFERSIZE * sizeof(T)) );

                *buf = (T*)&m_aBuffer[0] + m_bufferTop;

                m_bufferTop += sizeof(T)*size;

                return size;
            }
            void free(void* ptr) { ptr = NULL; }

            bool is_empty()                 { return get_free() == 0;  }

            unsigned long get_free()        { return get_max() - (m_bufferTop); }
            unsigned long get_allocated()   { return m_bufferTop; }
            unsigned long get_max()         { return (TBUFFERSIZE); }

            basic_allocator_stack(const basic_allocator_stack&) = delete;
            basic_allocator_stack& operator=(const basic_allocator_stack&) = delete;
        private:
            size_t          m_bufferTop;
            char*           m_aBuffer[TBUFFERSIZE];   
        };
    }
}

#endif