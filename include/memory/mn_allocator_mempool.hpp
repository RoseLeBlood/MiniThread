/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef _MINLIB_MEMPOOL_ALLOCATOR_H_
#define _MINLIB_MEMPOOL_ALLOCATOR_H_

namespace mn {
    namespace memory {
        /**
         * Wrapper class to use a mempool as allocator
         */ 
        template<typename T, class TMEMPOOL> 
        class basic_allocator_mempool {
        public:
            using self_type = basic_allocator_mempool<T, TMEMPOOL>;
            using value_type = T;
            using pointer = T*;
            using mempool_type = TMEMPOOL;

            basic_allocator_mempool() : m_sSize(sizeof(T)) { }
            /**
             * Create the allocator
             * @param [in] sMaxSize The max elements are allocated with this allocater
             */ 
            bool create(size_t unused = 0)  { 
                int _ret = m_Mempool.create(portMAX_DELAY);
                if(_ret == ERR_MEMPOOL_OK || _ret == ERR_MEMPOOL_MIN) {
                    m_sMaxSize = m_Mempool.size();
                    return true;
                }
                return false; 
            }
            /**
             * Allocate SIZE bytes of memory 
             * @return A pointer of the allocated ram
             */ 
            pointer alloc(unsigned int xTime) {
                if(is_empty()) return 0;

                T* buf = (T*)m_Mempool.alloc(m_sSize);
                assert(buf != NULL);

                return buf;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated ram
             */ 
            size_t calloc(size_t n, pointer* buf, unsigned int xTime) {
                if(is_empty()) return 0;

                size_t size = n;
                if(m_sMaxSize < size || get_free() < size) size = get_free();
                
                *buf = (pointer)m_Mempool.alloc(m_sSize * size);
                assert(buf != NULL);
                
                return size;
            }
            /**
             * Free a block allocated by `malloc', `realloc' or `calloc'. 
             */ 
            void free(pointer mem, unsigned int xTime) {
                if(mem == NULL) return;
                m_Mempool.free(mem);
            }

            bool add_memory(TType** address, size_t sSizeOf) {
                return m_Mempool.add_memory(address, sSizeOf);
            }
            bool add_memory(size_t nElements) {
                return m_Mempool.add_memory(nElements);
            }

            /**
             * Get the size of free bytes
             * @return The size of free bytes
             */ 
            unsigned long get_free()        { return m_Mempool.get_free() * m_sSize; }
            unsigned long get_allocated()   { return m_Mempool.get_used() * m_sSize; }
            unsigned long get_max()         { return m_Mempool.size() * m_sSize; }

            size_t size()                   { return m_sSize; } ///<Get the size of T
            void size(size_t uiSize)        { if(uiSize <= sizeof(T)) return; m_sSize = uiSize;  } ///<set the size off T. Muss be greater as sizeof(T) 
            bool is_empty()                 { return m_Mempool.is_empty();  }

            mempool_type& get_pool()        { return m_Mempool; }

            operator bool() { return !is_empty(); }
        private:
            mempool_type m_Mempool;

            size_t m_sMaxSize;
            size_t m_sSize;
        };

        
    }
}

#endif