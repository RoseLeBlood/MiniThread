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
#ifndef _MINLIB_BASIC_ENDPOINT_HPP_
#define _MINLIB_BASIC_ENDPOINT_HPP_

#include "../mn_config.hpp"
#include "mn_basic_ip_address.hpp"

#define MNNET_IPENDPOINT4_BROADCAST(PORT) 	mn::net::basic_ip4_endpoint(MNNET_IPV4_ADDRESS_BROADCAST, PORT)
#define MNNET_IPENDPOINT4_ANY(PORT) 		mn::net::basic_ip4_endpoint(MNNET_IPV4_ADDRESS_ANY, PORT)
#define MNNET_IPENDPOINT4_LOOPBACK(PORT)	mn::net::basic_ip4_endpoint(MNNET_IPV4_ADDRESS_LOOPBACK, PORT)
#define MNNET_IPENDPOINT4_NONE(PORT)		mn::net::basic_ip4_endpoint(MNNET_IPV4_ADDRESS_NONE, PORT)

#define MNNET_IPENDPOINT4(IP, PORT)			mn::net::basic_ip4_endpoint(IP, PORT)
#define MNNET_IPENDPOINT4_EMPTY				MNNET_IPENDPOINT4_ANY(0)

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
	#define MNNET_IPENDPOINT6_ANY(PORT) 		mn::net::basic_ip6_endpoint(MNNET_IPV6_ADDRESS_ANY, PORT)
	#define MNNET_IPENDPOINT6_LOOPBACK(PORT)	mn::net::basic_ip6_endpoint(MNNET_IPV6_ADDRESS_LOOPBACK, PORT)

	#define MNNET_IPENDPOINT6(IP, PORT)			mn::net::basic_ip6_endpoint(IP, PORT)
	#define MNNET_IPENDPOINT6_EMPTY				MNNET_IPENDPOINT4_ANY(0)
#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE

namespace mn {
	namespace net {
		class basic_endpoint {
		public:
			virtual basic_endpoint* get_copy() = 0;
		};

		class basic_ip4_endpoint : public basic_endpoint {
		public:
			basic_ip4_endpoint(const uint16_t& port = 0);
			basic_ip4_endpoint(const ip4_address& ip, const uint16_t& port);
			basic_ip4_endpoint(const basic_ip4_endpoint& pOther);

			uint16_t 			get_port();
			ip4_address 		get_ip();
			void 				set_port(const uint16_t& port);
			bool 				is_broadcast();
			bool 				is_loopback();
			basic_endpoint* 	get_copy() override;

			bool operator == (const basic_ip4_endpoint& pOther);
			bool operator != (const basic_ip4_endpoint& pOther);
			basic_ip4_endpoint& operator = (const basic_ip4_endpoint& pOther);
		private:
			uint16_t m_iPort;
			ip4_address m_ipAdress;
		};
#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		class basic_ip6_endpoint : public basic_endpoint {
		public:
			basic_ip6_endpoint(const uint16_t& port = 0);
			basic_ip6_endpoint(const ip6_address& ip, const uint16_t& port);
			basic_ip6_endpoint(const basic_ip6_endpoint& pOther);

			uint16_t 			get_port();
			ip6_address 		get_ip();
			void 				set_port(const uint16_t& port);
			bool 				is_multicast();
			bool 				is_loopback();
			basic_endpoint* 	get_copy() override;

			bool operator == (const basic_ip6_endpoint& pOther);
			bool operator != (const basic_ip6_endpoint& pOther);
			basic_ip6_endpoint& operator = (const basic_ip6_endpoint& pOther);
		private:
			uint16_t m_iPort;
			ip6_address m_ipAdress;
		};
		using ip6_endpoint = basic_ip6_endpoint;
#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE

		using ip4_endpoint = basic_ip4_endpoint;

	}
}

#endif // _MINLIB_BASIC_ENDPOINT_HPP_
