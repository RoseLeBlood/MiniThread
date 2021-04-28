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
		/**
		 * @brief This class provides an interface to a tcp IPv4 socket
		 * @ingroup socket
		 */
		class basic_stream_ip_socket : public basic_ip4_socket  {
		public:
			using self_type = basic_stream_ip_socket;
			using base_type = basic_ip4_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_stream_ip_socket()
				: base_type( socket_type::stream, protocol_type::unspec) { }


			/**
			 * @brief Establishes a connection to the stream server at the given address.
			 * @note Can also be used for UDP sockets. In this case, no connection is established. Instead,
			 * incoming and outgoing  packets are restricted to the specified address.
			 *
			 * @param remote_ep The adress to connect this socket
			 * @return true on connecte and false on error
			 */
			bool connect(endpoint_type remote_ep);
			/**
			 * @brief Puts the socket into listening state.
			 *
			 * @param backLog The maximum number of connections that can be queued for this socket.
			 * @return Return true when sucessfull put the socket in the listening state and false if not
			 */
			bool listen(int backLog = 8);

			/**
			 * @brief Gets the next completed connection from the socket's completed connection queue.
			 * @note If the queue is empty, waits until a connection request completes.
			 *
			 * @return A new basic_stream_ip_socket for the connection with the client.
			 */
			self_type* accept();

			/**
			 * @brief Receives data from the socket and stores it in buffer. Up to length bytes are received.
			 * @return Returns the number of bytes received.
			 */
			int recive(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return recive(buffer, 0, size, socketFlags); }
			/**
			 * @brief Receives data from the socket and stores it in buffer. Up to length bytes are received.
			 * @return Returns the number of bytes received.
			 */
			int recive(char* buffer, int offset, int size, socket_flags socketFlags);

			/**
			 * @brief Sends the contents of the given buffer.
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 *
			 * @param buffer 		The specific buffer to send
			 * @param size			The size of the buffer
			 * @param socketFlags	Socket sending optians
			 */
			int send_bytes(const void* buffer, int size, socket_flags socketFlags = socket_flags::none);

			/**
			 * @brief Sends the contents of the given buffer.
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 *
			 * @param buffer 		The specific buffer to send
			 * @param size			The size of the buffer
			 * @param socketFlags	Socket sending optians
			 * @param offset		buffer+offset
			 */
			int send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags = socket_flags::none);



		protected:
			basic_stream_ip_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_stream_ip_socket(const protocol_type& protocol)
				: base_type(socket_type::dgram, protocol) { }

		};

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		/**
		 * @brief This class provides an interface to a tcp IPv6 socket
		 * @ingroup socket
		 */
		class basic_stream_ip6_socket : public basic_ip6_socket  {
		public:
			using self_type = basic_stream_ip6_socket;
			using base_type = basic_ip6_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_stream_ip6_socket()
				: base_type( socket_type::stream, protocol_type::unspec) { }

			/**
			 * @brief Establishes a connection to the stream server at the given address.
			 * @note Can also be used for UDP sockets. In this case, no connection is established. Instead,
			 * incoming and outgoing  packets are restricted to the specified address.
			 *
			 * @param remote_ep The adress to connect this socket
			 * @return true on connecte and false on error
			 */
			bool connect(endpoint_type remote_ep);
			/**
			 * @brief Puts the socket into listening state.
			 *
			 * @param backLog The maximum number of connections that can be queued for this socket.
			 * @return Return true when sucessfull put the socket in the listening state and false if not
			 */
			bool listen(int backLog = 8);

			/**
			 * @brief Gets the next completed connection from the socket's completed connection queue.
			 * @note If the queue is empty, waits until a connection request completes.
			 *
			 * @return A new basic_stream_ip_socket for the connection with the client.
			 */
			self_type* accept();

			/**
			 * @brief Receives data from the socket and stores it in buffer. Up to length bytes are received.
			 * @return Returns the number of bytes received.
			 */
			int recive(char* buffer, int size, socket_flags socketFlags = socket_flags::none)
				{ return recive(buffer, 0, size, socketFlags); }
			/**
			 * @brief Receives data from the socket and stores it in buffer. Up to length bytes are received.
			 * @return Returns the number of bytes received.
			 */
			int recive(char* buffer, int offset, int size, socket_flags socketFlags);

			/**
			 * @brief Sends the contents of the given buffer.
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 *
			 * @param buffer 		The specific buffer to send
			 * @param size			The size of the buffer
			 * @param socketFlags	Socket sending optians
			 */
			int send_bytes(const void* buffer, int size, socket_flags socketFlags = socket_flags::none);

			/**
			 * @brief Sends the contents of the given buffer.
			 * @return Returns the number of bytes sent, which may be less than the number of bytes specified.
			 *
			 * @param buffer 		The specific buffer to send
			 * @param size			The size of the buffer
			 * @param socketFlags	Socket sending optians
			 * @param offset		buffer+offset
			 */
			int send_bytes(const void* buffer, int offset, int size, socket_flags socketFlags = socket_flags::none);


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
