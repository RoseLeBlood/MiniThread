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
#ifndef MINLIB_b5c55b35_96bb_46c7_b16c_d8c27897626c_H_
#define MINLIB_b5c55b35_96bb_46c7_b16c_d8c27897626c_H_

#include "container/mn_node.hpp"
#include "container/mn_pair.hpp"

#include "container/mn_list.hpp"
#include "container/mn_array.hpp"




namespace mn {
    namespace container {
        /**
         * @brief Node type witch allocated in global heap
         */
        using node = basic_node<memory::default_allocator_t>;

        template<typename TFIRST, typename TSECOND> 
        using pair = basic_pair<TFIRST, TSECOND>;

        /**
         * @brief List type witch allocated in global heap
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using list = basic_list<T, memory::default_allocator_t >;

        /**
         * @brief List type witch allocated on the staked memory buffer
         * 
         * @tparam T The holding type for the value 
         * @tparam TBUFFERSIZE The size of the memory buffer
         */
        template<typename T, int TBUFFERSIZE> 
        using stacked_list = basic_list<T, memory::basic_allocator_stack<TBUFFERSIZE> >;

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32 

        /** @brief Node type witch allocated in SPI-RAM (8-Bit) */
        using cps_node8_t = basic_node<memory::allocator_psram_esp32_t>;
        /** @brief Node type witch allocated in SPI-RAM (32-Bit) */
        using cps_node32_t = basic_node<memory::allocator_psram32_esp32_t>;
        /** @brief Node type witch allocated in internal ram (8-Bit) */
        using cin_node8_t = basic_node<memory::allocator_internal8_esp32_t>;
        /** @brief Node type witch allocated in internal ram (32-Bit) */
        using cin_node32_t = basic_node<memory::allocator_psram32_esp32_t>;
        /** @brief Node type witch allocated on DMA section (32-Bit) */
        using cdma_node32_t = basic_node<memory::allocator_internal_dma_esp32_t>;

        /**
         * @brief  List type with allocated in SPI-RAM (8-Bit) 
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using cps_list8_t = basic_list<T, memory::allocator_psram_esp32_t >;
        /**
         * @brief  List type with allocated in SPI-RAM (32-Bit) 
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using cps_list32_t = basic_list<T, memory::allocator_psram32_esp32_t >;
        /**
         * @brief  List type with allocated in internal ram (8-Bit) 
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using cin_list8_t = basic_list<T, memory::allocator_internal8_esp32_t >;
        /**
         * @brief  List type with allocated in intarnal ram (32-Bit) 
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using cin_list32_t = basic_list<T, memory::allocator_internal32_esp32_t >;
        /**
         * @brief  List type with allocated in DMA section(8-Bit) 
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using cdma_list_t = basic_list<T, memory::allocator_internal_dma_esp32_t >;

        
#endif
        /**
         * @brief Array type 
         * 
         * @tparam T The holding type of value
         * @tparam N The Number of Elements
         */
        template <typename T, size_t N> 
        using array = basic_array<T, N>;

        /**
         * @brief Fixed Array type 
         * 
         * @tparam T The holding type of value
         * @tparam N The Number of Elements
         */
        template <typename T, size_t N> 
        using fixed_array = basic_fixed_array<T, N>;

        

        template<typename TFIRST, typename TSECOND> 
        pair<TFIRST, TSECOND> make_pair(const TFIRST& a, const TSECOND& b) {
            return pair<TFIRST, TSECOND>(a, b);
        }
    }

    template <typename TFIRST, typename TSECOND> 
    struct is_pod< container::pair<TFIRST, TSECOND> > {
        enum { 
            value = ( (is_pod<TFIRST>::value || is_fundamental<TFIRST>::value) && 
                        (is_pod<TSECOND>::value || is_fundamental<TSECOND>::value) )
        };
    };
}

#endif