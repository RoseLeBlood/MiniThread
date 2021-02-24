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
#ifndef _MINLIB_ALLOCATOR_MULTI_HEAP_H_
#define _MINLIB_ALLOCATOR_MULTI_HEAP_H_

#include "mn_config.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "multi_heap.h"
#include "mn_allocator_interface.hpp"

namespace mn {
    namespace memory {
        
        template<size_t TBytes> 
        class basic_multiheap_allocator_esp32 : public allocator_interface {
        public:
            basic_multiheap_allocator_esp32() : allocator_interface(TBytes)  { 
                m_pHandle = multi_heap_register(m_buffer, TBytes);
            }

            void* alloc(size_t size, unsigned int xTime) {
                void* buffer = NULL;

                if(is_free(size)) {
                    buffer = multi_heap_malloc(m_pHandle, size ); 
                }
                if(buffer) add_allocatedsize(size);
                return buffer;
            }
            /**
             * @brief 
             * 
             * @param [in] n  How manf elements to allocated
             * @param [out] buf The allocated buffer 
             * @param xTime How long to wait to given up 
             * @return How many elements are real allocated
             */
            size_t calloc(size_t n, size_t size, void** buf, unsigned int xTime) {
                size_t _size = n * size;

                if(is_free(_size)) {
                    *buf = (void*) multi_heap_malloc(m_pHandle, _size );
                }
                if(*buf) add_allocatedsize(_size);
                return _size;
            }
            void free(void* ptr) {
                multi_heap_free(m_pHandle, ptr);
                rm_allocatedsize(sizeof(ptr));
            }

            basic_multiheap_allocator_esp32(const basic_multiheap_allocator_esp32&) = delete;
            basic_multiheap_allocator_esp32& operator=(const basic_multiheap_allocator_esp32&) = delete;
        private:
            char                    m_buffer[TBytes];
            multi_heap_handle_t     m_pHandle;
        };
    }
}



#endif

#endif