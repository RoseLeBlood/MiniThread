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
#ifndef _MINLIB_ALLOCATOR_CAPS_H_
#define _MINLIB_ALLOCATOR_CAPS_H_

#include "mn_config.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "esp_heap_caps.h"
#include "esp32/spiram.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"


namespace mn {
    namespace memory {

        enum class cap_allocator_map {
            NotUse2 = MALLOC_CAP_PID2,        ///< Memory must be mapped to PID2 memory space (PIDs are not currently used)
            NotUse3 = MALLOC_CAP_PID3,        ///< Memory must be mapped to PID3 memory space (PIDs are not currently used)
            NotUse4 = MALLOC_CAP_PID4,        ///< Memory must be mapped to PID4 memory space (PIDs are not currently used)
            NotUse5 = MALLOC_CAP_PID5,        ///< Memory must be mapped to PID5 memory space (PIDs are not currently used)
            NotUse6 = MALLOC_CAP_PID6,        ///< Memory must be mapped to PID6 memory space (PIDs are not currently used)
            NotUse7 = MALLOC_CAP_PID7,        ///< Memory must be mapped to PID7 memory space (PIDs are not currently used)
            SpiRam = MALLOC_CAP_SPIRAM,       ///< Memory must be in SPI RAM
            Internal = MALLOC_CAP_INTERNAL,   ///< Memory must be internal; specifically it should not disappear when flash/spiram cache is switched off
            Default = MALLOC_CAP_DEFAULT,     ///< Memory can be returned in a non-capability-specific memory allocation (e.g. malloc(), calloc()) call   
            DMA = MALLOC_CAP_EXEC,            ///< Memory must be able to run executable code
            Exec = MALLOC_CAP_DMA,            ///< Memory must be able to accessed by DMA
        };
        
        enum class cap_allocator_size {
            Size8Bit = MALLOC_CAP_8BIT,
            Size32Bit = MALLOC_CAP_32BIT, 
        };

        #define CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS) ((int)(TCAPS)|(int)(TSBITS))
        /**
        * ESP32 allocator heap wrapper
        */ 
        template <typename T, cap_allocator_map TCAPS, cap_allocator_size TSBITS>
        class basic_cap_allocator_esp32  {
        public:
            /**
            * Create the allocator
            * @param [in] maxSize The max size to alloc with this allocator. 
            * @note When maxSize 0 is then use all 
            */ 
            bool create(size_t maxSize = 0) { 
                m_sAlloced = 0;
                m_sSize = sizeof(T);
                m_sMaxSize = maxSize;

                if((int)(TCAPS) == MALLOC_CAP_SPIRAM ) {
                    return intarnal_create_spi_ram();
                } else if((int)(TCAPS) == MALLOC_CAP_INTERNAL) {
                    m_bFound = true;
                }

                if(m_bFound) {
                    int nMaxElements = heap_caps_get_total_size( CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS));
                    nMaxElements = nMaxElements / m_sSize;

                    if( (maxSize > nMaxElements) || (maxSize == 0) ) m_sMaxSize = nMaxElements;
                }
                
                return m_bFound; 
            }

            /**
            * Allocate SIZE bytes of memory 
            * @return A pointer of the allocated ram
            */ 
            T* alloc(unsigned int xTime) {
                if(!m_bFound) return NULL;
                if(is_empty() ) return NULL;

                T* buf = (T*)heap_caps_malloc(m_sSize,  CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS));
                assert(buf != NULL);

                if(buf) { m_sAlloced++; }
                return buf;
            }
            
            size_t calloc(size_t n, T** buf, unsigned int xTime) {
                if(!m_bFound) return NULL;
                if(is_empty() ) {  buf = NULL; return 0; }

                size_t size = n;
                if(m_sMaxSize < size) size = get_free();

                *buf = (T*)heap_caps_calloc( n, m_sSize, CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS));
                assert(buf != NULL);
                m_sAlloced += size; 
                
                return size;
            }
            void free_range(size_t n, T** buf) {
                for(int i=0; i < n; i++) free(buf[i]);
            }

            /**
            * Free a block allocated by `malloc', `realloc' or `calloc'. 
            * @return True the mem are free and false when not
            */ 
            void free(T* mem, unsigned int xTime) {
                if(mem == NULL) return;
                
                heap_caps_free(mem); 
                m_sAlloced--;
            }

            /**
            * @brief Return the size that a particular pointer was allocated with.
            *
            * @param ptr Pointer to currently allocated heap memory. Must be a pointer value previously 
            * returned by heap_caps_malloc,malloc,calloc, etc. and not yet freed.
            *
            * @return Size of the memory allocated at this block and 0 when mem are NULL is
            */
            unsigned long get_size(T* mem) {
                if(!m_bFound) return 0;
                if(mem == NULL) return 0;
                return heap_caps_get_allocated_size(mem);
            }

            /**
            * Get the size of free bytes
            * @return The size of free bytes
            */ 
            unsigned long get_free()        { return m_sMaxSize - m_sAlloced; }
            unsigned long get_allocated()   { return m_sAlloced; }
            unsigned long get_max()         { return m_sMaxSize; }

            size_t size()                   { return m_sSize; } ///<Get the size of T
            void size(size_t uiSize)        { if(uiSize <= sizeof(T)) return; m_sSize = uiSize;  } ///<set the size off T. Muss be greater as sizeof(T) 

            bool is_empty()                 { return get_free() == 0;  }
        private:

            bool intarnal_create_spi_ram() {
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

        private:
            size_t m_sMaxSize;
            size_t m_sAlloced;
            size_t m_sSize;

            bool m_bFound;
            bool m_bFailed;
        };
    }
}



#endif

#endif