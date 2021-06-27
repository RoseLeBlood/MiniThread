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
#ifndef __MINILIB_DEV_BASE_DEVICE_H__
#define __MINILIB_DEV_BASE_DEVICE_H__

#include "../mn_config.hpp"

namespace mn {
	namespace device {
		class basic_device  {
		public:
			basic_device(char prefix[8])
				: m_prefix(prefix) { }
			/**
			 * @brief Opens device.
			 * @return 0 on success, error code otherwise
			 */
			virtual int open() = 0;
			/**
			 * @brief Closes device.
			 * @pre Device is opened.
			 * @return 0 on success, error code otherwise
			 */
			virtual int stop() = 0;

			virtual bool is_enable() = 0;

			virtual bool is_stream_support() = 0;

			const char* get_prefix() { return m_prefix; }
		protected:
			const char* m_prefix;
		};

		class basic_streamed_device : public basic_device {
		public:
			basic_streamed_device(char prefix[8])
				: basic_device(prefix) { }

			virtual bool is_stream_support() { return true; }

			virtual void* get_stream() = 0;
		};
	}
}



#endif // __MINILIB_DEV_NETWORK_DEVICE_H__

