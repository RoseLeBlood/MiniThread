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
#ifndef __MINILIB_BASIC_STREAM_IP_SOCKET_H__
#define __MINILIB_BASIC_STREAM_IP_SOCKET_H__

#include "../mn_config.hpp"
#include "mn_basic_socket.hpp"
#include "mn_basic_ip4_socket.hpp"
#include "mn_basic_ip6_socket.hpp"

namespace mn {
	namespace net {
		class basic_stream_ip_socket : public basic_ip4_socket  {
		public:
			using self_type = basic_stream_ip_socket;
			using base_type = basic_ip4_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_stream_ip_socket()
				: base_type( socket_type::stream, protocol_type::unspec) { }

			int recive(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return recive(buffer, 0, size, socketFlags); }
			int send(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return send(buffer, 0, size, socketFlags); }

			int send(char* buffer, int offset, int size, socket_flags socketFlags);
			int recive(char* buffer, int offset, int size, socket_flags socketFlags);

			bool connect(ip4_endpoint remote_ep);
			bool listen(int backLog);

			self_type* accept();
		protected:
			basic_stream_ip_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_stream_ip_socket(const protocol_type& protocol)
				: base_type(socket_type::dgram, protocol) { }

		};

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		class basic_stream_ip6_socket : public basic_ip6_socket  {
		public:
			using self_type = basic_stream_ip6_socket;
			using base_type = basic_ip6_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_stream_ip6_socket()
				: base_type( socket_type::stream, protocol_type::unspec) { }

			int recive(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return recive(buffer, 0, size, socketFlags); }
			int send(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return send(buffer, 0, size, socketFlags); }

			int send(char* buffer, int offset, int size, socket_flags socketFlags);
			int recive(char* buffer, int offset, int size, socket_flags socketFlags);

			bool connect(endpoint_type remote_ep);
			bool listen(int backLog);

			self_type* accept();
		protected:
			basic_stream_ip6_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_stream_ip6_socket(const protocol_type& protocol)
				: base_type(socket_type::dgram, protocol) { }
		};
	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
	}
}

#endif // __MINILIB_BASIC_STREAM_IP_SOCKET_H__
