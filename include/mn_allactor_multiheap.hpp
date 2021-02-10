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


template<typename T, class TALLOC> 
class basic_multiheap_allocator_esp32 {
public:
    basic_multiheap_allocator_esp32()
        : m_pHandle(NULL), m_sSize(sizeof(T)) { }

    bool create(size_t unused = 0) { 
        if(m_pHandle == NULL) {
            m_pHandle = multi_heap_register(m_buffer, sizeof(m_buffer));
        }
        return m_pHandle != NULL; 
    }

    T* alloc(unsigned int xTime) {
    
        return (T*) multi_heap_malloc(m_pHandle, m_sSize);
    }
    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    size_t calloc(size_t n, T** buf, unsigned int xTime) {
        if(is_empty()) return 0;
    
    }
    void free(void* ptr) {
        multi_heap_free(m_pHandle, ptr);
    }

    size_t size() { 
        return m_sSize; 
    } 
    void size(size_t uiSize) { 
        if(uiSize <= sizeof(T)) return; m_sSize = uiSize;  
    } 
    bool is_empty() { 
        return get_free() == 0;  
    }

    unsigned long get_free() { 
        multi_heap_info_t info;
        multi_heap_get_info(m_pHandle, &info);

        return info.free_blocks / m_sSize; 
    }
    unsigned long get_allocated() { 
        multi_heap_info_t info;
        multi_heap_get_info(m_pHandle, &info);

        return info.allocated_blocks / m_sSize; 
    }   
    unsigned long get_max() { 
        return (TBytes*sizeof(T))/m_sSize; 
    }

    basic_multiheap_allocator_esp32(const basic_multiheap_allocator_esp32&) = delete;
    basic_multiheap_allocator_esp32& operator=(const basic_multiheap_allocator_esp32&) = delete;
private:
    bool internal_create() {
        
    }
private:
    char                    m_buffer[TBytes*sizeof(T)];
    multi_heap_handle_t     m_pHandle;
    size_t                  m_sSize;
};



#endif

#endif