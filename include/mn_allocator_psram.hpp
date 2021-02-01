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
#ifndef _MINLIB_ALLOCATOR_ESP32_SPIRAM_H_
#define _MINLIB_ALLOCATOR_ESP32_SPIRAM_H_



#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "mn_allocator_interface.hpp"

/**
 * Allocator for the esp32 spiram  
 */ 
class basic_allocator_spiram : public basic_allocator_interface {
public:
    basic_allocator_spiram();
    /**
     * Create the allocator 
     * @return True when created and found the spiram or false when not
     */ 
    virtual bool create();

    /**
     * Get the total size of SPIRAM 
     * @return total size in bytes
     */ 
    virtual unsigned int get_size();

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

    /**
     * @brief Return the size that a particular pointer was allocated with.
     *
     * @param ptr Pointer to currently allocated heap memory. Must be a pointer value previously 
     * returned by heap_caps_malloc,malloc,calloc, etc. and not yet freed.
     *
     * @return Size of the memory allocated at this block and 0 when mem are NULL is
     */
    virtual unsigned long get_size(void* mem);
private:
    bool m_bFound;
#ifndef CONFIG_SPIRAM_BOOT_INIT
    bool m_bFailed;
#endif
};


#endif

#endif