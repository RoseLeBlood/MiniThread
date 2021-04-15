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
#include "mn_config.hpp"
#include "net/mn_basic_endpoint.hpp"
#include "net/mn_basic_dgram_socket.hpp"


namespace mn {
	namespace net {
	//======================== basic_dgram_ip_socket ========================

		int basic_dgram_ip_socket::recive_from(char* buffer, int offset, int size,
			const socket_flags& socketFlags, typename basic_dgram_ip_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip_socket::ipaddress_type ip = ep->get_ip();
			unsigned int port = ep->get_port();

			int _flags = static_cast<int>(socketFlags);

			struct sockaddr_in addr;
			unsigned int addrlen=sizeof(addr);

			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;

			m_iLastError = lwip_recvfrom(m_iHandle, &buffer[offset], size-offset, _flags,
						  (struct sockaddr*)&addr,
						  &addrlen );
			return m_iLastError;
		}

		int basic_dgram_ip_socket::send_to(char* buffer, int offset, int size,
			const socket_flags& socketFlags, typename basic_dgram_ip_socket::endpoint_type& ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip_socket::ipaddress_type ip = ep.get_ip();
			unsigned int port = ep.get_port();

			int _flags = static_cast<int>(socketFlags);

			struct sockaddr_in addr;
			unsigned int addrlen=sizeof(addr);

			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;

			m_iLastError = lwip_sendto(m_iHandle, &buffer[offset], size-offset, _flags,
							   (struct sockaddr*)&addr,
							   addrlen );
			return m_iLastError;
		}
		bool basic_dgram_ip_socket::bind_multicast(ip4_endpoint local_ep) {
			bool _retBool = false;

			if(bind((int)local_ep.get_port())) {
				struct ip_mreq mreq;
				mreq.imr_multiaddr.s_addr = (in_addr_t)local_ep.get_ip();
				mreq.imr_interface.s_addr = INADDR_ANY;

				if (setsockopt(m_iHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
					 ESP_LOGE("dgram socket", "could not join igmp: %d", errno);
				} else {
					_retBool = true;
					m_ipMultiCast = local_ep.get_ip();
				}
			}
			return _retBool;
		}
		bool basic_dgram_ip_socket::bind_multicast(const typename basic_dgram_ip_socket::ipaddress_type& ip, const unsigned int& port) {
			return bind_multicast(ip4_endpoint(ip, port));
		}

		//======================== basic_dgram_ip6_socket ========================

		int basic_dgram_ip6_socket::recive_from(char* buffer, int offset, int size,
			socket_flags socketFlags, typename basic_dgram_ip6_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip6_socket::ipaddress_type ip = ep->get_ip();
			unsigned int port = ep->get_port();

			struct sockaddr_in6 addr;
			unsigned int addrlen=sizeof(addr);

			memset((char *) &addr, 0, sizeof(addr));
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			addr.sin6_addr.un.u32_addr[0] = ip.get_int(0);
			addr.sin6_addr.un.u32_addr[1] = ip.get_int(1);
			addr.sin6_addr.un.u32_addr[2] = ip.get_int(2);
			addr.sin6_addr.un.u32_addr[3] = ip.get_int(3);

			m_iLastError = lwip_recvfrom(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags),
						  (struct sockaddr*)&addr,
						  &addrlen );
			return m_iLastError;
		}

		int basic_dgram_ip6_socket::send_to(char* buffer, int offset, int size,
			socket_flags socketFlags, typename basic_dgram_ip6_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip6_socket::ipaddress_type ip = ep->get_ip();
			unsigned int port = ep->get_port();

			struct sockaddr_in6 addr;
			unsigned int addrlen=sizeof(addr);

			memset((char *) &addr, 0, sizeof(addr));
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			addr.sin6_addr.un.u32_addr[0] = ip.get_int(0);
			addr.sin6_addr.un.u32_addr[1] = ip.get_int(1);
			addr.sin6_addr.un.u32_addr[2] = ip.get_int(2);
			addr.sin6_addr.un.u32_addr[3] = ip.get_int(3);

			m_iLastError = lwip_sendto(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags),
							   (struct sockaddr*)&addr,
							   addrlen );
			return m_iLastError;
		}
	}
}
