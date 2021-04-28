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
#include "net/mn_basic_raw_ip_socket.hpp"

namespace mn {
	namespace net {

		//-----------------------------------
		//  basic_raw_ip4_socket
		//-----------------------------------
		basic_raw_ip4_socket::basic_raw_ip4_socket()
			: base_type( socket_type::raw, protocol_type::unspec) { }

		//-----------------------------------
		//  basic_raw_ip4_socket
		//-----------------------------------
		basic_raw_ip4_socket::basic_raw_ip4_socket(const basic_raw_ip4_socket& other)
			: base_type( socket_type::raw, protocol_type::unspec) { }

		//-----------------------------------
		//  operator=
		//-----------------------------------
		basic_raw_ip4_socket& basic_raw_ip4_socket::operator=(const basic_raw_ip4_socket& rhs) {
			if (this == &rhs) return *this; // handle self assignment
			//assignment operator
			return *this;
		}

		//-----------------------------------
		// connect
		//-----------------------------------
		bool basic_raw_ip4_socket::connect(endpoint_type remote_ep) {
			if(m_iHandle == -1) return false;

			ipaddress_type ip = remote_ep.get_host();
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
		// accept
		//-----------------------------------
		typename basic_raw_ip4_socket::self_type* basic_raw_ip4_socket::accept() {
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

		//-----------------------------------
		// listen
		//-----------------------------------
		bool basic_raw_ip4_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	return lwip_listen(m_iHandle, backLog) != -1;
		}

		//-----------------------------------
		//  send_to
		//-----------------------------------
		int basic_raw_ip4_socket::send_to(char* buffer, int offset, int size, const
											socket_flags& socketFlags, endpoint_type& ep) {

			if(m_iHandle == -1) return -1;

			typename basic_raw_ip4_socket::ipaddress_type ip = ep.get_host();
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

		//-----------------------------------
		//  send_bytes
		//-----------------------------------
		int basic_raw_ip4_socket::send_bytes(const void* buffer, int size, socket_flags socketFlags) {
			return send_bytes(buffer, 0, size, socketFlags);
		}
		//-----------------------------------
		// send_bytes
		//-----------------------------------
		int basic_raw_ip4_socket::send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags) {
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

		//-----------------------------------
		//  recive_from
		//-----------------------------------
		int basic_raw_ip4_socket::recive_from(char* buffer, int offset, int size,
												 const socket_flags& socketFlags, endpoint_type* ep) {


			if(m_iHandle == -1) return -1;

			int _flags = static_cast<int>(socketFlags);

			struct sockaddr_in addr;
			unsigned int addrlen=sizeof(addr);
			memset((char *) &addr, 0, sizeof(addr));

			int _iret = lwip_recvfrom(m_iHandle, &buffer[offset], size-offset, _flags,
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
		// recive
		//-----------------------------------
		int basic_raw_ip4_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			return lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));

		}
		//-----------------------------------
		// recive
		//-----------------------------------
		int basic_raw_ip4_socket::recive(char* buffer, int size, socket_flags socketFlags) {
			return recive(buffer, 0, size, socketFlags);
		}
	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

		//-----------------------------------
		//  basic_raw_ip6_socket
		//-----------------------------------
		basic_raw_ip6_socket::basic_raw_ip6_socket()
			: base_type( socket_type::raw, protocol_type::unspec) { }

		//-----------------------------------
		//  basic_raw_ip6_socket
		//-----------------------------------
		basic_raw_ip6_socket::basic_raw_ip6_socket(const basic_raw_ip6_socket& other)
			: base_type( socket_type::raw, protocol_type::unspec) { }

		//-----------------------------------
		//  operator=
		//-----------------------------------
		basic_raw_ip6_socket& basic_raw_ip6_socket::operator=(const basic_raw_ip6_socket& rhs) {
			if (this == &rhs) return *this; // handle self assignment
			//assignment operator
			return *this;
		}

		//-----------------------------------
		// basic_raw_ip6_socket::send
		//-----------------------------------
		int basic_raw_ip6_socket::send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags) {
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

		int basic_raw_ip6_socket::send_bytes(const void* buffer, int size, socket_flags socketFlags) {
			return send_bytes(buffer, 0, size, socketFlags);
		}
		//-----------------------------------
		// basic_raw_ip6_socket::recive
		//-----------------------------------
		int basic_raw_ip6_socket::recive(char* buffer, int offset, int size, socket_flags socketFlags) {
			if(m_iHandle == -1) return -1;
			return lwip_recv(m_iHandle, &buffer[offset], size-offset, static_cast<int>(socketFlags));

		}

		//-----------------------------------
		// basic_raw_ip6_socket::connect
		//-----------------------------------
		bool basic_raw_ip6_socket::connect(endpoint_type remote_ep) {
			if(m_iHandle == -1) return false;

			ipaddress_type ip = remote_ep.get_host();
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
		// basic_raw_ip6_socket::listen
		//-----------------------------------
		bool basic_raw_ip6_socket::listen(int backLog) {
			if(m_iHandle == -1) return false;
        	return lwip_listen(m_iHandle, backLog) != -1;
		}

		//-----------------------------------
		// basic_raw_ip6_socket::accept
		//-----------------------------------
		typename basic_raw_ip6_socket::self_type* basic_raw_ip6_socket::accept() {
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

		//-----------------------------------
		//  recive_from
		//-----------------------------------
		int basic_raw_ip6_socket::recive_from(char* buffer, int offset, int size,
												const socket_flags& socketFlags, endpoint_type* ep) {

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
		//  recive_from
		//-----------------------------------
		int basic_raw_ip6_socket::recive_from(char* buffer, int size, endpoint_type* ep, const socket_flags& socketFlags) {
			return recive_from(buffer, 0, size,socketFlags, ep );
		}

		//-----------------------------------
		//  send_to
		//-----------------------------------
		int basic_raw_ip6_socket::send_to(char* buffer, int offset, int size,
											const socket_flags& socketFlags, endpoint_type& ep) {

			if(m_iHandle == -1) return -1;

			ipaddress_type ip = ep.get_host();
			unsigned int port = ep.get_port();

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
		//-----------------------------------
		//  send_to
		//-----------------------------------
		int basic_raw_ip6_socket::send_to(char* buffer, int size, endpoint_type& ep, const socket_flags& socketFlags) {
			return send_to(buffer, 0, size,socketFlags, ep );
		}

	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
	}
}
