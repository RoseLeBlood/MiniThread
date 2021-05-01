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

#include <lwip/sockets.h>
#include <lwip/igmp.h>
#include <lwip/icmp6.h>

#include "net/mn_basic_multicast_ip_socket.hpp"
#include "mn_error.hpp"



namespace mn {
namespace net {
	//-----------------------------------
	// join_group
	//-----------------------------------
	int basic_multicast_ip_socket::join_group(const ipaddress_type& groupAddress, const ipaddress_type& infAddress) {
		int __erRet = NO_ERROR;

		struct ip_mreq mr;

		mr.imr_multiaddr.s_addr = (uint32_t)groupAddress;
		mr.imr_interface.s_addr = (uint32_t)infAddress;

		if (setsockopt(m_iHandle, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
			 ESP_LOGE("dgram socket", "could not join igmp: %d", errno);
			 __erRet = ERR_MNTHREAD_UNKN;
		} else {
			m_ipMulticast = (groupAddress);
		}
		return __erRet;
	}

	//-----------------------------------
	// leave_group
	//-----------------------------------
	int basic_multicast_ip_socket::leave_group() {
		return leave_group(m_ipMulticast, get_interface() );
	}

	//-----------------------------------
	// leave_group
	//-----------------------------------
	int basic_multicast_ip_socket::leave_group(const ipaddress_type& groupAddress,
											   const ipaddress_type& infAddress) {
		int __erRet = NO_ERROR;
		struct ip_mreq mr;

		mr.imr_multiaddr.s_addr = (uint32_t)groupAddress;
		mr.imr_interface.s_addr = (uint32_t)infAddress;

		if (setsockopt(m_iHandle, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
			 ESP_LOGE("dgram socket", "could not drop igmp: %d", errno);
			 __erRet = ERR_MNTHREAD_UNKN;
		}
		return __erRet;
	}

	//-----------------------------------
	// set_loopback
	//-----------------------------------
	void basic_multicast_ip_socket::set_loopback(bool flag)  {
		set_options(socket_option_level::ip, socket_option_name::multicast_loop, flag);
	}

	//-----------------------------------
	// set_time_to_live
	//-----------------------------------
	void basic_multicast_ip_socket::set_time_to_live(int value) {
		set_options(socket_option_level::ip, socket_option_name::multicast_ttl, value);
	}

	//-----------------------------------
	// set_interface
	//-----------------------------------
	void basic_multicast_ip_socket::set_interface(const ipaddress_type& infAddress) {
		set_options(socket_option_level::ip, socket_option_name::multicast_if, infAddress);
	}

	//-----------------------------------
	// get_loopback
	//-----------------------------------
	bool basic_multicast_ip_socket::get_loopback() {
		return get_option_bool(socket_option_level::ip, socket_option_name::multicast_loop);
	}

	//-----------------------------------
	// get_time_to_live
	//-----------------------------------
	int basic_multicast_ip_socket::get_time_to_live() {
		return get_option_int(socket_option_level::ip, socket_option_name::multicast_ttl);
	}

	//-----------------------------------
	// get_interface
	//-----------------------------------
	typename basic_multicast_ip_socket::ipaddress_type	basic_multicast_ip_socket::get_interface() {
		int _ipRaw = get_option_int(socket_option_level::ip, socket_option_name::multicast_if);
		return ipaddress_type(_ipRaw);
	}

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

	//-----------------------------------
	// join_group
	//-----------------------------------
	int basic_multicast_ip6_socket::join_group(const ipaddress_type& groupAddress, const uint8_t& uiInterface) {
		int __erRet = NO_ERROR;
		struct ipv6_mreq mr;

		mr.ipv6mr_multiaddr = { groupAddress.get_int(0), groupAddress.get_int(1),
								  groupAddress.get_int(2), groupAddress.get_int(3)};
		mr.ipv6mr_interface = uiInterface;


		if (setsockopt(m_iHandle, IPPROTO_IP, IPV6_ADD_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
			 ESP_LOGE("dgram socket ipv6", "could not drop igmp: %d", errno);
			 __erRet = ERR_MNTHREAD_UNKN;
		}
		return __erRet;
	}

	//-----------------------------------
	// leave_group
	//-----------------------------------
	int basic_multicast_ip6_socket::leave_group() {
		return leave_group(m_ipMulticast, get_interface());
	}

	//-----------------------------------
	// leave_group
	//-----------------------------------
	int basic_multicast_ip6_socket::leave_group(const ipaddress_type& groupAddress, const uint8_t& uiInterface) {
		int __erRet = NO_ERROR;
		struct ipv6_mreq mr;

		mr.ipv6mr_multiaddr = { groupAddress.get_int(0), groupAddress.get_int(1),
								  groupAddress.get_int(2), groupAddress.get_int(3)};
		mr.ipv6mr_interface = uiInterface;


		if (setsockopt(m_iHandle, IPPROTO_IP, IPV6_DROP_MEMBERSHIP, &mr, sizeof(mr)) < 0) {
			 ESP_LOGE("dgram socket ipv6", "could not drop igmp: %d", errno);
			 __erRet = ERR_MNTHREAD_UNKN;
		}
		return __erRet;
	}

	//-----------------------------------
	// set_loopback
	//-----------------------------------
	void basic_multicast_ip6_socket::set_loopback(bool flag)  {
		unsigned uflag = flag ? 1 : 0;
		setsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_LOOP, &uflag, sizeof(uflag));
	}

	//-----------------------------------
	// set_time_to_live
	//-----------------------------------
	void basic_multicast_ip6_socket::set_time_to_live(int value) {
		setsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_HOPS, &value, sizeof(value));
	}

	//-----------------------------------
	// set_interface
	//-----------------------------------
	void basic_multicast_ip6_socket::set_interface(const uint8_t& infAddress) {
		setsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_IF, &infAddress, sizeof(infAddress));
	}

	//-----------------------------------
	// get_loopback
	//-----------------------------------
	bool basic_multicast_ip6_socket::get_loopback() {
		uint32_t _value = 0;
		socklen_t _size = sizeof(_value);

		getsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_LOOP, &_value, &_size);

		return _value == 1;
	}

	//-----------------------------------
	// get_time_to_live
	//-----------------------------------
	int basic_multicast_ip6_socket::get_time_to_live() {
		uint32_t _value = 0;
		socklen_t _size = sizeof(_value);

		getsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_HOPS, &_value, &_size);
		return _value;

	}

	//-----------------------------------
	// get_interface
	//-----------------------------------
	uint8_t	basic_multicast_ip6_socket::get_interface() {
		uint8_t _value = 0;
		socklen_t _size = sizeof(_value);

		getsockopt(m_iHandle, IPPROTO_IP, IPV6_MULTICAST_IF, &_value, &_size);
		return _value;
	}


	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
} }
