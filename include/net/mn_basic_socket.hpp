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
#ifndef _MINILIB_BASIC_SOCKET_HPP_
#define _MINILIB_BASIC_SOCKET_HPP_

#include "../mn_config.hpp"
#include "../mn_autolock.hpp"

#include <sdkconfig.h>
#include <lwip/api.h>
#include <lwip/sockets.h>
#include <lwip/udp.h>

#include "mn_basic_endpoint.hpp"

#ifndef UDPLITE_SEND_CSCOV
#define UDPLITE_SEND_CSCOV 0x01
#endif // UDPLITE_SEND_CSCOV

#ifndef UDPLITE_RECV_CSCOV
#define UDPLITE_RECV_CSCOV 0x02
#endif // UDPLITE_RECV_CSCOV

namespace mn {
	namespace net {
		class basic_ip_socket {
		public:
			using handle_type = int;

			basic_ip_socket();
			explicit basic_ip_socket(const handle_type& hndl);
			basic_ip_socket(const address_family& fam, const socket_type& type, const protocol_type& protocol);
			basic_ip_socket(const basic_ip_socket& other);
			virtual ~basic_ip_socket();

			virtual void 				close() 		 { lwip_close(m_iHandle); }
			virtual int 				available();

			int 						get_last_error() { return m_iLastError; }
        	int 						get_handle() 	 { return m_iHandle; }
        	virtual basic_ip_socket* 	get_copy() = 0;

        	operator int () 			{ return m_iHandle; }
        	basic_ip_socket& operator = (const basic_ip_socket& other);

        	void set_nocheak(bool value) {
				set_options(socket_option_level::socket, socket_option_name::no_check, value);
        	}
        	bool get_nocheak() {
				return get_option_bool(socket_option_level::socket, socket_option_name::no_check);
        	}
		public:
			int set_options(const socket_option_level& opt, const socket_option_name& name, int value);
        	int set_options(const socket_option_level& opt, const socket_option_name& name, bool value);
        	int set_options(const socket_option_level& opt, const socket_option_name& name, void* value, uint32_t size);

        	int get_option_int(const socket_option_level& opt, const socket_option_name& name);
        	bool get_option_bool(const socket_option_level& opt, const socket_option_name& name);
        	void get_option_ex(const socket_option_level& opt, const socket_option_name& name, void* value, uint32_t size);
		protected:
			handle_type m_iHandle;
			int 		m_iLastError;
		};
		//using ip4_endpoint = basic_ip4_endpoint;
		class basic_ip4_socket : public basic_ip_socket {
		public:
			using handle_type = typename basic_ip_socket::handle_type;
			using endpoint_type = ip4_endpoint;
			using ipaddress_type = ip4_address;

        	basic_ip4_socket(handle_type& hndl, ip4_endpoint* endp = nullptr);
        	basic_ip4_socket(const socket_type& type, const protocol_type& protocol);
			basic_ip4_socket(const basic_ip4_socket& other);
			basic_ip4_socket& operator=(const basic_ip4_socket& other);
			virtual ~basic_ip4_socket() { close(); }

			bool bind(ip4_endpoint local_ep);
			bool bind(const unsigned int& port);
			bool bind(ip4_address ip, const unsigned int& port);

			ip4_endpoint* 		get_endpoint(bool local);
			ip4_endpoint* 		get_local()   				{ return get_endpoint(true); }
        	ip4_endpoint* 		get_remote()  				{ return get_endpoint(false); }
        	bool 		  		get_peername(ip4_endpoint& endpoint);
        	bool 		   		get_peername(ip4_address& strPeerAddress, uint16_t& iPeerPort);
        	basic_ip_socket*	get_copy() 					{ return new basic_ip4_socket(*this); }
		protected:
			ip4_endpoint* m_pEndPoint;
		};

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		class basic_ip6_socket : public basic_ip_socket {
		public:
			using handle_type = typename basic_ip_socket::handle_type;
			using endpoint_type = ip6_endpoint;
			using ipaddress_type = ip6_address;

        	basic_ip6_socket(handle_type& hndl, ip6_endpoint* endp = nullptr);
        	basic_ip6_socket(const socket_type& type, const protocol_type& protocol);
			basic_ip6_socket(const basic_ip6_socket& other);
			basic_ip6_socket& operator=(const basic_ip6_socket& other);
			virtual ~basic_ip6_socket() { close(); }

			bool bind(ip6_endpoint local_ep);
			bool bind(const unsigned int& port);
			bool bind(ip6_address ip, const unsigned int& port);

			ip6_endpoint* 		get_endpoint(bool local);
			ip6_endpoint* 		get_local()   				{ return get_endpoint(true); }
        	ip6_endpoint* 		get_remote()  				{ return get_endpoint(false); }
        	bool 		  		get_peername(ip6_endpoint& endpoint);
        	bool 		   		get_peername(ip6_address& strPeerAddress, uint16_t& iPeerPort);
        	basic_ip_socket*	get_copy() 					{ return new basic_ip6_socket(*this); }
		protected:
			ip6_endpoint* m_pEndPoint;
		};

		using ip6_socket = basic_ip6_socket;
#endif
		using ip4_socket = basic_ip4_socket;
	}
}



#endif // _MINILIB_BASIC_SOCKET_HPP_
