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
#ifndef _MINLIB_ALLOCATOR_ESP32_SPIRAM_H_
#define _MINLIB_ALLOCATOR_ESP32_SPIRAM_H_



#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "mn_allocator_interface.hpp"

#include "esp32/spiram.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"

/**
 * Allocator for the esp32 spiram  
 */ 
template <typname T>
class basic_allocator_spiram : public basic_allocator_interface<T> {
public:
    basic_allocator_spiram() : m_bFound(false), m_bFailed(false) { }
    /**
     * Create the allocator 
     * @return True when created and found the spiram or false when not
     */ 
    virtual bool create() {
        if(m_bFound) return true;

#ifndef CONFIG_SPIRAM_BOOT_INIT
        if(m_bFailed) return false;

        uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
        uint32_t pkg_ver = chip_ver & 0x7;

        if (pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32D2WDQ5 || pkg_ver == EFUSE_RD_CHIP_VER_PKG_ESP32PICOD2) {
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
        return (m_bFound = true);
    }

    /**
     * Get the total size of SPIRAM 
     * @return total size in bytes
     */ 
    virtual unsigned int get_size() {
        return heap_caps_get_total_size( MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }
    virtual unsigned int get_free() {
        return heap_caps_get_free_size( MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    }

    /**
     * Allocate SIZE bytes of memory 
     * @return A pointer of the allocated ram
     */ 
    virtual T* alloc(unsigned int xTime) {
        return  (m_bFound) ?  (T*)heap_caps_malloc( sizeof(T) , MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : NULL;
    }

    virtual T* alloc_range(size_t n, unsigned int xTime) {
        return (m_bFound) ?  (T*)heap_caps_malloc( sizeof(T) * n , MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : NULL;
    }
    virtual void* alloc_raw(size_t size, size_t n, unsigned int xTime ) {
        return (m_bFound) ? heap_caps_malloc( size * n , MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : NULL;
    }
    /**
     * Allocate n elements of SIZE bytes each, all initialized to 0. 
     * @return A pointer of the allocated ram
     */ 
    virtual T* calloc(size_t n, unsigned int xTime) {
        return (m_bFound) ?  (T*)heap_caps_calloc(n, sizeof(T), MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : NULL;
    }
    /**
     * Re-allocate the previously allocated block in PTR, making the new
     * block large enough for NMEMB elements of SIZE bytes each.
     * @return A pointer of the re-allocated ram
     */ 
    virtual T* realloc(void *ptr, size_t size, unsigned int xTime) {
        return (m_bFound) ? (T*)heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT) : NULL;
    }
    /**
     * Free a block allocated by `malloc', `realloc' or `calloc'. 
     * @return True the mem are free and false when not
     */ 
    virtual bool free(T* mem, unsigned int xTime) {
        if(!m_bFound) return false; 
        heap_caps_free(mem); return true;
    }

    /**
     * @brief Return the size that a particular pointer was allocated with.
     *
     * @param ptr Pointer to currently allocated heap memory. Must be a pointer value previously 
     * returned by heap_caps_malloc,malloc,calloc, etc. and not yet freed.
     *
     * @return Size of the memory allocated at this block and 0 when mem are NULL is
     */
    virtual unsigned long get_size(T* mem) {
        if(mem == NULL) return 0;
        return heap_caps_get_allocated_size(mem);
    }
private:
    bool m_bFound;
#ifndef CONFIG_SPIRAM_BOOT_INIT
    bool m_bFailed;
#endif
};



#endif

#endif