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

#ifndef _MINLIB_ALLOCATOR_H_
#define _MINLIB_ALLOCATOR_H_

#include "mn_allocator_system.hpp"
#include "mn_allocator_stack.hpp"
#include "mn_allocator_buffer.hpp"

#include "mn_allocator_caps.hpp"
#include "mn_allactor_multiheap.hpp"

namespace mn {
    namespace memory {

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32     
        template <typename T>
        using allocator_internal_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::Default, cap_allocator_size::Size8Bit>;

        template <typename T>
        using allocator_psram_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::SpiRam, cap_allocator_size::Size8Bit>;


        template <typename T>
        using allocator_internal32_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::Default, cap_allocator_size::Size32Bit>;

        template <typename T>
        using allocator_psram32_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::SpiRam, cap_allocator_size::Size32Bit>;

        template <typename T>
        using allocator_internal_dma_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::DMA, cap_allocator_size::Size32Bit>;

        template <typename T>
        using allocator_psram_dma_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::DMA, cap_allocator_size::Size32Bit>;
#endif // MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32


#if MN_THREAD_CONFIG_ALLOCATOR_DEFAULT == MN_THREAD_CONFIG_ALLOCATOR_SYSTEM
        template <typename T> using default_allocator_t = basic_allocator_system<T>;
#else
        template <typename T> using default_allocator_t = basic_cap_allocator_esp32<T>;
#endif

        template<typename T, int TBytes> 
        using allocator_stack_t = basic_allocator_stack<T, TBytes>;

        template<typename T> 
        using allocator_buffer_t = basic_allocator_buffer<T, default_allocator_t<T> >;


#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32  
        template<typename T> 
        using multiheap_allocator_esp32_t = basic_multiheap_allocator_esp32<T, default_allocator_t<T>>;
#endif
    }
}