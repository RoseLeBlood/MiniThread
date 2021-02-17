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

#include <assert.h>

namespace mn {
    namespace memory {
        
        template <typename T, int sMaxSize = 0>
        class basic_allocator_system {
        public:
            basic_allocator_system() : m_sMaxSize(sMaxSize), m_sAlloced(0) { }
            
            /**
             * Allocate SIZE bytes of memory 
             * @return A pointer of the allocated ram
             */ 
            T* alloc(unsigned int xTime) {
                if(is_empty() ) return NULL;
                T* buf = (T*)::malloc(sizeof(T));
                assert(buf != NULL);

                if(buf) { m_sAlloced++; }
                return buf;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, T** buf, unsigned int xTime) {
                if(is_empty() ) {  buf = NULL; return 0; }

                size_t size = n;

                if(m_sMaxSize != 0) {
                    if(m_sMaxSize < size || get_free() < size) size = get_free();
                }
                *buf = (T*)::calloc(size, sizeof(T));

                assert(buf != NULL);
                m_sAlloced += size; 
            
                return size;
            }
            /**
             * Free a block allocated by `malloc', `realloc' or `calloc'. 
             */ 
            void free(T* mem, unsigned int xTime) {
                if(mem == NULL) return;
                ::free(mem); 
                m_sAlloced--;
            }

            /**
             * Get the size of free bytes
             * @return The size of free bytes
             */ 
            unsigned long get_free()        { return (m_sMaxSize != 0) ? m_sMaxSize - m_sAlloced : __LONG_MAX__; }
            unsigned long get_allocated()   { return m_sAlloced; }
            unsigned long get_max()         { return (m_sMaxSize != 0) ? m_sMaxSize : __LONG_MAX__; }

            size_t size()                   { return m_sSize; } ///<Get the size of T
            void size(size_t uiSize)        { if(uiSize <= sizeof(T)) return; m_sSize = uiSize;  } ///<set the size off T. Muss be greater as sizeof(T) 

            bool is_empty()                 { return (m_sMaxSize != 0) ? get_free() == 0 : false;  }
        private:
            size_t m_sMaxSize;
            size_t m_sAlloced;
            size_t m_sSize;
        };
    }
}


#endif