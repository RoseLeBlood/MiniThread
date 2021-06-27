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
#ifndef __MINILIB_DEV_NETWORK_DEVICE_H__
#define __MINILIB_DEV_NETWORK_DEVICE_H__

#include "../mn_config.hpp"

#include "../net/mn_basic_ip4_address.hpp"
#include "mn_basic_device.hpp"

namespace mn {
	namespace device {
		class basic_network_device : public basic_device {
		public:
			using ip4_adress_t = net::basic_ip4_address;

			basic_network_device()
				: basic_network_device("dev") { }

			basic_network_device(char prefix[8])
				: basic_device(prefix) { }

			/**
			 * @brief Set IPv4 infos, local ip, gateway and subnet.
			 * @return If 0 then set all address and -1 if not.
			 */
			virtual int set_ipinfo(const ip4_adress_t& local_ip, const ip4_adress_t& gateway,
									const ip4_adress_t& subnet) = 0;

			/**
			 * @brief Get IPv4 infos, local ip, gateway and subnet.
			 * @return If 0 then get all address and -1 if not.
			 */
			virtual int get_ipinfo(ip4_adress_t& local_ip, ip4_adress_t& gateway,
									ip4_adress_t& subnet) = 0;

			/**
			 * @brief Get the MAC of the WiFi AP device as string.
			 * @return The MAC of the WiFi AP device as string, on error return nullptr.
			 */
			virtual const char*       get_mac() = 0;
		};
	}
}



#endif // __MINILIB_DEV_NETWORK_DEVICE_H__
