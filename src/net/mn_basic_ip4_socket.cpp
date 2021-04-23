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

#include "net/mn_basic_ip4_socket.hpp"

namespace mn {
	namespace net {
		//-----------------------------------
		// basic_ip4_socket::basic_ip4_socket
		//-----------------------------------
		basic_ip4_socket::basic_ip4_socket(handle_type& hndl, basic_ip4_endpoint* endp)
			: basic_ip_socket(hndl) {
				m_pEndPoint = endp;
		}

		//-----------------------------------
		// basic_ip4_socket::basic_ip4_socket
		//-----------------------------------
		basic_ip4_socket::basic_ip4_socket(const socket_type& type, const protocol_type& protocol)
			: basic_ip_socket(address_family::inet_v4, type, protocol) {
				m_pEndPoint = nullptr;
		}

		//-----------------------------------
		// basic_ip4_socket::basic_ip4_socket
		//-----------------------------------
		basic_ip4_socket::basic_ip4_socket(const basic_ip4_socket& other)
			: basic_ip_socket( other.m_iHandle), m_pEndPoint(other.m_pEndPoint) {
		}

		//-----------------------------------
		// basic_ip4_socket::operator =
		//-----------------------------------
		basic_ip4_socket& basic_ip4_socket::operator = (const basic_ip4_socket& other) {
			m_iHandle = other.m_iHandle;
			m_pEndPoint = other.m_pEndPoint;

			return *this;
		}

		//-----------------------------------
		// basic_ip4_socket::get_endpoint
		//-----------------------------------
		basic_ip4_endpoint* basic_ip4_socket::get_endpoint(bool local) {
			if(m_iHandle == -1) return nullptr;
			basic_ip4_endpoint *_ret = nullptr;

			struct sockaddr_in name;
			unsigned int name_len = sizeof(name);

			if (local) {
				 lwip_getsockname(m_iHandle, (struct sockaddr *) &name, &name_len);

			} else {
				 lwip_getpeername(m_iHandle, (struct sockaddr *) &name, &name_len);
			}

			_ret = new basic_ip4_endpoint(basic_ip4_address(name.sin_addr.s_addr), lwip_htons(name.sin_port));
			if(local) m_pEndPoint = _ret;

			return _ret;
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(basic_ip4_endpoint local_ep, bool reuseAddress, bool reusePort) {
			if(m_iHandle == -1) return false;

			basic_ip4_address ip = local_ep.get_ip();
			unsigned int port = local_ep.get_port();

			if (reuseAddress)
				set_reuse_address(true);
			if (reusePort)
				set_rause_port(true);

			struct sockaddr_in addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;
			int _iret = lwip_bind(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) );

			if(_iret == -1) {
				ESP_LOGE("socket", "could not bind socket: %d", errno);
				return false;
			}
			fcntl(m_iHandle, F_SETFL, O_NONBLOCK);

			return true;
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(const unsigned int& port, bool reuseAddress, bool reusePort) {
			return bind(MNNET_IPENDPOINT4_ANY(port), reuseAddress, reusePort );
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(basic_ip4_address ip, const unsigned int& port,
									bool reuseAddress, bool reusePort) {
			return bind(MNNET_IPENDPOINT4(ip, port), reuseAddress, reusePort );
		}

		//-----------------------------------
		// basic_ip4_socket::get_peername
		//-----------------------------------
		bool basic_ip4_socket::get_peername(basic_ip4_address& ipPeerAddress, uint16_t& iPeerPort) {
			bool _ret = false;
			struct sockaddr_in stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			int _iret = lwip_getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(_iret != 0) {
				ESP_LOGE("socket", "could not getpeername: %d", errno);
			} else {
				ipPeerAddress = basic_ip4_address(stPeer.sin_addr.s_addr);
				iPeerPort = stPeer.sin_port;
				_ret = true;
			}
			return _ret;
		}

		//-----------------------------------
		// basic_ip4_socket::get_peername
		//-----------------------------------
		bool basic_ip4_socket::get_peername(basic_ip4_endpoint& endpoint) {
			bool _ret = false;
			struct sockaddr_in stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			int _iret = lwip_getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(_iret != 0) {
				ESP_LOGE("socket", "could not getpeername: %d", errno);
			} else {
				endpoint = basic_ip4_endpoint(basic_ip4_address(stPeer.sin_addr.s_addr), stPeer.sin_port);
				_ret = true;
			}
			return _ret;
		}
	}
}
