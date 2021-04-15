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

		/**
         * @brief Defines socket address types for creating
         */
		enum class address_family {
			Unknown             = -1,   	 /*!< Unknown */
			Unspecified         = AF_UNSPEC, /*!< The address family is unspecified. */
			inet_v4      		= AF_INET,   /*!< The Internet Protocol version 4 (IPv4) address family. */
			inet_v6      		= AF_INET6,  /*!< The Internet Protocol version 6 (IPv6) address family. */
		};

		/**
         * @brief Defines socket protoco types for creating
         */
		enum class protocol_type : int {
            unspec = 0,					/*!<  */
            icmp = IPPROTO_ICMP,		/*!< The Internet Control Message Protocol (ICMP) */
            tcp = IPPROTO_TCP,			/*!< The Transmission Control Protocol (TCP). */
            udp = IPPROTO_UDP,			/*!< The User Datagram Protocol (UDP) */
		#if LWIP_IPV6 == 1

            icmpv6 = IPPROTO_ICMPV6,	/*!< The Internet Control Message Protocol Version 6 (ICMPv6) */
		#endif // LWIP_IPV6
            udp_lite = IPPROTO_UDPLITE,	/*!< This is an implementation of the Lightweight User Datagram
       Protocol (UDP-Lite), as described in RFC 3828. */
            raw = IPPROTO_RAW			/*!< Raw sockets allow new IPv4 protocols to be implemented in user
       space.  */
        };

		 /**
         * @brief Defines socket socket types for creating
         */
        enum class socket_type : int {
            unspec = -1,				/*!< Unspec type of socket */
            stream = SOCK_STREAM,		/*!< Socket type for stream (tcp) */
            dgram = SOCK_DGRAM,			/*!< Socket type for datagram (udp for example) */
            raw = SOCK_RAW,				/*!< A socket type that provides a raw socket that allows an application to manipulate the next upper-layer protocol header */
            MAX
        };
        /**
         * @brief Specifies socket send and receive behaviors.
         */
        enum class socket_flags : int {
            none = 0x00,					/*!< Use no flags for this call. */
            out_of_band = MSG_OOB,			/*!< Processes Out Of Band (OOB) data. */
            peek = MSG_PEEK,				/*!< Peeks at the incoming data. The data is copied into the buffer, but is not removed from the input queue. */
            /**
             * The receive request will complete only when one of the following events occurs:
			 *		- The buffer supplied by the caller is completely full.
			 *		- The connection has been closed.
			 *		- The request has been canceled or an error occurred.
             */
            wait_all = MSG_WAITALL,
            non_blocking = MSG_DONTWAIT,	/*!< Nonblocking i/o for this operation only */
            more = MSG_MORE,				/*!< Sender will send more */
            no_signal = MSG_NOSIGNAL		/*!< Requests not to send any data on errors on stream oriented sockets when the other end breaks the connection */
        };
        enum class socket_select_mode : int {
            read = SHUT_RD,					/*!< receptions will be disallowed  */
            write = SHUT_WR,				/*!< transmissions will be disallowed */
            rw = SHUT_RDWR					/*!< receptions and transmissions will be disallowed */
        };
        /**
         * @brief Defines socket option levels for the mn_socket::set_options and mn_socket::get_options
         */
        enum class socket_option_level : int {
            socket = 0xffff,								/*!< options for socket level */
            ip = IPPROTO_IP,								/*!< options for IPv4 level */
        #if LWIP_IPV6 == 1
            ipv6 = (int)IPPROTO_IPV6,						/*!< options for IPv6 level */
        #endif // LWIP_IPV6
            tcp = (int)protocol_type::tcp,					/*!< options for TCP level */
            udp = (int)protocol_type::udp,					/*!< options for UDP level */
            udp_lite = (int)protocol_type::udp_lite,		/*!< options for UDPLite level */

        };
        enum class socket_option_name : int {
			debug = SO_DEBUG,						/*!< Enables debug output. */
            acceptconn = SO_ACCEPTCONN,				/*!< Returns whether a socket is in listening mode. */
            dont_route = SO_DONTROUTE,				/*!< Sets whether outgoing data should be sent on interface the socket is bound to and not a routed on some other interface */
            use_loopback = SO_USELOOPBACK,			/*!< Use the local loopback address when sending data from this socket.  */
            linger = SO_LINGER,						/*!< Lingers on close if unsent data is present. */
            nolinger = SO_DONTLINGER,				/*!< Does not block close waiting for unsent data to be sent. Setting this option is equivalent to setting SO_LINGER with 0*/
            oob_inline = SO_OOBINLINE,				/*!< Indicates that out-of-bound data should be returned in-line with regular data. This option is only valid for connection-oriented protocols that support out-of-band data. */
            reuse_port = SO_REUSEPORT,				/*!< Allows the socket to be bound to an port that is already in use.  */
            reuse_addr = SO_REUSEADDR, 				/*!< Allows the socket to be bound to an address that is already in use.  */
            sendbuffer = SO_SNDBUF,					/*!< Specifies the total per-socket buffer space reserved for sends. */
            recivebuffer = SO_RCVBUF, 				/*!< Specifies the total per-socket buffer space reserved for receives. */

        #if LWIP_UDP == 1 && LWIP_UDPLITE == 1
            udplite_send_cscov = UDPLITE_SEND_CSCOV, /*!< sender checksum coverage */
            udplite_recv_cscov = UDPLITE_RECV_CSCOV, /*!< minimal receiver checksum coverage */
        #endif // LWIP_UDP
            send_lowat = SO_SNDLOWAT, 				/*!< This option sets the minimum number of bytes to process for socket output operations. */
            recive_lowat = SO_RCVLOWAT, 			/*!< This option sets the minimum number of bytes to process for socket input operations. */
            send_timeout = SO_SNDTIMEO,				/*!< Sets the timeout, in milliseconds, for blocking send calls. */
            recive_timeout = SO_RCVTIMEO,			/*!< Sets the timeout, in milliseconds, for blocking receive calls */
            error = SO_ERROR, 						/*!< Get last error */
            get_type = SO_TYPE, 					/*!< Returns the socket type for the given socket (SOCK_STREAM or SOCK_DGRAM) */
            commected_timeout = SO_CONTIMEO, 		/*!< Unimplemented: connect timeout */
            no_check = SO_NO_CHECK, 				/*!< don't create UDP checksum */
            bind_device = SO_BINDTODEVICE,			/*!<  */
            broadcast = SO_BROADCAST, 				/*!< Configures a socket for sending broadcast data. */
            keepalive = SO_KEEPALIVE, 				/*!< Enables sending keep-alive packets for a socket connection.  */

            tos = IP_TOS,							/*!< Type of Service (TOS) settings should only be set using the Quality of Service API. */
            ttl = IP_TTL,							/*!< Changes the default value set by the TCP/IP service provider in the TTL field of the IP header in outgoing datagrams */

		#if LWIP_TCP == 1
			tcp_nodelay = TCP_NODELAY,				/*!< Disables the Nagle algorithm for send coalescing. */
			tcp_keepalive = TCP_KEEPALIVE,			/*!< send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
			tcp_keepidle = TCP_KEEPIDLE,			/*!< Gets or sets the number of seconds a TCP connection will remain idle before keepalive probes are sent to the remote. */
			tcp_keepintvl = TCP_KEEPINTVL,			/*!< Gets or sets the number of seconds a TCP connection will wait for a keepalive response before sending another keepalive probe. */
			tcp_keepcnt = TCP_KEEPCNT,				/*!< Gets or sets the number of TCP keep alive probes that will be sent before the connection is terminated. */
		#endif

		#ifdef LWIP_MULTICAST_TX_OPTIONS
			multicast_ttl = IP_MULTICAST_TTL,		/*!< Sets/gets the TTL value associated with IP multicast traffic on the socket. */
			multicast_if = IP_MULTICAST_IF,			/*!< Gets or sets the outgoing interface for sending IPv4 multicast traffic. */
			multicast_loop = IP_MULTICAST_LOOP,		/*!< Controls whether data sent by an application on the local computer (not necessarily by the same socket) in a multicast session will be received by a socket joined to the multicast destination group on the loopback interface */
		#endif

		#if ESP_LWIP == 1 && LWIP_IPV6_MLD == 1
			multicast_if6 = IPV6_MULTICAST_IF,		/*!< Gets or sets the outgoing interface for sending IPv6 multicast traffic. */
			multicast_hops6 = IPV6_MULTICAST_HOPS,	/*!< Gets or sets the TTL value associated with IPv6 multicast traffic on the socket */
			multicast_loop6 = IPV6_MULTICAST_LOOP,	/*!< Indicates multicast data sent on the socket will be echoed to the sockets receive buffer if it is also joined on the destination multicast group.  */
			multicast_add_membership6 = IPV6_ADD_MEMBERSHIP, /*!< IP6 Join the socket to the supplied multicast group on the specified interface. ip6 */
			multicast_drop_membership6 = IPV6_DROP_MEMBERSHIP, /*!< IP6 Leaves the specified multicast group from the specified interface. Service providers must support this option when multicast is supported. */
		#endif
		#if LWIP_IGMP == 1
			igmp_add_membership = IP_ADD_MEMBERSHIP, 	/*!< Join the socket to the supplied multicast group on the specified interface. */
            igmp_drop_membership = IP_DROP_MEMBERSHIP, /*!< Leaves the specified multicast group from the specified interface. Service providers must support this option when multicast is supported.  */
		#endif // LWIP_IGMP
        };

        enum service_provides {
			tos_mask = IPTOS_TOS_MASK,			/*!<  */
			low_delay = IPTOS_LOWDELAY,			/*!<  */
			through_put = IPTOS_THROUGHPUT,		/*!<  */
			reliability = IPTOS_RELIABILITY,	/*!<  */
			low_cost = IPTOS_LOWCOST,			/*!<  */
			min_cost = IPTOS_MINCOST,			/*!<  */
        };

	}
}

#endif

