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
#ifndef __MINILIB_NET_BASIC_IP4_SOCKET_H__
#define __MINILIB_NET_BASIC_IP4_SOCKET_H__

#include "../mn_config.hpp"
#include "mn_basic_socket.hpp"
#include "mn_basic_ip4_endpoint.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief This class represents an internet (IP) version 4 host socket.
		 * @ingroup socket
		 */
		class basic_ip4_socket : public basic_ip_socket {
		public:
			using handle_type = typename basic_ip_socket::handle_type;
			using endpoint_type = basic_ip4_endpoint;
			using ipaddress_type = basic_ip4_address;

			/**
			 * @brief Construct the wrapper class from a given lwip socket handle and the endpoint
			 * @param hndl The raw lwip socket handle.
			 * @param endp The endpoint for this socket
			 */
        	basic_ip4_socket(handle_type& hndl, endpoint_type* endp = nullptr) noexcept;
        	/**
        	 * @brief Construct create a IPv4 Socket AF_INET
        	 * @param type The type of the socket
        	 * @param protocol The using protocal for this socket
        	 */
        	basic_ip4_socket(const socket_type& type, const protocol_type& protocol = protocol_type::unspec) noexcept;
        	/**
        	 * @brief a copy construtor
        	 */
			basic_ip4_socket(const basic_ip4_socket& other) noexcept;
			/**
			 * @brief a copy operator
			 * @param other The other socket object
			 *
			 * @return Return this as reference
			 */
			basic_ip4_socket& operator=(const basic_ip4_socket& other) noexcept;

			/**
			 * @brief Bind the socket on the given endpoint (ip and port)
			 * @param ep The endpoint to bind this socket
			 *
			 * @return
			 * 		- true: bind the socket on the given endpoint
			 *		- false: bind error
			 */
			bool bind(endpoint_type local_ep, bool reuseAddress = false, bool reusePort = false);
			/**
			 * @brief Bind the socket
			 * @note bind on MNNET_IPV4_ADDRESS_ANY : port
			 *
			 * @param port The port to bind this socket
			 *
			 * @return
			 * 		- true: bind the socket on MNNET_IPV4_ADDRESS_ANY : port
			 *		- false: bind error
			 */
			bool bind(const unsigned int& port, bool reusePort = false);
			/**
			 * @brief Bind the socket on the given ip and port
			 *
			 * @param ip The IPv4 address to bind this socket
			 * @param port The port to bind this socket
			 *
			 * @return
			 * 		- true: success
			 *		- false: on error
			 */
			bool bind(ipaddress_type ip, const unsigned int& port, bool reuseAddress = false, bool reusePort = false);

			/**
			 * @brief Get the local or remote ip endpoint
			 *
			 * @param local When this flag true is then return the local endpoint
			 *
			 * @return The local or remote ip endpoint
			 */
			endpoint_type* 		get_endpoint(bool local);
			/**
			 * @brief Get the local endpoint
			 * @return The local endpoint
			 */
			endpoint_type* 		get_local()   				{ return get_endpoint(true); }
			/**
			 * @brief Get the remote endpoint
			 * @return The remote endpoint
			 */
        	endpoint_type* 		get_remote()  				{ return get_endpoint(false); }

        	/**
			 * @brief Get the remote endpoint
			 * @param endpoint The holder of the returned endpoint
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		  		get_peername(endpoint_type& endpoint);
        	/**
			 * @brief Get the remote endpoint
			 * @param ipPeerAddress The holder of the returned ip4 address
			 * @param iPeerPort The holder of the returned port
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		   		get_peername(ipaddress_type& ipPeerAddress, uint16_t& iPeerPort);
        	/**
			 * @brief Get a copy of this socket
			 * @return A copy of this socket
			 */
        	virtual basic_ip_socket*	get_copy() 					{ return new basic_ip4_socket(*this); }


        	virtual void swap(basic_ip4_socket& rhs) noexcept {
        		basic_ip_socket::swap(rhs);
				mn::swap(m_pEndPoint, rhs.m_pEndPoint);
			}
		protected:
			/**
			 * @brief A saved / cached copy of the endpoint on binde the socket
			 */
			endpoint_type* m_pEndPoint;
		};
	}
}

#endif // __MINILIB_NET_BASIC_IP4_SOCKET_H__
