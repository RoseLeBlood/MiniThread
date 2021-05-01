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
#ifndef _MINILIB_BASIC_DGRAM_SOCKET_H_
#define _MINILIB_BASIC_DGRAM_SOCKET_H_

#include "../mn_config.hpp"
#include "mn_basic_socket.hpp"
#include "mn_basic_ip4_socket.hpp"
#include "mn_basic_ip6_socket.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief Basic dram socket class
		 * @ingroup socket
		 */
		class basic_dgram_ip_socket : public basic_ip4_socket {
		public:
			using self_typ = basic_dgram_ip_socket;
			using base_type = basic_ip4_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			/**
			 * @brief Construt
			 */
			basic_dgram_ip_socket()
				: base_type( socket_type::dgram, protocol_type::unspec) { }

			/**
			 * @brief send a buffer of data to the given enpoint
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param ep The endpoint to send the biffer
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 */
			int send_to(char* buffer, int size, endpoint_type& ep, const socket_flags& socketFlags  = socket_flags::none)
				{ return send_to(buffer, 0, size, socketFlags, ep); }

			/**
			 * @brief recive a buffer of data
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param[out] ep The endpoint from recive the data
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes received.
			 */
			int recive_from(char* buffer, int size, endpoint_type* ep, const socket_flags& socketFlags  = socket_flags::none)
				{ return recive_from(buffer, 0, size, socketFlags, ep); }

			/**
			 * @brief recive a buffer of data
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param[out] ep The endpoint from recive the data
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes received.
			 */
			int recive_from(char* buffer, int offset, int size, const socket_flags& socketFlags, endpoint_type* ep);

			/**
			 * @brief send a buffer of data to the given enpoint
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param offset sending buffer offset (buffer + offset)
			 * @param ep The endpoint to send the biffer
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 */
			int send_to(char* buffer, int offset, int size, const socket_flags& socketFlags, endpoint_type& ep);

		protected:
			basic_dgram_ip_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_dgram_ip_socket(const protocol_type& protocol)
				: base_type(socket_type::dgram, protocol) { }
		private:
        	//ipaddress_type m_ipMultiCast;
		};
#if LWIP_UDP && LWIP_UDPLITE
		/**
		 * @brief Basic dram lite socket class
		 * @ingroup socket
		 */
		class basic_dgramlite_ip_socket : public basic_dgram_ip_socket {
		public:
			basic_dgramlite_ip_socket() : basic_dgram_ip_socket(protocol_type::udp_lite) { }
		};
#endif

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		/**
		 * @brief Basic dram socket class
		 * @ingroup socket
		 */
		class basic_dgram_ip6_socket : public basic_ip6_socket {
		public:
			using self_typ = basic_dgram_ip_socket;
			using base_type = basic_ip6_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_dgram_ip6_socket()
				: base_type( socket_type::dgram, protocol_type::unspec) { }

			/**
			 * @brief send a buffer of data to the given enpoint
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param ep The endpoint to send the biffer
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 */
			int send_to(char* buffer, int size, endpoint_type* ep, socket_flags socketFlags  = socket_flags::none)
				{ return send_to(buffer, 0, size, socketFlags, ep); }

			/**
			 * @brief recive a buffer of data
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param[out] ep The endpoint from recive the data
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes received.
			 */
			int recive_from(char* buffer, int size, endpoint_type* ep, socket_flags socketFlags  = socket_flags::none)
				{ return recive_from(buffer, 0, size, socketFlags, ep); }

			/**
			 * @brief recive a buffer of data
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param[out] ep The endpoint from recive the data
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes received.
			 */
			int recive_from(char* buffer, int offset, int size, socket_flags socketFlags, endpoint_type* ep);
			/**
			 * @brief send a buffer of data to the given enpoint
			 *
			 * @param buffer The buffer to send
			 * @param size The size of sending buffer
			 * @param offset sending buffer offset (buffer + offset)
			 * @param ep The endpoint to send the biffer
			 * @param socketFlags The options for send
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 */
			int send_to(char* buffer, int offset, int size, socket_flags socketFlags, endpoint_type* ep);
		protected:
			basic_dgram_ip6_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_dgram_ip6_socket(const protocol_type& protocol)
				: base_type(socket_type::dgram, protocol) { }
		};
	#if LWIP_UDP && LWIP_UDPLITE
		/**
		 * Basic dram lite socket class, for ip6adress
		 * @ingroup socket
		 */
		class basic_dgramlite_ip6_socket : public basic_dgram_ip6_socket {
		public:
			basic_dgramlite_ip6_socket() : basic_dgram_ip6_socket(protocol_type::udp_lite) { }
		};
	#endif // LWIP_UDP


#endif
	}
}

#endif // _MINILIB_BASIC_DGRAM_SOCKET_H_
