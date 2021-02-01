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

#include "mn_allocator_interface.hpp"
/**
 * basic wrapper for system malloc
 */ 
class basic_allocator_system : public basic_allocator_interface {
public:
    /**
     * Get the size of ram
     * @return The size of bytes - works only on esp32
     */ 
    virtual unsigned long get_size();

    /**
     * Get the size of free bytes
     * @return The size of free bytes
     */ 
    virtual unsigned long get_free();

    /**
     * Allocate SIZE bytes of memory 
     * @return A pointer of the allocated ram
     */ 
    virtual void* alloc(size_t size, unsigned int xTime);
    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    virtual void* calloc(size_t n, size_t size, unsigned int xTime);
    /**
     * Re-allocate the previously allocated block in PTR, making the new
     * block large enough for NMEMB elements of SIZE bytes each.
     * @return A pointer of the re-allocated ram
     */ 
    virtual void* realloc(void *ptr, size_t size, unsigned int xTime);
    /**
     * Free a block allocated by `malloc', `realloc' or `calloc'. 
     * @return True the mem are free and false when not
     */ 
    virtual bool free(void* mem, unsigned int xTime);

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
    /**
     * @brief Return the size that a particular pointer was allocated with.
     *
     * @param ptr Pointer to currently allocated heap memory. Must be a pointer value previously 
     * returned by heap_caps_malloc,malloc,calloc, etc. and not yet freed.
     *
     * @return Size of the memory allocated at this block and 0 when mem are NULL is
     */
    virtual unsigned long get_size(void* mem);
#endif
};



#endif