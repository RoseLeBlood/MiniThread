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
#ifndef __MINILIB_BASIC_IP6_ENDPOINT_H__
#define __MINILIB_BASIC_IP6_ENDPOINT_H__

#include "../mn_config.hpp"

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

#define MNNET_IPENDPOINT6_ANY(PORT) 		mn::net::basic_ip6_endpoint(MNNET_IPV6_ADDRESS_ANY, PORT)
#define MNNET_IPENDPOINT6_LOOPBACK(PORT)	mn::net::basic_ip6_endpoint(MNNET_IPV6_ADDRESS_LOOPBACK, PORT)

#define MNNET_IPENDPOINT6(IP, PORT)			mn::net::basic_ip6_endpoint(IP, PORT)
#define MNNET_IPENDPOINT6_EMPTY				MNNET_IPENDPOINT4_ANY(0)


#include "mn_basic_endpoint.hpp"
#include "mn_basic_ip6_address.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief This class represents an IPv6 endpoint/socket address.
		 * @ingroup socket
		 */
		class basic_ip6_endpoint : public basic_ip_endpoint<basic_ip6_address, address_family::inet_v6> {
			using base_type = basic_ip_endpoint<basic_ip6_address, address_family::inet_v6>;
		public:
			basic_ip6_endpoint(const uint16_t& port = 0);
			basic_ip6_endpoint(const basic_ip6_address& ip, const uint16_t& port);
			basic_ip6_endpoint(const basic_ip6_endpoint& pOther);

			/**
			 * @brief  Get the port number.
			 * @return The port number
			 */
			using base_type::get_port;
			/**
			 * @brief Get the host IP address.
			 * @return The host IP address.
			 */
			using base_type::get_host;
			/**
			 * @brief Set a port number, between 0-9999
			 */
			using base_type::set_port;
			/**
			 * @brief Is the host address a multicast address
			 * @return If true is the host address a multicast address and false is not
			 */
			using base_type::is_multicast;
			/**
			 * @brief Is the host address a loopaddress
			 * @return If true is the host address a loopaddress address and false is not
			 */
			using base_type::is_loopback;
			/**
			 * @brief Get the address family of the address.
			 * @return The address family of the address.
			 */
			using base_type::get_famile;

			using base_type::operator =;
			using base_type::operator ==;
			using base_type::operator !=;

			/**
			 * @brief Get a copy from this
			 * @return The copy of this.
			 */
			basic_endpoint* 	get_copy() override;

			/**
			 * @brief Get the host IP address.
			 * @return The host IP address.
			 * @note deprecated
			 */
			basic_ip6_address get_ip() __attribute__ ((deprecated)) { return get_host(); }
		};
	}
}
#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE

#endif // __MINILIB_BASIC_IP6_ENDPOINT_H__
