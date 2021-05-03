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

#define MNTHREAD_NET_INVALID_SOCKET -1

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
			explicit basic_ip_socket(const handle_type& hndl) noexcept;
			/**
			 * @brief construtor create the socket
			 * @param fam 		The address family for the creating socket
			 * @param type 		The type for the creating  socket
			 * @param protocol 	The protocol for the creating socket
			 */
			basic_ip_socket(const address_family& fam, const socket_type& type, const protocol_type& protocol) noexcept;
			/**
			 * @brief copy construtor
			 */
			basic_ip_socket(const basic_ip_socket& other) noexcept;
			/**
			 * @brief deconstrutor, close the socket
			 */
			virtual ~basic_ip_socket() noexcept;

			/**
			 * @brief Open the socket - only used when the socket not initialized
			 * @return
			 *		- True 		The socket are created
			 *		- False		If not
			 */
			virtual bool			  	open();


			/**
			 * Close and destroy the socket
			 */
			virtual void 				close() 		 { lwip_close(m_iHandle); }
			/**
			 * @brief Returns the number of bytes available that can be read without causing the socket to block.
			 * @return The number of bytes are available in the buffer.
			 */
			virtual int 				available();

			/**
			 * @brief Reset the socket
			 * @param hnd The new raw socket handle
			 * @param bClosed If true and socket initialized then close this socket, after this will set the new handle
			 */
			virtual void				reset(const handle_type& hnd = MNTHREAD_NET_INVALID_SOCKET, bool bClosed = false);

			/**
			 * @brief If the socket initialized
			 * @return
			 *		- True: The socket is initialized
			 *		- False: If not
			 */
			virtual bool 				initialized();

			/**
			 * @brief Get the saved copy of the last lwip error code
			 * @note Attentions the error codes are decode in raw socket type, not in ERR_XX_XXXX error types
			 * @param poolLast If this flag true then get the last error from the Socket API, if false then get
			 * the last saved error from m_iLastError
			 * @return The saved copy of the last lwip error code
			 */
			int 						get_last_error(bool poolLast = true);
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


        	virtual void swap(basic_ip_socket& rhs) noexcept {
				mn::swap(m_iHandle, rhs.m_iHandle);
				mn::swap(m_eFam, rhs.m_eFam);
				mn::swap(m_eType, rhs.m_eType);
				mn::swap(m_eProtocol, rhs.m_eProtocol);
				mn::swap(m_bBlocked, rhs.m_bBlocked);
			}

			/**
			 * @brief send shutdown
			 * @param cmd The type of shutdown
			 */
			void shutdown(const socket_shutdown_type& cmd);

			/**
			 * @brief poll the socket
			 *
			 * @param timeout How many micros to poll
			 * @param mode poll mode
			 *
			 * @return
			 *		- true: on success
			 *		- false: if not success
			 */
			bool poll(const unsigned long& timeout, int mode);
			/**
        	 * @brief Set the option socket_option_name::reuse_addr
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_reuse_address(bool flag);
        	/**
        	 * @brief Set the option socket_option_name::reuse_port
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_rause_port(bool flag);
        	/**
        	 * @brief Set the option socket_option_name::linger
        	 * @param on if true then enable the option and false when not
        	 * @param seconds Set the seconds are to wait
        	 */
        	void set_linger(bool on, int seconds);
        	/**
        	 * @brief Set the option socket_option_name::tcp_nodelay
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_no_delay(bool flag);
        	/**
        	 * @brief Set the option socket_option_name::keepalive
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_keep_alive(bool flag);
        	/**
        	 * @brief Set the option socket_option_name::oob_inline
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_oob_inline(bool flag);
        	/**
        	 * @brief Set the socket for bloking
        	 * @param flag If true then blocking and if false when not
        	 */
        	void set_blocking(bool flag);
        	/**
        	 * @brief Set the option socket_option_name::no_check
        	 * @param flag if true then enable the option and false when not
        	 */
        	void set_nocheak(bool value);
			/**
        	 * @brief Set the size of the send buffer
        	 * @param value The size of the buffer in bytes
        	 */
        	void set_send_buffer_size(int value);
        	/**
        	 * @brief Set the size of the recive buffer
        	 * @param value The size of the buffer in bytes
        	 */
        	void set_recive_buffer_size(int value);

        	/**
        	 * @brief Set the option socket_option_name::send_timeout
        	 * @param value The value for the option in millis
        	 */
        	void set_send_timeout(int value);
        	/**
        	 * @brief Set the option socket_option_name::recive_timeout
        	 * @param value The value for the option in millis
        	 */
        	void set_recive_timeout(int value);


        	/**
        	 * @brief Get the value of the option socket_option_name::reuse_addr
        	 * @return The value of the option socket_option_name::reuse_addr
        	 */
        	bool get_reuse_address();
        	/**
        	 * @brief Get the value of the option socket_option_name::reuse_port
        	 * @return The value of the option socket_option_name::reuse_port
        	 */
        	bool get_rause_port();
        	/**
        	 * @brief Get the value of the option socket_option_name::linger
        	 * @param[out] on if true then is the option enable and false when not
        	 * @param[out] seconds The setted seconds
        	 */
        	void get_linger(bool& on, int& seconds);

        	/**
        	 * @brief Get the value of the option socket_option_name::no_delay
        	 * @return The value of the option socket_option_name::no_delay
        	 */
        	bool get_no_delay();
        	/**
        	 * @brief Get the value of the option socket_option_name::keepalive
        	 * @return The value of the option socket_option_name::keepalive
        	 */
        	bool get_keep_alive();
        	/**
        	 * @brief Get the value of the option socket_option_name::oob_inline
        	 * @return The value of the option socket_option_name::oob_inline
        	 */
        	bool get_oob_inline();
        	/**
        	 * @brief Get the value of the option socket_option_name::no_check
        	 * @return If true then Is the socket set as blocked and if false when not
        	 */
        	bool get_nocheak();

        	/**
        	 * @brief Is the socket blocking enabled
        	 * @return If true then blocking and if false when not
        	 */
        	bool get_blocking() { return m_bBlocked; }
        	/**
        	 * @brief Get the size of the send buffer
        	 * @return The size of the buffer in bytes
        	 */
        	int get_send_buffer_size();
        	/**
        	 * @brief Get the size of the recive buffer
        	 * @return The size of the buffer in bytes
        	 */
        	int get_recive_buffer_size();

        	/**
        	 * @brief Set the option socket_option_name::send_timeout
        	 * @return The millis for the option in millis
        	 */
        	int get_send_timeout();
        	/**
        	 * @brief Set the option socket_option_name::recive_timeout
        	 * @return The millis for the option in millis
        	 */
        	int get_recive_timeout();
		public:
			/**
        	 * @brief Sets the socket option specified by level and option to the given integer value.
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
			int set_options(const socket_option_level& opt, const socket_option_name& name, int value);

			/**
        	 * @brief Sets the socket option specified by level and option to the given integer value.
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
			int set_options(const socket_option_level& opt, const socket_option_name& name, unsigned int value);

			/**
        	 * @brief Sets the socket option specified by level and option to the given integer value.
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
			int set_options(const socket_option_level& opt, const socket_option_name& name, basic_ip4_address value);

			/**
        	 * @brief Sets the socket option specified by level and option to the given integer value.
        	 * @param opt The socket option level
        	 * @param name The name of the option
        	 * @param value The value for the option
        	 * @return The socket error code. If 0 then sucess
        	 */
			int set_options(const socket_option_level& opt, const socket_option_name& name, basic_ip6_address value);

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
        	int set_options(const socket_option_level& opt, const socket_option_name& name,
							void* value, uint32_t size);

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
        	int get_option_raw(const socket_option_level& opt, const socket_option_name& name, void* value,
								uint32_t size);
		public:
			/**
			 * @brief A wrapper for the ioctl system call.
			 */
			int ioctl(const ioctl_request_type& request, int& arg);
			/**
			 * @brief A wrapper for the ioctl system call.
			 */
			int ioctl(const ioctl_request_type& request, void* arg);

			/**
			 * @brief A wrapper for the fcntl system call.
			 */
			int set_fcntl(int arg);

			/**
			 * @brief A wrapper for the fcntl system call.
			 */
			int get_fcntl();

		protected:
			/**
			 * @brief Open the socket - only used when the socket not initialized
			 * @param fam 		The address family for the creating socket
			 * @param type 		The type for the creating  socket
			 * @param protocol 	The protocol for the creating socket
			 * @return
			 *		- True 		The socket are created
			 *		- False		If not
			 */
			virtual bool			  	open(const address_family& fam, const socket_type& type,
											 const protocol_type& protocol);


		protected:
			/**
        	 * @brief Reference to the underlying socket handle for this socket.
        	 * @note Can be obtained from get_handle().
        	 */
			handle_type m_iHandle;

			/**
			 * @brief  A saved / cached copy of the address family type of this socket.
			 */
			address_family 	m_eFam;
			/**
			 * @brief  A saved / cached copy of the socket type of this socket.
			 */
			socket_type 	m_eType;
			/**
			 * @brief  A saved / cached copy of the protocal type of this socket.
			 */
			protocol_type 	m_eProtocol;

			/**
			 *  @brief A saved / cached copy  of a blocked flag
			 */
			bool m_bBlocked;
		};

	}
}



#endif // _MINILIB_BASIC_SOCKET_HPP_
