/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef __MINLIB_MNTHREAD_CAPS_ALLOCATOR_H__
#define __MINLIB_MNTHREAD_CAPS_ALLOCATOR_H__

#include "../mn_config.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "esp_heap_caps.h"
#include "esp32/spiram.h"
#include "soc/efuse_reg.h"
#include "esp_heap_caps.h"

namespace mn {
	namespace memory {

		enum class cap_allocator_map {
            NotUse2 = MALLOC_CAP_PID2, ///< Memory must be mapped to PID2 memory space (PIDs are not currently used)
            NotUse3 = MALLOC_CAP_PID3, ///< Memory must be mapped to PID3 memory space (PIDs are not currently used)
            NotUse4 = MALLOC_CAP_PID4, ///< Memory must be mapped to PID4 memory space (PIDs are not currently used)
            NotUse5 = MALLOC_CAP_PID5, ///< Memory must be mapped to PID5 memory space (PIDs are not currently used)
            NotUse6 = MALLOC_CAP_PID6, ///< Memory must be mapped to PID6 memory space (PIDs are not currently used)
            NotUse7 = MALLOC_CAP_PID7, ///< Memory must be mapped to PID7 memory space (PIDs are not currently used)
            SpiRam = MALLOC_CAP_SPIRAM, ///< Memory must be in SPI RAM
            Internal = MALLOC_CAP_INTERNAL, ///< Memory must be internal; specifically it should not disappear when flash/spiram cache is switched off
            Default = MALLOC_CAP_DEFAULT,  ///< Memory can be returned in a non-capability-specific memory allocation (e.g. malloc(), calloc()) call
            DMA = MALLOC_CAP_EXEC, ///< Memory must be able to run executable code
            Exec = MALLOC_CAP_DMA, ///< Memory must be able to accessed by DMA
        };
        #define CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS) ((int)(TCAPS)|(int)(TSBITS))

        enum class cap_allocator_size {
            Size8Bit = MALLOC_CAP_8BIT,
            Size32Bit = MALLOC_CAP_32BIT,
        };

        /**
        * ESP32 allocator heap wrapper
        */
        template <cap_allocator_map TCAPS, cap_allocator_size TSBITS >
		class basic_caps_allocator {
		public:
			using allocator_category = std_allocator_tag();
			using is_thread_safe = false ;

			basic_caps_allocator() {}

			static void first() noexcept {
				if((int)(TCAPS) == MALLOC_CAP_SPIRAM ) m_bFound = intarnal_create_spi_ram();
                else if((int)(TCAPS) == MALLOC_CAP_INTERNAL) m_bFound = true;
			}
			static void* allocate(size_t size, size_t alignment) noexcept {
				if(!m_bFound) return NULL;

				void* buf = (void*)heap_caps_malloc(size,  CAP_ALLOCATOR_MAP_SIZE(TCAPS, TSBITS));
				assert(buf != NULL);

                return buf;
			}
			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {
				heap_caps_free(ptr);
			}

			static size_t max_node_size()  {
				return size_t(-1);
			}
			static size_t get_max_alocator_size() const  {
				return __SIZE_MAX__;
			}
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
			static bool m_bFound;
		};

		template <cap_allocator_map TCAPS, cap_allocator_size TSBITS >
		bool basic_caps_allocator<TCAPS, TSBITS>::m_bFound = false;


		template<cap_allocator_map TCAPS, cap_allocator_size TSBITS, class TFilter = basic_allocator_filter>
		using caps_allocator = basic_allocator<basic_caps_allocator<TCAPS, TSBITS>, TFilter>;

	}
}

#endif // MN_THREAD_CONFIG_BOARD

#endif // __BASIC_CAPS_ALLOCATOR_HPP
