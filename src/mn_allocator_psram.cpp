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
#include "mn_allocator_psram.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "esp32/spiram.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"

basic_allocator_spiram::basic_allocator_spiram() {
    m_bFound = false;
    m_bFailed = false;
}
bool basic_allocator_spiram::create() {
    if(m_bFound) return true;

#ifndef CONFIG_SPIRAM_BOOT_INIT
    if(m_bFailed) return false;

    uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
    uint32_t pkg_ver = chip_ver & 0x7;

    if (pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32D2WDQ5 || pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32PICOD2) {
        log_w("PSRAM not supported!");
        return false;
    }
    esp_spiram_init_cache();
    if (esp_spiram_init() != ESP_OK) {
        m_bFailed = true;
        pinMatrixOutDetach(16, false, false);
        pinMatrixOutDetach(17, false, false);
        return false;
    }
    if (!esp_spiram_test()) {
        m_bFailed = true; return false;
    }
    if (esp_spiram_add_to_heapalloc() != ESP_OK) {
        m_bFailed = true; return false;
    }
#endif
    m_bFound = true;
    return true;
}

void* basic_allocator_spiram::alloc(size_t size, unsigned int xTime) {
    if(!m_bFound) return NULL; 

    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}
void* basic_allocator_spiram::calloc(size_t n, size_t size, unsigned int xTime) {
    if(!m_bFound) return NULL; 

    return heap_caps_calloc(n, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}
void* basic_allocator_spiram::realloc(void *ptr, size_t size, unsigned int xTime) {
    if(!m_bFound) return NULL; 

    return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}
bool basic_allocator_spiram::free(void* mem, unsigned int xTime) {
    if(!m_bFound) return false; 
    if(mem == NULL) return false;
    
    heap_caps_free(mem); return true;
}
unsigned int basic_allocator_spiram::get_size() { 
    return heap_caps_get_total_size(MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}
unsigned long basic_allocator_spiram::get_free()  {
    return heap_caps_get_free_size( MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
}

unsigned long basic_allocator_spiram::get_size(void* mem) {
    if(mem == NULL) return 0;

    return heap_caps_get_allocated_size(mem);
}

#endif
