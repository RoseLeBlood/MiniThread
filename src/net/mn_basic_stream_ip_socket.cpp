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
#include "net/mn_basic_stream_ip_socket.hpp"

namespace mn {
	namespace net {

		int basic_stream_ip_socket::send(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			m_iLastError = lwip_send(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));
			return m_iLastError;
		}
		int basic_stream_ip_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			m_iLastError = lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));
			return m_iLastError;
		}

		bool basic_stream_ip_socket::connect(ip4_endpoint remote_ep) {
			if(m_iHandle == -1) return false;

			ipaddress_type ip = remote_ep.get_ip();
			unsigned int port = remote_ep.get_port();

			struct sockaddr_in addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;

			m_iLastError = lwip_connect(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) ) ;
			return m_iLastError != -1;
		}
		bool basic_stream_ip_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	m_iLastError = lwip_listen(m_iHandle, backLog) ;
        	return m_iLastError != -1;
		}

		typename basic_stream_ip_socket::self_type* basic_stream_ip_socket::accept() {
			if(m_iHandle == -1) return nullptr;

			int clientfd;
			struct sockaddr_in client_addr;
			unsigned int addrlen=sizeof(client_addr);
			self_type* socket_return = nullptr;

			clientfd = lwip_accept(m_iHandle, (struct sockaddr*)&client_addr, &addrlen);
			if(clientfd >= 0) {
				auto port = lwip_ntohs(client_addr.sin_port);
				auto ip = ip4_address( (uint32_t)(client_addr.sin_addr.s_addr) );

				socket_return = new self_type(clientfd, new endpoint_type( ip, port) );
			} else {
				m_iLastError = clientfd;
			}
			return socket_return;
		}

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

		int basic_stream_ip6_socket::send(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			m_iLastError = lwip_send(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));
			return m_iLastError;
		}
		int basic_stream_ip6_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			m_iLastError = lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));
			return m_iLastError;
		}

		bool basic_stream_ip6_socket::connect(endpoint_type remote_ep) {
			if(m_iHandle == -1) return false;

			ipaddress_type ip = remote_ep.get_ip();
			unsigned int port = remote_ep.get_port();

			struct sockaddr_in6 addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);
			addr.sin6_addr.un.u32_addr[0] = ip.get_int(0);
			addr.sin6_addr.un.u32_addr[1] = ip.get_int(1);
			addr.sin6_addr.un.u32_addr[2] = ip.get_int(2);
			addr.sin6_addr.un.u32_addr[3] = ip.get_int(3);

			m_iLastError = lwip_connect(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) ) ;
			return m_iLastError != -1;
		}
		bool basic_stream_ip6_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	m_iLastError = lwip_listen(m_iHandle, backLog) ;
        	return m_iLastError != -1;
		}

		typename basic_stream_ip6_socket::self_type* basic_stream_ip6_socket::accept() {
			if(m_iHandle == -1) return nullptr;

			int clientfd;
			struct sockaddr_in6 client_addr;
			unsigned int addrlen=sizeof(client_addr);
			self_type* socket_return = nullptr;

			clientfd = lwip_accept(m_iHandle, (struct sockaddr*)&client_addr, &addrlen);

			if(clientfd >= 0) {
				auto port = lwip_ntohs(client_addr.sin6_port);
		#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
				auto ip = ip6_address( client_addr.sin6_addr.un.u8_addr ,  client_addr.sin6_scope_id );
		#else
				auto ip = ip6_address( client_addr.sin6_addr.un.u8_addr );
		#endif
				socket_return = new self_type(clientfd, new endpoint_type( ip, port) );
			} else {
				m_iLastError = clientfd;
			}
			return socket_return;
		}

	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
	}
}
