/** 
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
#ifndef _MINLIB_ALLOCATOR_INTERFACE_H_
#define _MINLIB_ALLOCATOR_INTERFACE_H_

#include <cassert>
#include <stddef.h>

namespace mn {
    namespace memory {
        /**
         * @brief A basic abstractet interface for all allocator in the library
         */
        class allocator_interface {
        public:
            /**
             * @brief Construct a new allocator interface object, with no limit
             */
            allocator_interface() 
                : m_szeMaxSize(0), m_szeAllocted(0) { }
            /**
             * @brief Construct a new allocator interface object, with limit
             * 
             * @param maxSize The limit of bytes to use, when 0 then use no limit
             */
            allocator_interface(size_t maxSize) 
                : m_szeMaxSize(maxSize), m_szeAllocted(0) { }

            /**
             * @brief Alloc a n size of bytes
             * 
             * @param n       The size of bytes to alloc
             * @param xTime   How long to wait to get the Lock until giving up. (default = 0xffffffffUL)
             * @return The allocated memory from heap
             */
            virtual void* alloc(size_t n, unsigned int xTime = __UINT32_MAX__) = 0;
            /**
             * @brief 
             * 
             * @param n 
             * @param b 
             * @param buf 
             * @param xTime 
             * @return size_t 
             */
            virtual size_t calloc(size_t n, size_t b, void** buf, unsigned int xTime = __UINT32_MAX__) = 0;
            virtual void free(void* mem, unsigned int xTime) = 0;


            virtual void* alloc_alignment(size_t n, size_t alignment) { 
                void* buf = NULL; 
            
                if((alignment <= (sizeof(char*)*2) ) ) {
                    buf = alloc(n, __UINT32_MAX__);
                }
                return buf;
            }
            /**
             * @brief Set the limit of bytes to use
             * @param maxSize The limit of bytes to use, when 0 then use no limit 
             */
            virtual void set_limit(size_t maxSize) { m_szeMaxSize = maxSize; }
            /**
             * @brief Get the limit of bytes to use
             * @return The limit of bytes to use, when 0 then use no limit 
             */
            size_t limit()  { return m_szeMaxSize; }
            /**
             * @brief Have the allocator a limit?
             * 
             * @return true The allocator is limited
             * @return false The allocator have no limit
             */
            bool have_limit() { return m_szeMaxSize != 0; }
            /**
             * @brief In the heap free
             * 
             * @return true The heap is free
             * @return false The limit is erreicht
             */
            bool is_free() { return have_limit() ? m_szeAllocted >=  m_szeMaxSize : true; }
            bool is_free(size_t mSize) { return have_limit() ? (m_szeAllocted + mSize) >=  m_szeMaxSize : true; }
            
            size_t get_allocated() { return m_szeAllocted; }

            template<typename T> 
            T* alloc(size_t n, unsigned int xTime) { return static_cast<T*>(alloc(n, xTime)); }

            template<typename T> 
            size_t max_size() const { return (sizeof(-1) / sizeof(T)) ; }

            template <typename T, typename TARG> 
            T* construct(size_t size, TARG arg, unsigned int xTime = __UINT32_MAX__) {
                void* buf = alloc(size, __UINT32_MAX__);
                return new (buf) T(arg);
            }
            template <typename T>
            void deconstruct(T* pObject) { pObject->~T(); }

        protected:
            void add_allocatedsize  (size_t size) { m_szeAllocted += size;}
            void rm_allocatedsize  (size_t size)  { m_szeAllocted -= size;}
        protected:
            size_t m_szeMaxSize;
            size_t m_szeAllocted;
        };
    }
}

#endif