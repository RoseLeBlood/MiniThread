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

		/**
		 * @brief Wrapper class around lwip implementation of a socket.
		 *
		 * @note To use this, you need to subclass it. All of your ip sockets should
   		 * be derived from the basic_ip_socket class. Then implement the virtual get_copy
		 * function.
		 *
		 * @ingroup socket
		 * @author Amber-Sophia Schröck
		 */
		class basic_ip_socket {
		public:
			using handle_type = int;

			/**
			 * @brief construtor from a raw handle
			 * @param hndl The raw handle
			 */
			explicit basic_ip_socket(const handle_type& hndl);
			/**
			 * @brief construtor create the socket
			 * @param fam 		The address family for the creating socket
			 * @param type 		The type for the creating  socket
			 * @param protocol 	The protocol for the creating socket
			 */
			basic_ip_socket(const address_family& fam, const socket_type& type, const protocol_type& protocol);
			/**
			 * @brief copy construtor
			 */
			basic_ip_socket(const basic_ip_socket& other);
			/**
			 * @brief deconstrutor, close the socket
			 */
			virtual ~basic_ip_socket();

			/**
			 * Close and destroy the socket
			 */
			virtual void 				close() 		 { lwip_close(m_iHandle); }
			/**
			 * @brief How many bytes in the recive buffer available.
			 * @return The number of bytes are available in the buffer.
			 */
			virtual int 				available();

			/**
			 * @brief Get the saved copy of the last lwip error code
			 * @note Attentions the error codes are decode in raw socket type, not in ERR_XX_XXXX error types
			 * @return The saved copy of the last lwip error code
			 */
			int 						get_last_error() { return m_iLastError; }
			/**
			 * @brief Get the raw socket handle.
			 * @return The raw socket handle.
			 */
        	int 						get_handle() 	 { return m_iHandle; }
        	/**
			 * @brief Get a copy of the socket wrapper (abstarct)
			 * @return The copy
			 */
        	virtual basic_ip_socket* 	get_copy() = 0;


        	basic_ip_socket& operator = (const basic_ip_socket& other);

        	/**
        	 * @brief Set the option socket_option_name::no_check
        	 * @param value The value
        	 */
        	void set_nocheak(bool value) {
				set_options(socket_option_level::socket, socket_option_name::no_check, value);
        	}
        	/**
        	 * @brief Get the option socket_option_name::no_check
        	 * @return
        	 *		- true: The option is enable
        	 *		- false: The option is disable
        	 */
        	bool get_nocheak() {
				return get_option_bool(socket_option_level::socket, socket_option_name::no_check);
        	}
		public:
			/**
        	 * @brief Set a given option, version for interger values
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
			int set_options(const socket_option_level& opt, const socket_option_name& name, int value);
			/**
        	 * @brief Set a given option, version for boolen values
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
        	int set_options(const socket_option_level& opt, const socket_option_name& name, bool value);
        	/**
        	 * @brief Set a given option, version for data
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The pointer of data for the option
        	 * @param size The size of the pointer
        	 * @return The socket error code. If 0 then sucess
        	 */
        	int set_options(const socket_option_level& opt, const socket_option_name& name, void* value, uint32_t size);

        	/**
        	 * @brief Get a interger value of a given option
        	 * @param opt The socket option level
        	 * @param name The name of the option
			 *
        	 * @return The a interger value of a given option
        	 */
        	int get_option_int(const socket_option_level& opt, const socket_option_name& name);
        	/**
        	 * @brief Get a booles value of a given option
        	 * @param opt The socket option level
        	 * @param name The name of the option
			 *
        	 * @return The a boolen value of a given option
        	 */
        	bool get_option_bool(const socket_option_level& opt, const socket_option_name& name);
        	/**
        	 * @brief Get a buffer value of a given option
        	 *
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param[out] value Contains the return of the given options, null when error
        	 * @param size The size of the value
			 *
        	 */
        	void get_option_ex(const socket_option_level& opt, const socket_option_name& name, void* value, uint32_t size);
		protected:
			/**
        	 * @brief Reference to the underlying socket handle for this socket.
        	 * @note Can be obtained from get_handle().
        	 */
			handle_type m_iHandle;
			/**
        	 * @brief Save a copy of the last lwip error code
        	 * @note Can be obtained from get_last_error().
        	 */
			int 		m_iLastError;
		};

		/**
		 * Wrapper class around lwip implementation of a ip4 socket.
		 */
		class basic_ip4_socket : public basic_ip_socket {
		public:
			using handle_type = typename basic_ip_socket::handle_type;
			using endpoint_type = ip4_endpoint;
			using ipaddress_type = ip4_address;

			/**
			 * @brief Construct the wrapper class from a given lwip socket handle and the endpoint
			 * @param hndl The raw lwip socket handle.
			 * @param endp The endpoint for this socket
			 */
        	basic_ip4_socket(handle_type& hndl, ip4_endpoint* endp = nullptr);
        	/**
        	 * @brief Construct create a IPv4 Socket AF_INET
        	 * @param type The type of the socket
        	 * @param protocol The using protocal for this socket
        	 */
        	basic_ip4_socket(const socket_type& type, const protocol_type& protocol = protocol_type::unspec);
        	/**
        	 * @brief a copy construtor
        	 */
			basic_ip4_socket(const basic_ip4_socket& other);
			/**
			 * @brief a copy operator
			 * @param other The other socket object
			 *
			 * @return Return this as reference
			 */
			basic_ip4_socket& operator=(const basic_ip4_socket& other);

			/**
			 * @brief Bind the socket on the given endpoint (ip and port)
			 * @param ep The endpoint to bind this socket
			 *
			 * @return
			 * 		- true: bind the socket on the given endpoint
			 *		- false: bind error
			 */
			bool bind(ip4_endpoint local_ep);
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
			bool bind(const unsigned int& port);
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
			bool bind(ip4_address ip, const unsigned int& port);

			/**
			 * @brief Get the local or remote ip endpoint
			 *
			 * @param local When this flag true is then return the local endpoint
			 *
			 * @return The local or remote ip endpoint
			 */
			ip4_endpoint* 		get_endpoint(bool local);
			/**
			 * @brief Get the local endpoint
			 * @return The local endpoint
			 */
			ip4_endpoint* 		get_local()   				{ return get_endpoint(true); }
			/**
			 * @brief Get the remote endpoint
			 * @return The remote endpoint
			 */
        	ip4_endpoint* 		get_remote()  				{ return get_endpoint(false); }

        	/**
			 * @brief Get the remote endpoint
			 * @param endpoint The holder of the returned endpoint
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		  		get_peername(ip4_endpoint& endpoint);
        	/**
			 * @brief Get the remote endpoint
			 * @param ipPeerAddress The holder of the returned ip4 address
			 * @param iPeerPort The holder of the returned port
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		   		get_peername(ip4_address& ipPeerAddress, uint16_t& iPeerPort);
        	/**
			 * @brief Get a copy of this socket
			 * @return A copy of this socket
			 */
        	basic_ip_socket*	get_copy() 					{ return new basic_ip4_socket(*this); }
		protected:
			/**
			 * @brief A saved / cached copy of the endpoint on binde the socket
			 */
			ip4_endpoint* m_pEndPoint;
		};

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

		/**
		 * Wrapper class around lwip implementation of a ip6 socket.
		 */
		class basic_ip6_socket : public basic_ip_socket {
		public:
			using handle_type = typename basic_ip_socket::handle_type;
			using endpoint_type = ip6_endpoint;
			using ipaddress_type = ip6_address;

			/**
			 * @brief Construct the wrapper class from a given lwip socket handle and the endpoint
			 * @param hndl The raw lwip socket handle.
			 * @param endp The endpoint for this socket
			 */
        	basic_ip6_socket(handle_type& hndl, ip6_endpoint* endp = nullptr);
        	/**
        	 * @brief Construct create a IPv4 Socket AF_INET
        	 * @param type The type of the socket
        	 * @param protocol The using protocal for this socket
        	 */
        	basic_ip6_socket(const socket_type& type, const protocol_type& protocol);
        	/**
        	 * @brief a copy construtor
        	 */
			basic_ip6_socket(const basic_ip6_socket& other);
			/**
			 * @brief a copy operator
			 * @param other The other socket object
			 *
			 * @return Return this as reference
			 */
			basic_ip6_socket& operator=(const basic_ip6_socket& other);


			/**
			 * @brief Bind the socket on the given endpoint (ip and port)
			 * @param ep The endpoint to bind this socket
			 *
			 * @return
			 * 		- true: bind the socket on the given endpoint
			 *		- false: bind error
			 */
			bool bind(ip6_endpoint local_ep);
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
			bool bind(const unsigned int& port);
			/**
			 * @brief Bind the socket on the given ip and port
			 *
			 * @param ip The IPv6 address to bind this socket
			 * @param port The port to bind this socket
			 *
			 * @return
			 * 		- true: success
			 *		- false: on error
			 */
			bool bind(ip6_address ip, const unsigned int& port);

			/**
			 * @brief Get the local or remote ip endpoint
			 *
			 * @param local When this flag true is then return the local endpoint
			 *
			 * @return The local or remote ip endpoint
			 */
			ip6_endpoint* 		get_endpoint(bool local);
			/**
			 * @brief Get the local endpoint
			 * @return The local endpoint
			 */
			ip6_endpoint* 		get_local()   				{ return get_endpoint(true); }
			/**
			 * @brief Get the remote endpoint
			 * @return The remote endpoint
			 */
        	ip6_endpoint* 		get_remote()  				{ return get_endpoint(false); }

        	/**
			 * @brief Get the remote endpoint
			 * @param endpoint The holder of the returned endpoint
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		  		get_peername(ip6_endpoint& endpoint);
        	/**
			 * @brief Get the remote endpoint
			 * @param ipPeerAddress The holder of the returned ip4 address
			 * @param iPeerPort The holder of the returned port
			 *
			 * @return if true then success and if false on error
			 */
        	bool 		   		get_peername(ip6_address& ipPeerAddress, uint16_t& iPeerPort);
        	/**
			 * @brief Get a copy of this socket
			 * @return A copy of this socket
			 */
        	basic_ip_socket*	get_copy() 					{ return new basic_ip6_socket(*this); }
		protected:
			/**
			 * @brief A saved / cached copy of the endpoint on binde the socket
			 */
			ip6_endpoint* m_pEndPoint;
		};
		/**
		 * @brief using the class basic_ip6_socket as ip6_socket type
		 */
		using ip6_socket = basic_ip6_socket;
#endif
		/**
		 * @brief using the class basic_ip4_socket as ip4_socket type
		 */
		using ip4_socket = basic_ip4_socket;
	}
}



#endif // _MINILIB_BASIC_SOCKET_HPP_
