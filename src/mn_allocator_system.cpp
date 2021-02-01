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
#include "mn_allocator_system.hpp"
#include <iostream>

#if MN_THREAD_CONFIG_BOARD !=  MN_THREAD_CONFIG_ESP32

unsigned int basic_allocator_system::get_size() { 
    return 2048;
}
unsigned long basic_allocator_system::get_free() {
    return 2048;
}

void* basic_allocator_system::alloc(size_t size, unsigned int xTime) {
    return ::malloc(size);
}
void* basic_allocator_system::calloc(size_t n, size_t size, unsigned int xTime) {
    return ::calloc(n, size);
}
void* basic_allocator_system::realloc(void *ptr, size_t size, unsigned int xTime) {
    return ::realloc(ptr, size);
}
bool basic_allocator_system::free(void* mem, unsigned int xTime) {
    if(mem == NULL) return false;
    
    ::free(mem); return true;
}
#else 

#include "esp_heap_caps.h"

unsigned long basic_allocator_system::get_size() { 
    return heap_caps_get_total_size( MALLOC_CAP_INTERNAL);
}
unsigned long basic_allocator_system::get_free() {
    return heap_caps_get_free_size( MALLOC_CAP_INTERNAL );
}

void* basic_allocator_system::alloc(size_t size, unsigned int xTime) {
    return heap_caps_malloc(size,  MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}
void* basic_allocator_system::calloc(size_t n, size_t size, unsigned int xTime) {
    return heap_caps_calloc(n, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}
void* basic_allocator_system::realloc(void *ptr, size_t size, unsigned int xTime) {
    return heap_caps_realloc(ptr, size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}
bool basic_allocator_system::free(void* mem, unsigned int xTime) {
    if(mem == NULL) return false;
    
    heap_caps_free(mem); return true; 
}
unsigned long basic_allocator_system::get_size(void* mem) {
    if(mem == NULL) return 0;

    return heap_caps_get_allocated_size(mem);
}

#endif