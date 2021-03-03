/**
*
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.
*
* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_b5c55b35_96bb_46c7_b16c_d8c27897626c_ESP32_H_
#define MINLIB_b5c55b35_96bb_46c7_b16c_d8c27897626c_ESP32_H_

#include "mn_container.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32 
namespace mn {
    namespace container {
// NODE
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

// LISTS
        /** @brief  List type with allocated in SPI-RAM (8-Bit)   */
        template<typename T > 
        using cps_list8_t = basic_list<T, memory::allocator_psram_esp32_t >;

        /** @brief  List type with allocated in SPI-RAM (32-Bit)  */
        template<typename T > 
        using cps_list32_t = basic_list<T, memory::allocator_psram32_esp32_t >;

        /** @brief  List type with allocated in internal ram (8-Bit) */
        template<typename T > 
        using cin_list8_t = basic_list<T, memory::allocator_internal8_esp32_t >;

        /** @brief  List type with allocated in intarnal ram (32-Bit) */
        template<typename T > 
        using cin_list32_t = basic_list<T, memory::allocator_internal32_esp32_t >;

        /** @brief  List type with allocated in DMA section(8-Bit) */
        template<typename T > 
        using cdma_list_t = basic_list<T, memory::allocator_internal_dma_esp32_t >;

// RB TREE
        /** @brief  List type with allocated in SPI-RAM (8-Bit) */
        template<typename T > 
        using cps_rb_tree8_t = basic_rb_tree<T, memory::allocator_psram_esp32_t >;

        /** @brief  List type with allocated in SPI-RAM (32-Bit) */
        template<typename T > 
        using cps_rb_tree32_t = basic_rb_tree<T, memory::allocator_psram32_esp32_t >;

        /** @brief  List type with allocated in internal ram (8-Bit) */
        template<typename T > 
        using cin_rb_tree8_t = basic_rb_tree<T, memory::allocator_internal8_esp32_t >;

        /** @brief  List type with allocated in intarnal ram (32-Bit) */
        template<typename T > 
        using cin_rb_tree32_t = basic_rb_tree<T, memory::allocator_internal32_esp32_t >;

        /** @brief  List type with allocated in DMA section(8-Bit) */
        template<typename T > 
        using cdma_rb_tree_t = basic_rb_tree<T, memory::allocator_internal_dma_esp32_t >;
    }
}
#endif

#endif