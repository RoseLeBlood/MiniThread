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

#ifndef __MINILIB_SYSTEMINFOS_H__
#define __MINILIB_SYSTEMINFOS_H__

#include "../mn_config.hpp"
#include "../mn_timespan.hpp"

namespace mn {
	namespace device {
		/**
		 * @brief A simple abstractes class for easy handle any flash drivers.
		 */
		class system_stream {
		public:
			virtual int erase(uint32_t sector) = 0;
			virtual int erase(uint32_t offset, size_t size) = 0;
    		virtual int write(uint32_t offset, uint32_t *data, size_t size) = 0;
    		virtual int read(uint32_t offset, uint32_t *data, size_t size) = 0;
		};

		/**
		 * @brief Get system infos and useful system
		 * functins of the using platform.
		 */
		class basic_system {
		public /* types */:

		public /*section power */:
			enum class power_state_code : int {
				normal, 	/*!<  normal code, no error. */
				error,		/*!< on error */
			};

			/**
			 * @brief Halt the CPU.
			 * @param code
			 * @param span how many time to wait to shutdown the system.
			 * @note ESP32: Enter to the deepsleep mode with wakeup with any gpio pin.
			 */
			static void shutdown(power_state_code code, mn::timespan_t span = mn::timespan_t(0));

			/**
			 * @brief Restart the CPU.
			 * @note ESP32: Peripherals (except for WiFi, BT, UART0, SPI1, and legacy timers) are not reset.
			 * @param code
			 * @param span how many time to wait to shutdown the system.
			 */
			static void restart(power_state_code code, mn::timespan_t span = mn::timespan_t(0));

			/**
			 * @brief Enter deep sleep with wakeup by timer.
			 * @param time_us deep-sleep time, unit: microsecond.
			 * @param span how many time to wait to deepsleep the system.
			 */
			static void deepsleep(power_state_code code, uint64_t time_us, mn::timespan_t span = mn::timespan_t(0));
		public /* section RAM */:

			/**
			 * @brief RAM Informations.
			 */
			union randomAccessMemory {
				struct {
					uint32_t size;				/*!< Total size of the RAM. */
					uint32_t free;				/*!< Current available RAM.*/
					uint32_t min_freeSinceBoot;	/*!< lowest size of free RAM since boot */
					uint32_t maxAlloc;	/*!< largest block of bytes that can be allocated at once. */
				};
				uint32_t data[3];
			};

			/**
			 * @brief Return the infos from the internal RAM.
			 */
			static randomAccessMemory get_internal();
			/**
			 * @brief Return the infos from the external RAM.
			 * @note ESP32: psram.
			 */
			static randomAccessMemory get_external();


		public /* cpu/SoC Infos */:

			/**
			 * @brief Get the number of useable cores.
			 * @return The number of useable cores. (On ESP32 1 or 2)
			 */
			static uint8_t get_num_cores();

			/**
			 * @brief Get the speed of a core in Mhz
			 * @return Cet the speed of a core.
			 * @note On ESP32: return get_speed();
			 */
			static uint8_t get_core_speed(const uint8_t core);

			/**
			 * @brief Get the mega hertz of the cpu.
			 * @return The speed in Mhz of the cpu.
			 */
			static uint8_t get_speed();

			/**
			 * @brief Set the speed of a given core.
			 * @note On ESP32 is the same with set_speed(speed);
			 */
			static void set_core_speed(const uint8_t core, const uint8_t speed);

			/**
			 * @brief Set the speed in Mhz of the cpu.
			 */
			static void set_speed(const uint8_t speed);

			/**
			 * @brief Get the version of the using platform SDKs.
			 * @return The version of the using platform SDKs.
			 */
			static const char* get_sdk_version();

			/**
			 * @brief Get the name of the using Platform.
			 * @return The name.
			 */
			static const char* get_name();

		public /*misc */:

			/**
			 * @brief Get a system stream to write/read to the SPI Flash.
			 */
			static system_stream* 	get_flash();

			/**
			 * @brief Get a system stream to write/read to a partition.
			 * @param name The name of partition.
			 * @return The stream to a partiotion or nullptr if the partition mith the given name not exist.
			 */
			static system_stream* 	get_partition_stream(const char* name);

		};
	}
}



#endif // __MINILIB_SYSTEMINFOS_H__
