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

#include "mn_allocator_interface.hpp"
#include "malloc.h"

namespace mn {
    namespace memory {

        class basic_allocator_system : public allocator_interface{
        public:
            /**
             * Allocate SIZE bytes of memory 
             * @return A pointer of the allocated ram
             */ 
            void* alloc(size_t n, unsigned int xTime = __UINT32_MAX__) override {
                void* buf = NULL;

                if(is_free(n)) {
                    buf = ::malloc(n);
                    assert(buf != NULL);
                    add_allocatedsize(n);
                }
                return buf;
            }
            /**
             * Allocate n elements of SIZE bytes each, all initialized to 0. 
             * @return A pointer of the allocated mem
             */ 
            size_t calloc(size_t n, size_t b, void** buf, unsigned int xTime = __UINT32_MAX__) override {
                size_t _size = n*b;

                if(is_free(_size)) {
                    *buf = (void*)::calloc(n, b);
                    assert(buf != NULL);
                    add_allocatedsize(_size);
                }
                return sizeof(*buf);
            }
            /**
             * Free a block allocated by `malloc', `realloc' or `calloc'. 
             */ 
            void free(void* mem, unsigned int xTime = __UINT32_MAX__) override {
                ::free(mem); rm_allocatedsize(sizeof(mem));
            }
        };
    }
}


#endif