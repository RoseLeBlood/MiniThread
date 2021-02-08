/*
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2021 Amber-Sophia Schroeck
* 
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.  
*/
#ifndef _MINLIB_ALLOCATOR_SYSTEM_H_
#define _MINLIB_ALLOCATOR_SYSTEM_H_

/**
 * basic wrapper for system malloc
 */ 
template <typename T>
class basic_allocator_system {
public:
    /**
     * Create the allocator
     * @param [in] maxSize The max size to alloc with this allocator. 
     * @note When maxSize 0 is then use all 
     */ 
    bool create(size_t maxSize) : m_sMaxSize(maxSize), m_sAlloced(0) { 
        return true; 
    }
    /**
     * Get the size of T
     * @return The size of T
     */
    size_t size() {
        return m_sSize;
    }
    /**
     * set the size off T. Muss be greater as sizeof(T) 
     */ 
    void size(size_t uiSize) {
        if(uiSize <= sizeof(T)) return;
        m_sSize = uiSize;
    }

    /**
     * Get the size of ram
     * @return The size of bytes - works only on esp32
     */ 
    unsigned long get_size() {
        if(m_sMaxSize == 0) return __LONG_MAX__;
        return m_sMaxSize;
    }

    /**
     * Get the size of free bytes
     * @return The size of free bytes
     */ 
    unsigned long get_free() {
        if(m_sMaxSize == 0) {
           	return __LONG_MAX__  - m_sAlloced;
        } else {
        	return m_maxSize - m_sAlloced; 
        }
    }
    bool is_empty() {
        if(m_sMaxSize == 0) {
            return get_free() == 0;
        } else {
            return false;
        }
    }

    /**
     * Allocate SIZE bytes of memory 
     * @return A pointer of the allocated ram
     */ 
    T* alloc(unsigned int xTime) {
        if(is_empty() ) return NULL;
        T* buf = (T*)::malloc(sizeof(T));

        if(buf) { m_sAlloced += sizeof(T); }
        return buf;
    }
    
    T* alloc_range(size_t n, unsigned int xTime) {
        if(is_empty() ) return NULL; 

        T* buf = (T*)::malloc(sizeof(T) * n);
        if(buf) { m_sAlloced += sizeof(T) * n; }
        return buf;
    }
    void* alloc_raw(size_t size, size_t n, unsigned int xTime ) {
         if(is_empty() ) return NULL;
        T* buf = ::malloc(size * n);
        if(buf) { m_sAlloced += size * n; }
        return buf;
    }

    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    T* calloc(size_t n,  unsigned int xTime) {
        if(is_empty() ) return NULL;
        T* buf = (T*)::calloc(n, sizeof(T));
        if(buf) { m_sAlloced += sizeof(T); }
        return buf;
    }

    /**
     * Re-allocate the previously allocated block in PTR, making the new
     * block large enough for NMEMB elements of SIZE bytes each.
     * @return A pointer of the re-allocated ram
     */ 
    T* realloc(T *ptr, size_t size, unsigned int xTime) {
        if(is_empty() ) return NULL;
        buf = (T*)::realloc(ptr, size);
        if(buf) { m_sAlloced = (m_sAlloced - sizeof(T)) + size; }
        return buf;
    }

    /**
     * Free a block allocated by `malloc', `realloc' or `calloc'. 
     * @return True the mem are free and false when not
     */ 
    bool free(T* mem, unsigned int xTime) {
        if(mem == NULL) return false;
        m_sAlloced = (m_sAlloced - sizeof(T));
        ::free(mem); return true;
    }
private:
   size_t m_sMaxSize;
   size_t m_sAlloced;
};



#endif