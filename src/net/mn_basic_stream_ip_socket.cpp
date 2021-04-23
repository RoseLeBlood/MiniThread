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

#include "mn_task.hpp"
#include "net/mn_basic_endpoint.hpp"
#include "net/mn_basic_stream_ip_socket.hpp"

namespace mn {
	namespace net {

		//-----------------------------------
		// basic_stream_ip_socket::send
		//-----------------------------------
		int basic_stream_ip_socket::send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;

			const char* _pBuf = reinterpret_cast<const char*>(buffer) + offset;
			int _remaining = size-offset;
			int _sended = 0;
			int _sent = 0;
			bool _blocking = get_blocking();

			while (_remaining > 0) {
				_sended = lwip_send(m_iHandle, _pBuf, _remaining, static_cast<int>(socketFlags));

				_pBuf += _sended;
				_sent += _sended;

				_remaining -= _sended;

				if (_blocking && _remaining > 0)
					mn::basic_task::yield();
				else
					break;
			}
			return _sent;
		}

		int basic_stream_ip_socket::send_bytes(const void* buffer, int size, socket_flags socketFlags) {
			return send_bytes(buffer, 0, size, socketFlags);
		}

		//-----------------------------------
		// basic_stream_ip_socket::recive
		//-----------------------------------
		int basic_stream_ip_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			return lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));

		}

		//-----------------------------------
		// basic_stream_ip_socket::connect
		//-----------------------------------
		bool basic_stream_ip_socket::connect(endpoint_type remote_ep) {
			if(m_iHandle == -1) return false;

			ipaddress_type ip = remote_ep.get_ip();
			unsigned int port = remote_ep.get_port();

			struct sockaddr_in addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;

			bool _ret = ( lwip_connect(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) ) != -1) ;
			if(_ret) set_blocking(false);

			return _ret;
		}

		//-----------------------------------
		// basic_stream_ip_socket::listen
		//-----------------------------------
		bool basic_stream_ip_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	return lwip_listen(m_iHandle, backLog) != -1;
		}

		//-----------------------------------
		// basic_stream_ip_socket::accept
		//-----------------------------------
		typename basic_stream_ip_socket::self_type* basic_stream_ip_socket::accept() {
			if(m_iHandle == -1) return nullptr;

			int clientfd;
			struct sockaddr_in client_addr;
			unsigned int addrlen=sizeof(client_addr);
			self_type* socket_return = nullptr;

			clientfd = lwip_accept(m_iHandle, (struct sockaddr*)&client_addr, &addrlen);
			if(clientfd >= 0) {
				auto port = lwip_ntohs(client_addr.sin_port);
				auto ip = basic_ip4_address( (uint32_t)(client_addr.sin_addr.s_addr) );

				socket_return = new self_type(clientfd, new endpoint_type( ip, port) );
			}
			return socket_return;
		}

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

		//-----------------------------------
		// basic_stream_ip6_socket::send
		//-----------------------------------
		int basic_stream_ip6_socket::send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;

			const char* _pBuf = reinterpret_cast<const char*>(buffer) + offset;
			int _remaining = size-offset;
			int _sended = 0;
			int _sent = 0;
			bool _blocking = get_blocking();

			while (_remaining > 0) {
				_sended = lwip_send(m_iHandle, _pBuf, _remaining, static_cast<int>(socketFlags));

				_pBuf += _sended;
				_sent += _sended;

				_remaining -= _sended;

				if (_blocking && _remaining > 0)
					mn::basic_task::yield();
				else
					break;
			}
			return _sent;
		}

		int basic_stream_ip6_socket::send_bytes(const void* buffer, int size, socket_flags socketFlags) {
			return send_bytes(buffer, 0, size, socketFlags);
		}
		//-----------------------------------
		// basic_stream_ip6_socket::recive
		//-----------------------------------
		int basic_stream_ip6_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			return lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));

		}

		//-----------------------------------
		// basic_stream_ip6_socket::connect
		//-----------------------------------
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

			bool _ret = lwip_connect(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) ) != -1 ;
			if(_ret) set_blocking(false);

			return _ret;
		}

		//-----------------------------------
		// basic_stream_ip6_socket::listen
		//-----------------------------------
		bool basic_stream_ip6_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	return lwip_listen(m_iHandle, backLog) != -1;
		}

		//-----------------------------------
		// basic_stream_ip6_socket::accept
		//-----------------------------------
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
				auto ip = basic_ip6_address( client_addr.sin6_addr.un.u8_addr ,  client_addr.sin6_scope_id );
		#else
				auto ip = basic_ip6_address( client_addr.sin6_addr.un.u8_addr );
		#endif
				socket_return = new self_type(clientfd, new endpoint_type( ip, port) );
			}
			return socket_return;
		}

	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
	}
}
