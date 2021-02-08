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


#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        #include "mn_allocator_caps.hpp"
        
        template <typename T>
        using allocator_internal_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::Internal, cap_allocator_size::Size8Bit>;

        template <typename T>
        using allocator_psram_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::SpiRam, cap_allocator_size::Size8Bit>;


        template <typename T>
        using allocator_internal32_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::Internal, cap_allocator_size::Size32Bit>;

        template <typename T>
        using allocator_psram32_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::SpiRam, cap_allocator_size::Size32Bit>;
        #endif

        template <typename T>
        using allocator_internal_dma_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::DMA, cap_allocator_size::Size32Bit>;

        template <typename T>
        using allocator_psram_dma_esp32_t = 
            basic_cap_allocator_esp32<T, cap_allocator_map::DMA, cap_allocator_size::Size32Bit>;
        #endif
#endif // MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32


#if MN_THREAD_CONFIG_ALLOCATOR_DEFAULT == MN_THREAD_CONFIG_ALLOCATOR_SYSTEM
    template <typename T> using default_allocator_t = basic_allocator_system<T>;
#else
    template <typename T> using default_allocator_t = basic_cap_allocator_esp32<T>;
#endif
