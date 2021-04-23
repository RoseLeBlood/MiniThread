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
#include "mn_micros.hpp"
#include "net/mn_basic_socket.hpp"

namespace mn {
	namespace net {
		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::basic_ip_socket(const handle_type& hndl)
			: m_iHandle(hndl) { }

		//-----------------------------------
		// basic_ip_socket::basic_ip_socket
		//-----------------------------------
		basic_ip_socket::basic_ip_socket(const address_family& fam, const socket_type& type,
										const protocol_type& protocol) {
			 m_eFam = fam;
			 m_eType = type;
			 m_eProtocol = protocol;

			 m_iHandle = lwip_socket(static_cast<int>(fam), static_cast<int>(type), static_cast<int>(protocol) );
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
			: m_iHandle(other.m_iHandle) {
			m_eFam = other.m_eFam;
			m_eType = other.m_eType;
			m_eProtocol = other.m_eProtocol;
		}

		//-----------------------------------
		// basic_ip_socket::available
		//-----------------------------------
		int basic_ip_socket::available() {
			if(!initialized()) return 0;

			int cBytes = 0;
			 ioctl(ioctl_request_type::available, &cBytes);
			return cBytes;
		}

		//-----------------------------------
		// basic_ip_socket::reset
		//-----------------------------------
		void basic_ip_socket::reset(const handle_type& handl, bool bClosed) {
			if(initialized() && bClosed) close();

			m_iHandle = handl;
		}

		//-----------------------------------
		// basic_ip_socket::open
		//-----------------------------------
		bool basic_ip_socket::open()  {
			if(!initialized()) return true;

			m_iHandle = lwip_socket(static_cast<int>(m_eFam), static_cast<int>(m_eType),
									static_cast<int>(m_eProtocol) );

			return m_iHandle != MNTHREAD_NET_INVALID_SOCKET;
		}

		//-----------------------------------
		// basic_ip_socket::open
		//-----------------------------------
		bool basic_ip_socket::open(const address_family& fam, const socket_type& type,
											 const protocol_type& protocol)  {
			if(!initialized()) return true;


			if(fam != m_eFam) m_eFam = fam;
			if(type != m_eType) m_eType = type;
			if(protocol != m_eProtocol) m_eProtocol = protocol;

			m_iHandle = lwip_socket(static_cast<int>(m_eFam), static_cast<int>(m_eType),
									static_cast<int>(m_eProtocol) );


			return m_iHandle != MNTHREAD_NET_INVALID_SOCKET;
		}

		//-----------------------------------
		// basic_ip_socket::initialized
		//-----------------------------------
		bool basic_ip_socket::initialized()  {
			return m_iHandle != MNTHREAD_NET_INVALID_SOCKET;
		}
		//-----------------------------------
		// basic_ip_socket::get_last_error
		//-----------------------------------
		int basic_ip_socket::get_last_error(bool poolLast) {
			return get_option_int(socket_option_level::socket, socket_option_name::error);
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name, int value) {
			if(!initialized()) return -1;

			socklen_t _size = sizeof(value);
			return ( setsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, _size));
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name, bool flag) {
			int _value = flag ? 0 : 1;

			return set_options(opt, name, _value);
		}

		//-----------------------------------
		// basic_ip_socket::set_options
		//-----------------------------------
		int basic_ip_socket::set_options(const socket_option_level& opt, const socket_option_name& name,
										void* value, uint32_t size) {
			if(!initialized()) return -1;
			return ( setsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), value, size));
		}

		//-----------------------------------
		// basic_ip_socket::get_option_int
		//-----------------------------------
		int basic_ip_socket::get_option_int(const socket_option_level& opt, const socket_option_name& name) {
			if(!initialized()) return -1;

			int value = 0;
			socklen_t _size = sizeof(value);
			getsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, &_size);

			return value;
		}

		//-----------------------------------
		// basic_ip_socket::get_option_bool
		//-----------------------------------
		bool basic_ip_socket::get_option_bool(const socket_option_level& opt, const socket_option_name& name) {
			return get_option_int(opt, name) == 1;
		}

		//-----------------------------------
		// basic_ip_socket::get_option_raw
		//-----------------------------------
		int basic_ip_socket::get_option_raw(const socket_option_level& opt, const socket_option_name& name,
											void* value, uint32_t size) {
			if(m_iHandle == -1) { value = NULL; return -1; }
			return getsockopt(m_iHandle, static_cast<int>(opt), static_cast<int>(name), &value, &size);
		}

		//-----------------------------------
		// basic_ip_socket::operator =
		//-----------------------------------
		basic_ip_socket& basic_ip_socket::operator = (const basic_ip_socket& other) {
			m_iHandle = other.m_iHandle;
			m_eFam = other.m_eFam;
			m_eType = other.m_eType;
			m_eProtocol = other.m_eProtocol;

			return *this;
		}

		//-----------------------------------
		// basic_ip_socket::set_reuse_address
		//-----------------------------------
		void basic_ip_socket::set_reuse_address(bool flag) {
			set_options(socket_option_level::socket, socket_option_name::reuse_addr, flag);

		}
		//-----------------------------------
		// basic_ip_socket::set_rause_port
		//-----------------------------------
		void basic_ip_socket::set_rause_port(bool flag) {
		#ifdef SO_REUSEPORT
			set_options(socket_option_level::socket, socket_option_name::reuse_port, flag);
		#endif
		}
		//-----------------------------------
		// basic_ip_socket::set_linger
		//-----------------------------------
		void basic_ip_socket::set_linger(bool on, int seconds) {
			struct linger l;
			l.l_onoff  = on ? 1 : 0;
			l.l_linger = seconds;

			set_options(socket_option_level::socket, socket_option_name::linger, &l, sizeof(l));
		}
		//-----------------------------------
		// basic_ip_socket::set_no_delay
		//-----------------------------------
		void basic_ip_socket::set_no_delay(bool flag) {
			set_options(socket_option_level::tcp, socket_option_name::tcp_nodelay, flag);
		}
		//-----------------------------------
		// basic_ip_socket::set_keep_alive
		//-----------------------------------
		void basic_ip_socket::set_keep_alive(bool flag) {
			set_options(socket_option_level::socket, socket_option_name::keepalive, flag);
		}
		//-----------------------------------
		// basic_ip_socket::set_oob_inline
		//-----------------------------------
		void basic_ip_socket::set_oob_inline(bool flag) {
			set_options(socket_option_level::socket, socket_option_name::oob_inline, flag);
		}
		//-----------------------------------
		// basic_ip_socket::set_blocking
		//-----------------------------------
		void basic_ip_socket::set_blocking(bool flag) {
			int arg = flag ? 0 : 1;
			ioctl(ioctl_request_type::non_blocking, arg);

			m_bBlocked = flag;
		}
		//-----------------------------------
		// basic_ip_socket::set_nocheak
		//-----------------------------------
		void basic_ip_socket::set_nocheak(bool value) {
			set_options(socket_option_level::socket, socket_option_name::no_check, value);
		}

		//-----------------------------------
		// basic_ip_socket::set_send_buffer_size
		//-----------------------------------
		void basic_ip_socket::set_send_buffer_size(int value) {
			set_options(socket_option_level::socket, socket_option_name::sendbuffer, value);
        }

        //-----------------------------------
		// basic_ip_socket::set_recive_buffer_size
		//-----------------------------------
		void basic_ip_socket::set_recive_buffer_size(int value) {
			set_options(socket_option_level::socket, socket_option_name::recivebuffer, value);
        }

        //-----------------------------------
		// basic_ip_socket::set_send_timeout
		//-----------------------------------
        void basic_ip_socket::set_send_timeout(int value) {
			set_options(socket_option_level::socket, socket_option_name::send_timeout, value);
        }

        //-----------------------------------
		// basic_ip_socket::set_recive_timeout
		//-----------------------------------
        void basic_ip_socket::set_recive_timeout(int value) {
			set_options(socket_option_level::socket, socket_option_name::recive_timeout, value);
        }

		//-----------------------------------
		// basic_ip_socket::get_reuse_address
		//-----------------------------------
		bool basic_ip_socket::get_reuse_address() {
			return get_option_bool(socket_option_level::socket, socket_option_name::reuse_addr);

		}
		//-----------------------------------
		// basic_ip_socket::get_rause_port
		//-----------------------------------
		bool basic_ip_socket::get_rause_port() {
		#ifdef SO_REUSEPORT
			return get_option_bool(socket_option_level::socket, socket_option_name::reuse_port);
		#else
			return false;
		#endif
		}
		//-----------------------------------
		// basic_ip_socket::get_linger
		//-----------------------------------
		void basic_ip_socket::get_linger(bool& on, int& seconds) {
			struct linger l;
			if(get_option_raw(socket_option_level::socket, socket_option_name::linger, &l, sizeof(l)) > 0) {
				on = l.l_onoff != 0;
				seconds = l.l_linger;
			} else {
				on = false;
				seconds = -1;
			}
		}
		//-----------------------------------
		// basic_ip_socket::get_no_delay
		//-----------------------------------
		bool basic_ip_socket::get_no_delay() {
			return get_option_bool(socket_option_level::tcp, socket_option_name::tcp_nodelay);
		}
		//-----------------------------------
		// basic_ip_socket::get_keep_alive
		//-----------------------------------
		bool basic_ip_socket::get_keep_alive() {
			return get_option_bool(socket_option_level::socket, socket_option_name::keepalive);
		}
		//-----------------------------------
		// basic_ip_socket::get_oob_inline
		//-----------------------------------
		bool basic_ip_socket::get_oob_inline() {
			return get_option_bool(socket_option_level::socket, socket_option_name::oob_inline);
		}

		//-----------------------------------
		// basic_ip_socket::get_nocheak
		//-----------------------------------
		bool basic_ip_socket::get_nocheak() {
			return get_option_bool(socket_option_level::socket, socket_option_name::no_check);
		}
		//-----------------------------------
		// basic_ip_socket::get_send_buffer_size
		//-----------------------------------
		int basic_ip_socket::get_send_buffer_size() {
			return get_option_int(socket_option_level::socket, socket_option_name::sendbuffer);
        }

        //-----------------------------------
		// basic_ip_socket::get_recive_buffer_size
		//-----------------------------------
		int basic_ip_socket::get_recive_buffer_size() {
			return get_option_int(socket_option_level::socket, socket_option_name::recivebuffer);
        }

        //-----------------------------------
		// basic_ip_socket::get_send_timeout
		//-----------------------------------
        int basic_ip_socket::get_send_timeout() {
			return get_option_int(socket_option_level::socket, socket_option_name::send_timeout);
        }

        //-----------------------------------
		// basic_ip_socket::get_recive_timeout
		//-----------------------------------
        int basic_ip_socket::get_recive_timeout() {
			return get_option_int(socket_option_level::socket, socket_option_name::recive_timeout);
        }


		//-----------------------------------
		// basic_ip_socket::ioctl
		//-----------------------------------
		int basic_ip_socket::ioctl(const ioctl_request_type& request, void* arg) {
			if(!initialized()) return -1;

			return lwip_ioctl(m_iHandle, static_cast<int>(request), arg);
		}
		//-----------------------------------
		// basic_ip_socket::ioctl
		//-----------------------------------
		int basic_ip_socket::ioctl(const ioctl_request_type& request, int& arg) {
			if(!initialized()) return -1;

			return lwip_ioctl(m_iHandle, static_cast<int>(request), &arg);
		}
		//-----------------------------------
		// basic_ip_socket::fcntl
		//-----------------------------------
		int basic_ip_socket::set_fcntl(int arg) {
			if(!initialized()) return -1;

			return lwip_fcntl(m_iHandle, F_SETFL, arg);
		}
		//-----------------------------------
		// basic_ip_socket::fcntl
		//-----------------------------------
		int basic_ip_socket::get_fcntl()  {
			if(!initialized()) return -1;

			return ::fcntl(m_iHandle,F_GETFL);
		}
		//-----------------------------------
		// basic_ip_socket::shutdown
		//-----------------------------------
		void basic_ip_socket::shutdown(const socket_shutdown_type& cmd) {
			 lwip_shutdown(m_iHandle, static_cast<int>(cmd));
		}
		//-----------------------------------
		// basic_ip_socket::poll
		//-----------------------------------
		bool basic_ip_socket::poll(const unsigned long& timeout, int mode) {
			if(!initialized()) return false;
			handle_type _sock_hdnl = m_iHandle;
			unsigned long _remainingTime = timeout, _start = micros(), _end = 0, _waited;
			int _polled = 0;

			pollfd _pollBuf;
			memset(&_pollBuf, 0, sizeof(pollfd));
			_pollBuf.fd = _sock_hdnl;

			do {
				_polled = ::poll(&_pollBuf, 1, _remainingTime);
				if (_polled < 0) {
					_end = micros();
					_waited = (_end - _start);

					if( _waited < _remainingTime ) _remainingTime -= _waited;
					else _remainingTime = 0;
				}

	  		} while(_polled < 0);

	  		return _polled > 0;
		}

	}
}


