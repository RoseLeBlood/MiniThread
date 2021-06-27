/**
 * @file
 * @brief
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
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
#include "device/mn_system.hpp"
#include "mn_config.hpp"

#if MN_THREAD_CONFIG_BOARD == MN_THREAD_CONFIG_ESP32

#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include "soc/rtc.h"
#include "soc/rtc_cntl_reg.h"

#include <esp_log.h>

#include "mn_sleep.hpp"
#include "mn_error.hpp"

#define TAG	"system"

namespace mn {
namespace device {

	class flash_stream : public system_stream {
	public:
		virtual int erase(uint32_t sector) {
			return spi_flash_erase_sector(sector) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
		virtual int erase(uint32_t offset, size_t size) {
			return spi_flash_erase_range(offset, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
    	virtual int write(uint32_t offset, uint32_t *data, size_t size) {
			return spi_flash_write(offset, (uint32_t*) data, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
    	virtual int read(uint32_t offset, uint32_t *data, size_t size) {
			return spi_flash_read(offset, (uint32_t*) data, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
	};

	 system_stream* basic_system::get_flash() {
		static system_stream* __flash_stream = nullptr;
		if(__flash_stream == nullptr) __flash_stream = new flash_stream();
		return __flash_stream;
	 }

	class partition_stream : public system_stream {
	public:
		partition_stream(esp_partition_t* part)
			: m_part(part) {  }

		virtual int erase(uint32_t sector) {
			return erase(sector, 4);
		}
		virtual int erase(uint32_t offset, size_t size) {
			return esp_partition_erase_range(m_part, offset, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
    	virtual int write(uint32_t offset, uint32_t *data, size_t size) {
			return esp_partition_write(m_part, offset, data, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}
    	virtual int read(uint32_t offset, uint32_t *data, size_t size) {
			return esp_partition_read(m_part, offset, data, size) == ESP_OK ? NO_ERROR : ERR_MNTHREAD_UNKN;
		}

		static esp_partition_t* find(const char* name) {
			return const_cast<esp_partition_t*>(esp_partition_find_first(
									(esp_partition_type_t)0xff, ESP_PARTITION_SUBTYPE_ANY, name));
		}
	private:
		esp_partition_t* m_part;
	};

	system_stream* 	basic_system::get_partition_stream(const char* name) {
		esp_partition_t* part = partition_stream::find(name);

		if(part != nullptr) {
			//return new partition_stream(part);
		}
		return nullptr;
	}

	void basic_system::shutdown(basic_system::power_state_code code, mn::timespan_t span) {
		ESP_LOGI(TAG, "reach shutdown with code %d", (int)code);
		if(span == mn::timespan_t::now())
			ESP_LOGI(TAG, "system will go to shutdown now!");
		else {
			ESP_LOGI(TAG, "system will go to shutdown in %d secs", (int)span.get_total_seconds());
			mn::delay(span);
		}

		ESP_LOGE(TAG, "enter deep sleep wakeup by any gpio pin!");

		esp_sleep_enable_gpio_wakeup();
		esp_deep_sleep_start();
	}

	void basic_system::restart(basic_system::power_state_code code, mn::timespan_t span) {
		ESP_LOGI(TAG, "reach restart with code %d", (int)code);
		if(span == mn::timespan_t::now())
			ESP_LOGI(TAG, "system will restart now!");
		else {
			ESP_LOGI(TAG, "system will restart in %d secs", (int)span.get_total_seconds());
			mn::delay(span);
		}
		esp_restart();
	}

	void basic_system::deepsleep(power_state_code code, uint64_t time_us, mn::timespan_t span) {
		ESP_LOGI(TAG, "reach deepsleep with code %d", (int)code);
		if(span == mn::timespan_t::now())
			ESP_LOGI(TAG, "system will go to deepsleep now!");
		else {
			ESP_LOGI(TAG, "system will go to deepsleep in %d secs", (int)span.get_total_seconds());
			mn::delay(span);
		}

		esp_sleep_enable_timer_wakeup(time_us);
		esp_deep_sleep_start();
	}


	basic_system::randomAccessMemory basic_system::get_internal() {
		uint32_t  _free=0, _min_freeSinceBoot=0, _maxAlloc=0;
		multi_heap_info_t info;
		heap_caps_get_info(&info, MALLOC_CAP_INTERNAL);

		_free = heap_caps_get_free_size(MALLOC_CAP_INTERNAL);
		_min_freeSinceBoot = heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL);
		_maxAlloc = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL);

		return randomAccessMemory { info.total_free_bytes + info.total_allocated_bytes,
									_free,
									_min_freeSinceBoot,
									_maxAlloc };
	}

	basic_system::randomAccessMemory basic_system::get_external() {
		uint32_t  _free=0, _min_freeSinceBoot=0, _maxAlloc=0;
		multi_heap_info_t info;
		heap_caps_get_info(&info, MALLOC_CAP_SPIRAM);

		_free = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
		_min_freeSinceBoot = heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM);
		_maxAlloc = heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);

		return randomAccessMemory { info.total_free_bytes + info.total_allocated_bytes,
									_free,
									_min_freeSinceBoot,
									_maxAlloc };
	}

	uint8_t basic_system::get_num_cores() {
		esp_chip_info_t chip_info;
		esp_chip_info(&chip_info);
		return chip_info.cores;
	}
	uint8_t basic_system::get_core_speed(const uint8_t core) {
		MN_UNUSED_VARIABLE(core);
		return get_speed();
	}
	uint8_t basic_system::get_speed() {
		rtc_cpu_freq_config_t conf;
    	rtc_clk_cpu_freq_get_config(&conf);
    	return conf.freq_mhz;
	}

	void basic_system::set_core_speed(const uint8_t core, const uint8_t speed) {
		MN_UNUSED_VARIABLE(core);
		set_speed(speed);
	}
	void basic_system::set_speed(const uint8_t speed) {
		return;
	}

	const char* basic_system::get_sdk_version() {
		return esp_get_idf_version();
	}
	const char* basic_system::get_name() {
		uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
		uint32_t pkg_ver = chip_ver & 0x7;

		switch (pkg_ver) {
		case EFUSE_RD_CHIP_VER_PKG_ESP32D0WDQ6 :
			return "ESP32-D0WDQ6";
		case EFUSE_RD_CHIP_VER_PKG_ESP32D0WDQ5 :
			return "ESP32-D0WDQ5";
		case EFUSE_RD_CHIP_VER_PKG_ESP32D2WDQ5 :
			return "ESP32-D2WDQ5";
		case EFUSE_RD_CHIP_VER_PKG_ESP32PICOD2 :
			return "ESP32-PICO-D2";
		case EFUSE_RD_CHIP_VER_PKG_ESP32PICOD4 :
			return "ESP32-PICO-D4";
		default:
			return "Unknown";
		}
	}

}
}

#endif // MN_THREAD_CONFIG_BOARD
