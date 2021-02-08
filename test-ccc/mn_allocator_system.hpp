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

#undef MN_THREAD_CONFIG_BOARD
#define MN_THREAD_CONFIG_BOARD 3000
#define MN_THREAD_CONFIG_ESP32 1

/**
 * basic wrapper for system malloc
 */ 
template <typename T>
class basic_allocator_system  {
public:
    /**
     * Create the allocator
     * @param [in] maxSize The max size to alloc with this allocator. 
     * @note When maxSize 0 is then use all 
     */ 
    bool create(size_t maxSize)  { 
        m_sMaxSize = (maxSize); m_sAlloced = (0);
        return true; 
    }
    /**
     * Get the size of ram
     * @return The size of bytes - works only on esp32
     */ 
    unsigned long get_size() {
        if(m_sMaxSize == 0) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
                return heap_caps_get_total_size( MALLOC_CAP_INTERNAL);
#else 
                return 2048;
#endif
        } else {
                return m_sMaxSize;
        }
    }

    /**
     * Get the size of free bytes
     * @return The size of free bytes
     */ 
    unsigned long get_free() {
        if(m_sMaxSize == 0) {
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
                return heap_caps_get_free_size( MALLOC_CAP_INTERNAL );
#else 
                return 2048;
#endif
        } else {
               return m_sMaxSize - m_sAlloced; 
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
        T* buf = NULL;
        
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        buf = (T*)heap_caps_malloc(sizeof(T),  MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        buf = (T*)::malloc(sizeof(T));
#endif
        if(buf) { m_sAlloced += sizeof(T); }
        return buf;
    }
    
    T* alloc_range(size_t n, unsigned int xTime) {
        if(is_empty() ) return NULL; 

        T* buf = NULL;
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        buf = (T*)heap_caps_malloc( sizeof(T) * n , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        buf = (T*)::malloc(sizeof(T) * n);
#endif
        if(buf) { m_sAlloced += sizeof(T) * n; }
        return buf;
    }
    void* alloc_raw(size_t size, size_t n, unsigned int xTime ) {
         if(is_empty() ) return NULL;
        T* buf = NULL;

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        buf = heap_caps_malloc( size * n , MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
        buf = ::malloc(size * n);
#endif
        if(buf) { m_sAlloced += size * n; }
        return buf;
    }

    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    T* calloc(size_t n,  unsigned int xTime) {
        if(is_empty() ) return NULL;
        T* buf = NULL;

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        buf = (T*)heap_caps_calloc(n, sizeof(T), MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        buf = (T*)::calloc(n, sizeof(T));
#endif
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
        T* buf = NULL;
        
#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        buf = (T*)heap_caps_realloc(ptr, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else 
        buf = (T*)::realloc(ptr, size);
#endif
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
    unsigned long get_size(T* mem) {
        if(mem == NULL) return 0;
        return heap_caps_get_allocated_size(mem);
    }
#endif
private:
   size_t m_sMaxSize;
   size_t m_sAlloced;
};



#endif