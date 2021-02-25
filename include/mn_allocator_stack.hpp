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

#include "mn_allocator_interface.hpp"

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
         */
        template <int TBUFFERSIZE>
        class basic_allocator_stack : public allocator_interface  {
        public:
            explicit basic_allocator_stack() 
                : allocator_interface(TBUFFERSIZE), m_bufferTop(0)  { }

            void* alloc(size_t size, unsigned int xTime) {
                if(is_free(size)) { 
                    char* ret = &m_aBuffer[0] + m_bufferTop;
                    m_bufferTop += size;
                    add_allocatedsize(size);
                    return (void*)ret;
                }
                return NULL;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, size_t b, void** buf, unsigned int xTime) {
                size_t size = n*b;

                if(is_free(size)) { 
                    *buf = (void*)&m_aBuffer[0] + m_bufferTop;
                    m_bufferTop += size;
                    add_allocatedsize(size);
                    return size;
                }
                return 0;
            }
            void free(void* ptr)            { ptr = NULL; }

            basic_allocator_stack(const basic_allocator_stack&) = delete;
            basic_allocator_stack& operator=(const basic_allocator_stack&) = delete;

            /**
             * @brief Set the limit of bytes to use
             * @param maxSize The limit of bytes to use, when 0 then use no limit 
             */
            virtual void set_limit(size_t) {  }
        private:
            size_t          m_bufferTop;
            char*           m_aBuffer[TBUFFERSIZE];   
        };
    }
}

#endif