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
#ifndef _MINLIB_BASIC_NET_TYPES_HPP_
#define _MINLIB_BASIC_NET_TYPES_HPP_

#include "lwip/err.h"
#include "lwip/sockets.h"

#ifndef UDPLITE_SEND_CSCOV
#define UDPLITE_SEND_CSCOV 0x01
#endif // UDPLITE_SEND_CSCOV

#ifndef UDPLITE_RECV_CSCOV
#define UDPLITE_RECV_CSCOV 0x02
#endif // UDPLITE_RECV_CSCOV

#define SERVICE_PROVIDES_TOS(tos) (mn::net::service_provides) ((tos) & mn::net::service_provides::tos_mask)

namespace mn {
	namespace net {

		enum class address_family {
			Unknown             = -1,   // Unknown
			Unspecified         = AF_UNSPEC,    // unspecified
			inet_v4      		= AF_INET,    // internetwork: UDP, TCP, etc.
			inet_v6      		= AF_INET6,   // Internetwork Version 6
			Max = 5,   // Max
		};

		enum class protocol_type : int {
            unspec = 0,
            ip = IPPROTO_IP,
            icmp = IPPROTO_ICMP,
            tcp = IPPROTO_TCP,
            udp = IPPROTO_UDP,
		#if LWIP_IPV6 == 1
            ipv6 = IPPROTO_IPV6,
            icmpv6 = IPPROTO_ICMPV6,
		#endif // LWIP_IPV6
            udp_lite = IPPROTO_UDPLITE,
            raw = IPPROTO_RAW
        };

        enum class socket_type : int {
            unspec = -1,
            stream = SOCK_STREAM,
            dgram = SOCK_DGRAM,
            raw = SOCK_RAW,
            MAX
        };
        enum class socket_flags : int {
            none = 0x00,
            out_of_band = MSG_OOB,
            peek = MSG_PEEK,
            wait_all = MSG_WAITALL,
            non_blocking = MSG_DONTWAIT,
            more = MSG_MORE,
            no_signal = MSG_NOSIGNAL
        };
        enum class socket_select_mode : int {
            read = SHUT_RD,
            write = SHUT_WR,
            rw = SHUT_RDWR
        };
        enum class socket_option_level : int {
            socket = 0xffff,
            ip = (int)protocol_type::ip,
        #if LWIP_IPV6 == 1
            ipv6 = (int)protocol_type::ipv6,
        #endif // LWIP_IPV6
            tcp = (int)protocol_type::tcp,
            udp = (int)protocol_type::udp,
            udp_lite = (int)protocol_type::udp_lite,
            tos = IP_TOS,
            ttl = IP_TTL,

		#if LWIP_TCP == 1
			nodelay = TCP_NODELAY,
			keepalive = TCP_KEEPALIVE,
			keep_idle = TCP_KEEPIDLE,
			keep_intvl = TCP_KEEPINTVL,
			keep_cnt = TCP_KEEPCNT,
		#endif

		#ifdef LWIP_MULTICAST_TX_OPTIONS
			multicast_ttl = IP_MULTICAST_TTL,
			multicast_if = IP_MULTICAST_IF,
			multicast_loop = IP_MULTICAST_LOOP,
		#endif

		#if ESP_LWIP == 1 && LWIP_IPV6_MLD == 1
			multicast_if6 = IPV6_MULTICAST_IF,
			multicast_hops6 = IPV6_MULTICAST_HOPS,
			multicast_loop6 = IPV6_MULTICAST_LOOP,
			add_membership6 = IPV6_ADD_MEMBERSHIP, //IPV6_JOIN_GROUP
			drop_membership6 = IPV6_DROP_MEMBERSHIP, //IPV6_LEAVE_GROUP
		#endif
		#if LWIP_IGMP == 1
			add_membership = IP_ADD_MEMBERSHIP,
            drop_membership = IP_DROP_MEMBERSHIP,
		#endif // LWIP_IGMP
        };
        enum class socket_option_name : int {
			debug = SO_DEBUG,
            acceptconn = SO_ACCEPTCONN,
            dont_route = SO_DONTROUTE,
            use_loopback = SO_USELOOPBACK,
            linger = SO_LINGER,
            nolinger = SO_DONTLINGER,
            oob_inline = SO_OOBINLINE,
            reuse_port = SO_REUSEPORT,
            reuse_addr = SO_REUSEADDR,
            sendbuffer = SO_SNDBUF,
            recivebuffer = SO_RCVBUF,
        #if LWIP_UDP == 1 && LWIP_UDPLITE == 1
            udplite_send_cscov = UDPLITE_SEND_CSCOV,
            udplite_recv_cscov = UDPLITE_RECV_CSCOV,
        #endif // LWIP_UDP
            send_lowat = SO_SNDLOWAT,
            recive_lowat = SO_RCVLOWAT,
            send_timeout = SO_SNDTIMEO,
            recive_timeout = SO_RCVTIMEO,
            error = SO_ERROR,
            get_type = SO_TYPE,
            commected_timeout = SO_CONTIMEO,
            no_check = SO_NO_CHECK,
            bind_device = SO_BINDTODEVICE,
            broadcast = SO_BROADCAST,
            keepalive = SO_KEEPALIVE,
        };

        enum service_provides {
			tos_mask = IPTOS_TOS_MASK,
			low_delay = IPTOS_LOWDELAY,
			through_put = IPTOS_THROUGHPUT,
			reliability = IPTOS_RELIABILITY,
			low_cost = IPTOS_LOWCOST,
			min_cost = IPTOS_MINCOST,
        };

	}
}

#endif

