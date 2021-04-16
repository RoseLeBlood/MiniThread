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
#include "net/mn_basic_socket.hpp"

namespace mn {
	namespace net {
		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::basic_ip_socket(const handle_type& hndl)
			: m_iHandle(hndl), m_iLastError(-1) { }

		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::basic_ip_socket(const address_family& fam, const socket_type& type,
										const protocol_type& protocol) {
			 m_iHandle = lwip_socket(static_cast<int>(fam), static_cast<int>(type), static_cast<int>(protocol) );
			 m_iLastError = m_iHandle != -1 ? 0 : m_iHandle;
		}

		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::~basic_ip_socket() {
			close();
		}

		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::basic_ip_socket(const basic_ip_socket& other)
			: m_iHandle(other.m_iHandle), m_iLastError(other.m_iLastError) { }

		//-----------------------------------
		// basic_ip_socket::available
		//-----------------------------------
		int basic_ip_socket::available() {
			if(m_iHandle == -1) return 0;
			int cBytes = 0;

			m_iLastError = lwip_ioctl(m_iHandle, FIONREAD, &cBytes);

			return cBytes;
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name, int value) {
			if(m_iHandle == -1) return -1;

			socklen_t _size = sizeof(value);
			return (m_iLastError = setsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, _size));
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name, bool value) {
			return set_options(opt, name, value ? 1 : 0 );
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name,
										void* value, uint32_t size) {
			if(m_iHandle == -1) return -1;
			return (m_iLastError = setsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), value, size));
		}

		//-----------------------------------
		// basic_ip_socket::get_option_int
		//-----------------------------------
		int basic_ip_socket::get_option_int(const socket_option_level& opt, const socket_option_name& name) {
			if(m_iHandle == -1) return -1;

			int value = 0;
			socklen_t _size = sizeof(value);
			m_iLastError = getsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, &_size);

			return value;
		}

		//-----------------------------------
		// basic_ip_socket::get_option_bool
		//-----------------------------------
		bool basic_ip_socket::get_option_bool(const socket_option_level& opt, const socket_option_name& name) {
			return get_option_int(opt, name) == 1;
		}

		//-----------------------------------
		// basic_ip_socket::get_option_ex
		//-----------------------------------
		void basic_ip_socket::get_option_ex(const socket_option_level& opt, const socket_option_name& name,
											void* value, uint32_t size) {
			if(m_iHandle == -1) { value = NULL; return; }
			m_iLastError = getsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, &size);
		}

		//-----------------------------------
		// basic_ip_socket::operator =
		//-----------------------------------
		basic_ip_socket& basic_ip_socket::operator = (const basic_ip_socket& other) {
			m_iHandle = other.m_iHandle;
			m_iLastError = other.m_iLastError;
			return *this;
		}

		//-----------------------------------
		// basic_ip4_socket::basic_ip4_socket
		//-----------------------------------
		basic_ip4_socket::basic_ip4_socket(handle_type& hndl, ip4_endpoint* endp)
			: basic_ip_socket(hndl) {
				m_pEndPoint = endp;
				m_iLastError = 0;
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
			m_iLastError = other.m_iLastError;
		}

		//-----------------------------------
		// basic_ip4_socket::operator =
		//-----------------------------------
		basic_ip4_socket& basic_ip4_socket::operator = (const basic_ip4_socket& other) {
			m_iHandle = other.m_iHandle;
			m_iLastError = other.m_iLastError;
			m_pEndPoint = other.m_pEndPoint;

			return *this;
		}

		//-----------------------------------
		// basic_ip4_socket::get_endpoint
		//-----------------------------------
		ip4_endpoint* basic_ip4_socket::get_endpoint(bool local) {
			if(m_iHandle == -1) return nullptr;
			ip4_endpoint *_ret = nullptr;

			struct sockaddr_in name;
			unsigned int name_len = sizeof(name);

			if (local) {
				m_iLastError = lwip_getsockname(m_iHandle, (struct sockaddr *) &name, &name_len);

			} else {
				m_iLastError = lwip_getpeername(m_iHandle, (struct sockaddr *) &name, &name_len);
			}

			_ret = new ip4_endpoint(ip4_address(name.sin_addr.s_addr), lwip_htons(name.sin_port));
			if(local) m_pEndPoint = _ret;

			return _ret;
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(ip4_endpoint local_ep) {
			if(m_iHandle == -1) return false;

			ip4_address ip = local_ep.get_ip();
			unsigned int port = local_ep.get_port();

			struct sockaddr_in addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = (in_addr_t)ip;
			m_iLastError = lwip_bind(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) );

			if(m_iLastError == -1) {
				ESP_LOGE("socket", "could not bind socket: %d", errno);
				return false;
			}
			fcntl(m_iHandle, F_SETFL, O_NONBLOCK);

			return true;
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(const unsigned int& port) {
			return bind(MNNET_IPENDPOINT4_ANY(port) );
		}

		//-----------------------------------
		// basic_ip4_socket::bind
		//-----------------------------------
		bool basic_ip4_socket::bind(ip4_address ip, const unsigned int& port) {
			return bind(MNNET_IPENDPOINT4(ip, port) );
		}

		//-----------------------------------
		// basic_ip4_socket::get_peername
		//-----------------------------------
		bool basic_ip4_socket::get_peername(ip4_address& ipPeerAddress, uint16_t& iPeerPort) {
			bool _ret = false;
			struct sockaddr_in stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			m_iLastError = lwip_getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(m_iLastError != 0) {
				ESP_LOGE("socket", "could not getpeername: %d", errno);
			} else {
				ipPeerAddress = ip4_address(stPeer.sin_addr.s_addr);
				iPeerPort = stPeer.sin_port;
				_ret = true;
			}
			return _ret;
		}

		//-----------------------------------
		// basic_ip4_socket::get_peername
		//-----------------------------------
		bool basic_ip4_socket::get_peername(ip4_endpoint& endpoint) {
			bool _ret = false;
			struct sockaddr_in stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			m_iLastError = lwip_getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(m_iLastError != 0) {
				ESP_LOGE("socket", "could not getpeername: %d", errno);
			} else {
				endpoint = ip4_endpoint(ip4_address(stPeer.sin_addr.s_addr), stPeer.sin_port);
				_ret = true;
			}
			return _ret;
		}


		//-----------------------------------
		// basic_ip6_socket::basic_ip6_socket
		//-----------------------------------
		basic_ip6_socket::basic_ip6_socket(handle_type& hndl, ip6_endpoint* endp)
			: basic_ip_socket(hndl) {
				m_pEndPoint = endp;
				m_iLastError = 0;
		}

		//-----------------------------------
		// basic_ip6_socket::basic_ip6_socket
		//-----------------------------------
		basic_ip6_socket::basic_ip6_socket(const socket_type& type, const protocol_type& protocol)
			: basic_ip_socket(address_family::inet_v6, type, protocol) {
				m_pEndPoint = nullptr;
		}

		//-----------------------------------
		// basic_ip6_socket::basic_ip6_socket
		//-----------------------------------
		basic_ip6_socket::basic_ip6_socket(const basic_ip6_socket& other)
			: basic_ip_socket( other.m_iHandle), m_pEndPoint(other.m_pEndPoint) {
			m_iLastError = other.m_iLastError;
		}

		//-----------------------------------
		// basic_ip6_socket::operator =
		//-----------------------------------
		basic_ip6_socket& basic_ip6_socket::operator = (const basic_ip6_socket& other) {
			m_iHandle = other.m_iHandle;
			m_iLastError = other.m_iLastError;
			m_pEndPoint = other.m_pEndPoint;

			return *this;
		}

		//-----------------------------------
		// basic_ip6_socket::get_endpoint
		//-----------------------------------
		ip6_endpoint* basic_ip6_socket::get_endpoint(bool local) {
			if(m_iHandle == -1) return nullptr;
			ip6_endpoint *_ret = nullptr;

			struct sockaddr_in6 name;
			unsigned int name_len = sizeof(name);


			if (local) {
				m_iLastError = lwip_getsockname(m_iHandle, (struct sockaddr *) &name, &name_len);

			} else {
				m_iLastError = lwip_getpeername(m_iHandle, (struct sockaddr *) &name, &name_len);
			}

			_ret = new ip6_endpoint(
					   ip6_address(name.sin6_addr.un.u32_addr),
					   lwip_htons(name.sin6_port));
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			_ret->set_scopeid(name.sin6_scope_id);
#endif
			if(local) m_pEndPoint = _ret;

			return _ret;
		}

		//-----------------------------------
		// basic_ip6_socket::bind
		//-----------------------------------
		bool basic_ip6_socket::bind(ip6_endpoint local_ep) {
			if(m_iHandle == -1) return false;

			ip6_address ip = local_ep.get_ip();
			unsigned int port = local_ep.get_port();

			struct sockaddr_in6 addr;
			memset((char *) &addr, 0, sizeof(addr));
			addr.sin6_family = AF_INET6;
			addr.sin6_port = htons(port);

			addr.sin6_addr.un.u32_addr[0] = ip[0];
			addr.sin6_addr.un.u32_addr[1] = ip[1];
			addr.sin6_addr.un.u32_addr[2] = ip[2];
			addr.sin6_addr.un.u32_addr[3] = ip[3];
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			addr.sin6_scope_id = ip.get_scopeid();
#endif
			m_iLastError = lwip_bind(m_iHandle, (struct sockaddr*)&addr, sizeof(addr) );

			if(m_iLastError == -1) {
				ESP_LOGE("socket v6", "could not bind socket: %d", errno);
				return false;
			}
			fcntl(m_iHandle, F_SETFL, O_NONBLOCK);

			return true;
		}

		//-----------------------------------
		// basic_ip6_socket::bind
		//-----------------------------------
		bool basic_ip6_socket::bind(const unsigned int& port) {
			return bind(MNNET_IPENDPOINT6_ANY(port) );
		}

		//-----------------------------------
		// basic_ip6_socket::bind
		//-----------------------------------
		bool basic_ip6_socket::bind(ip6_address ip, const unsigned int& port) {
			return bind(MNNET_IPENDPOINT6(ip, port) );
		}

		//-----------------------------------
		// basic_ip6_socket::get_peername
		//-----------------------------------
		bool basic_ip6_socket::get_peername(ip6_address& ipPeerAddress, uint16_t& iPeerPort) {
			bool _ret = false;
			struct sockaddr_in6 stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			m_iLastError = getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(m_iLastError != 0) {
				ESP_LOGE("socket v6", "could not getpeername: %d", errno);
			} else {
				ipPeerAddress = ip6_address(stPeer.sin6_addr.un.u32_addr);
				iPeerPort = stPeer.sin6_port;
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
				ipPeerAddress.set_scopeid(stPeer.sin6_scope_id);
#endif
				_ret = true;
			}
			return _ret;
		}

		//-----------------------------------
		// basic_ip6_socket::get_peername
		//-----------------------------------
		bool basic_ip6_socket::get_peername(ip6_endpoint& endpoint) {
			bool _ret = false;
			struct sockaddr_in6 stPeer;
			memset(&stPeer, 0, sizeof(struct sockaddr));
			socklen_t iLen = sizeof(sockaddr);

			m_iLastError = getpeername(m_iHandle, (struct sockaddr *)&stPeer, &iLen);
			if(m_iLastError != 0) {
				ESP_LOGE("socket", "could not getpeername: %d", errno);
			} else {
				endpoint = ip6_endpoint(ip6_address(stPeer.sin6_addr.un.u32_addr), stPeer.sin6_port);
				_ret = true;
			}
			return _ret;
		}
	}
}


