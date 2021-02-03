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
#ifndef _MINLIB_ALLOCATOR_SYSTEM_H_
#define _MINLIB_ALLOCATOR_SYSTEM_H_

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
#include "esp_heap_caps.h"
#endif

#include "mn_allocator_interface.hpp"
/**
 * basic wrapper for system malloc
 */ 
template <typename T>
class basic_allocator_system : public basic_allocator_interface<T> {
public:
    /**
     * Get the size of ram
     * @return The size of bytes - works only on esp32
     */ 
    virtual unsigned long get_size() {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return heap_caps_get_total_size( MALLOC_CAP_INTERNAL);
#else 
        return 2048;
#endif
    }

    /**
     * Get the size of free bytes
     * @return The size of free bytes
     */ 
    virtual unsigned long get_free() {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return heap_caps_get_free_size( MALLOC_CAP_INTERNAL );
#else 
        return 2048;
#endif
    }

    /**
     * Allocate SIZE bytes of memory 
     * @return A pointer of the allocated ram
     */ 
    virtual T* alloc(unsigned int xTime) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return (T*)heap_caps_malloc(sizeof(T),  MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        return (T*)::malloc(sizeof(T));
#endif
    }
    
    virtual T* alloc_range(size_t n, unsigned int xTime) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return (T*)heap_caps_malloc( sizeof(T) * n , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        return (T*)::malloc(sizeof(T) * n);
#endif
    }
    virtual void* alloc_raw(size_t size, size_t n, unsigned int xTime ) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return heap_caps_malloc( size * n , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
        return ::malloc(size * n);
#endif
    }

    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    virtual T* calloc(size_t n,  unsigned int xTime) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return (T*)heap_caps_calloc(n, sizeof(T), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        return (T*)::calloc(n, sizeof(T));
#endif
    }

    /**
     * Re-allocate the previously allocated block in PTR, making the new
     * block large enough for NMEMB elements of SIZE bytes each.
     * @return A pointer of the re-allocated ram
     */ 
    virtual T* realloc(T *ptr, size_t size, unsigned int xTime) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        return (T*)heap_caps_realloc(ptr, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        return (T*)::realloc(ptr, size);
#endif
    }

    /**
     * Free a block allocated by `malloc', `realloc' or `calloc'. 
     * @return True the mem are free and false when not
     */ 
    virtual bool free(T* mem, unsigned int xTime) {
        if(mem == NULL) return false;
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        heap_caps_free(mem); return true; 
#else 
        ::free(mem); return true;
#endif
    }


#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
    /**
     * @brief Return the size that a particular pointer was allocated with.
     *
     * @param ptr Pointer to currently allocated heap memory. Must be a pointer value previously 
     * returned by heap_caps_malloc,malloc,calloc, etc. and not yet freed.
     *
     * @return Size of the memory allocated at this block and 0 when mem are NULL is
     */
    virtual unsigned long get_size(T* mem) {
        if(mem == NULL) return 0;
        return heap_caps_get_allocated_size(mem);
    }
#endif
};



#endif