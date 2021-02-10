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
#ifndef _MINLIB_ALLOCATOR_STACK_H_
#define _MINLIB_ALLOCATOR_STACK_H_

#include <assert.h>

/** Stack based allocator.
 * Traits:
 *      - operates on buffer of TBytes bytes of stack memory
 *      - never frees memory
 *      - cannot be copied
 */
template<typename T, int TBytes> 
class basic_allocator_stack {
public:
    explicit basic_allocator_stack() : m_bufferTop(0), m_sSize(sizeof(T)) { }

    bool create(size_t unused = 0) { return true; }

    T* alloc(unsigned int xTime) {
       if(is_empty()) return NULL;

        assert(m_bufferTop + m_sSize <= (TBytes * m_sSize) );
        char* ret = &m_buffer[0] + m_bufferTop;
        m_bufferTop += m_sSize;
        return (T*)ret;
    }
    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    size_t calloc(size_t n, T** buf, unsigned int xTime) {
        if(is_empty()) return 0;
    
        size_t size = n;
        if( (get_max() - get_allocated() < size) ) size = get_free();

        assert(m_bufferTop + (m_sSize*size) <= (TBytes * m_sSize) );
        *buf = (T*)&m_buffer[0] + m_bufferTop;
        m_bufferTop += m_sSize*size;

        return size;
    }
    void free(void* ptr) { ptr = NULL; }

    size_t size()                   { return m_sSize; } ///<Get the size of T
    void size(size_t uiSize)        { if(uiSize <= sizeof(T)) return; m_sSize = uiSize;  } ///<set the size off T. Muss be greater as sizeof(T) 

    bool is_empty()                 { return get_free() == 0;  }

    unsigned long get_free()        { return get_max() - (m_bufferTop/m_sSize); }
    unsigned long get_allocated()   { return m_bufferTop/m_sSize; }
    unsigned long get_max()         { return (TBytes); }

    basic_allocator_stack(const basic_allocator_stack&) = delete;
    basic_allocator_stack& operator=(const basic_allocator_stack&) = delete;
private:
    char            m_buffer[TBytes*sizeof(T)];
    size_t          m_bufferTop;
    size_t          m_sSize;
};


#endif