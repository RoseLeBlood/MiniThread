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

#ifndef __MINILIB_BLOCK_DEVICE_H__
#define __MINILIB_BLOCK_DEVICE_H__

#include "../mn_config.hpp"
#include "../mn_def.hpp"

namespace mn {
	namespace device {
		/**
		 * @brief basic_block_device class is an interface for a block device.
		 * @ingroup devices
		 */
		class basic_block_device {
		public:
			basic_block_device() = default;

			/**
			 * @brief BlockDevice's destructor
			 */
			virtual ~basic_block_device() = default;

			/**
			 * @brief Opens device.
			 * @return 0 on success, error code otherwise
			 */
			virtual int open() = 0;
			/**
			 * @brief Synchronizes state of a device, ensuring all cached writes are finished.
			 *
			 * @pre Device is opened.
			 *
			 * @return 0 on success, error code otherwise
			 */
			virtual int synchronize() = 0;
			/**
			 * @brief Closes device.
			 * @pre Device is opened.
			 * @return 0 on success, error code otherwise
			 */
			virtual int close() = 0;

			/**
			 * @brief Locks the device for exclusive use by current thread.
			 */
			virtual void lock() = 0;

			/**
			 * @brief Unlocks the device which was previously locked by current thread.
			 * @warning This function must not be called from interrupt context!
			 */
			virtual void unlock() = 0;

			/**
			 * @brief Reads data from a device.
			 *
			 * @param [in] address is the address of data that will be read, must be a multiple of block size
			 * @param [out] buffer is the buffer into which the data will be read, must be valid
			 * @param [in] size is the size of \a buffer, bytes, must be a multiple of block size
			 *
			 * @return The size to readed and 0 on error.
			 */

			virtual int read(uint64_t address, void* buffer, size_t size) = 0;
			/**
			 * @brief Writes data to a device.
			 * @pre \a address and \a buffer and \a size are valid.
			 * @pre Selected range is within address space of device.
			 *
			 * @param [in] address is the address of data that will be written, must be a multiple of block size
			 * @param [in] buffer is the buffer with data that will be written, must be valid
			 * @param [in] size is the size of \a buffer, bytes, must be a multiple of block size
			 *
			 * @return The size to written and 0 on error.
			 */
			virtual int write(uint64_t address, const void* buffer, size_t size) = 0;

			/**
			 * @brief Erases blocks on a device.
			 * @param [in] address is the address of range that will be erased, must be a multiple of block size
			 * @param [in] size is the size of erased range, bytes, must be a multiple of block size
			 *
			 * @return 0 on success, error code otherwise
			 */
			virtual int erase(uint64_t address, uint64_t size) = 0;

			/**
			 * @return block size, bytes
			 */
			virtual size_t get_block_size() const = 0;

			/**
			 * @return size of block device, bytes
			 */
			virtual uint64_t get_size() const = 0;

			basic_block_device(const basic_block_device&) = delete;
			basic_block_device& operator=(const basic_block_device&) = delete;

		};
	}
}


#endif // __MINILIB_BLOCK_DEVICE_H__
