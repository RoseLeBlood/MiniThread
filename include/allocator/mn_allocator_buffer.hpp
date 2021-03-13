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
#ifndef _MINLIB_ALLOCATOR_BUFFER1_H_
#define _MINLIB_ALLOCATOR_BUFFER1_H_

#include "mn_allocator_interface.hpp"

namespace mn {
    namespace memory {
        /** 
         * @brief buffer based allocator.
         * @note - never frees memory
         * @note - cannot be copied
         * 
         * @author RoseLeBlood
         * @date 2021.02.21
         * @version 1.0
         */
        class basic_allocator_buffer : public allocator_interface {
        public:
            basic_allocator_buffer(char* TBUFFER, unsigned long TSIZE) 
                : allocator_interface(TSIZE), m_bufferTop(0), m_BufferSize(TSIZE), m_buffer(TBUFFER)  {  }

            void* alloc(size_t size, unsigned int xTime) {
                char* ret = NULL;

                if(is_free(size)) { 
                    assert(m_bufferTop + size <= (m_BufferSize ) );
                    ret = &m_buffer[0] + m_bufferTop;
                    m_bufferTop += size;
                    add_allocatedsize(size);
                }
                return (void*)ret;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, size_t size, void** buf, unsigned int xTime) {
                size_t index = (size*n);

                if(is_free(index)) {  
                    
                    assert(m_bufferTop + index <= (m_BufferSize ) );
                    char *_buf = (char*)&m_buffer[0] + m_bufferTop;
                    m_bufferTop += index;
                    add_allocatedsize(index);
                    *buf = _buf;

                    return size;
                }
                return 0;
            }
            void free(void*) {  }

            basic_allocator_buffer(const basic_allocator_buffer&) = delete;
            basic_allocator_buffer& operator=(const basic_allocator_buffer&) = delete;

            /**
             * @brief Set the limit of bytes to use
             * @param maxSize The limit of bytes to use, when 0 then use no limit 
             */
            virtual void set_limit(size_t) {  }
        private:
            size_t          m_bufferTop;
            size_t          m_BufferSize;
            char            *m_buffer;
        };
    }
}


#endif