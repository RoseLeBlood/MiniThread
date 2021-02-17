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

namespace mn {
    namespace memory {
        
        template<typename T, unsigned long TBytes> 
        class basic_multiheap_allocator_esp32 {
        public:
            basic_multiheap_allocator_esp32()
                : m_pHandle(NULL) { 
                if(m_pHandle == NULL) {
                    m_pHandle = multi_heap_register(m_buffer, TBytes);
                }
            }
            basic_multiheap_allocator_esp32(char* buffer, int size) {
                 if(m_pHandle == NULL) {
                    m_pHandle = multi_heap_register(buffer, size);
                }
            }

            T* alloc(unsigned int xTime) {
            
                return (T*) multi_heap_malloc(m_pHandle, sizeof(T) );
            }
            /**
             * @brief 
             * 
             * @param [in] n  How manf elements to allocated
             * @param [out] buf The allocated buffer 
             * @param xTime How long to wait to given up 
             * @return How many elements are real allocated
             */
            size_t calloc(size_t n, T** buf, unsigned int xTime) {
                if(is_empty()) return 0;

                size_t size = n * sizeof(T);
                if( get_free() < size) size = get_free();

                if(size != 0) {
                    *buf = (T*) multi_heap_malloc(m_pHandle, size );
                    size = size / sizeof(T);
                }
                return 0;
            }
            void free(void* ptr) {
                multi_heap_free(m_pHandle, ptr);
            }

            bool is_empty() { 
                return get_free() == 0;  
            }

            unsigned long get_free() { 
                multi_heap_info_t info;
                multi_heap_get_info(m_pHandle, &info);

                return info.free_blocks ; 
            }
            unsigned long get_allocated() { 
                multi_heap_info_t info;
                multi_heap_get_info(m_pHandle, &info);

                return info.allocated_blocks ; 
            }   
            unsigned long get_max() { 
                return TBytes; 
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