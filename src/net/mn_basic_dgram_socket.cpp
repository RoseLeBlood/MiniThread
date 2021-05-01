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

		//-----------------------------------
		//  recive_from
		//-----------------------------------
		int basic_dgram_ip_socket::recive_from(char* buffer, int offset, int size,
			const socket_flags& socketFlags, typename basic_dgram_ip_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			struct sockaddr_in addr;
			unsigned int addrlen=sizeof(addr);
			memset((char *) &addr, 0, sizeof(addr));

			int _iret = lwip_recvfrom(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags),
						  (struct sockaddr*)&addr,
						  &addrlen );

			if(_iret > 0) {
				if(ep != NULL) {
					ep->set_host( basic_ip4_address( (uint32_t) addr.sin_addr.s_addr ) ) ;
					ep->set_port(addr.sin_port);
				}
			}
			return _iret;

		}

		//-----------------------------------
		//  send_to
		//-----------------------------------
		int basic_dgram_ip_socket::send_to(char* buffer, int offset, int size,
			const socket_flags& socketFlags, typename basic_dgram_ip_socket::endpoint_type& ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip_socket::ipaddress_type ip = ep.get_host();
			unsigned int port = ep.get_port();

			int _flags = static_cast<int>(socketFlags);

			struct sockaddr_in addr;
			unsigned int addrlen=sizeof(addr);

			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;

			return lwip_sendto(m_iHandle, &buffer[offset], size-offset, _flags,
							   (struct sockaddr*)&addr,
							   addrlen );
		}



		//======================== basic_dgram_ip6_socket ========================
	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		//-----------------------------------
		//  recive_from
		//-----------------------------------
		int basic_dgram_ip6_socket::recive_from(char* buffer, int offset, int size,
			socket_flags socketFlags, typename basic_dgram_ip6_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			struct sockaddr_in6 addr;
			unsigned int addrlen=sizeof(addr);
			memset((char *) &addr, 0, sizeof(addr));

			int _iret =  lwip_recvfrom(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags),
						  (struct sockaddr*)&addr,
						  &addrlen );


			if(_iret > 0) {
				if(ep != NULL) {
					basic_ip6_address _ipx( addr.sin6_addr.un.u32_addr[0],  addr.sin6_addr.un.u32_addr[1],
											addr.sin6_addr.un.u32_addr[2],  addr.sin6_addr.un.u32_addr[3]  );

				#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
					_ipx->set_scopeid(addr.sin6_scope_id);

				#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID
					ep->set_host(_ipx) ;
					ep->set_port(addr.sin6_port);
				}
			}
			return _iret;
		}

		//-----------------------------------
		//  send_to
		//-----------------------------------
		int basic_dgram_ip6_socket::send_to(char* buffer, int offset, int size,
			socket_flags socketFlags, typename basic_dgram_ip6_socket::endpoint_type* ep) {

			if(m_iHandle == -1) return -1;

			typename basic_dgram_ip6_socket::ipaddress_type ip = ep->get_host();
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

			return lwip_sendto(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags),
							   (struct sockaddr*)&addr,
							   addrlen );

		}
	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
	}
}
