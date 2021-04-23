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
         * @ingroup socket
         */
		enum class address_family {
			/**
			 * @brief The address family is unspecified.
			 */
			Unspecified         = AF_UNSPEC,
			/**
			 * @brief The Internet Protocol version 4 (IPv4) address family.
			 */
			inet_v4      		= AF_INET,
			/**
			 * @brief The Internet Protocol version 6 (IPv6) address family.
			 */
			inet_v6      		= AF_INET6,
		};

		/**
         * @brief Defines socket protoco types for creating
         * @ingroup socket
         */
		enum class protocol_type : int {
			/**
			 * @brief
			 */
            unspec = IPPROTO_IP,
            /**
			 * @briefThe Internet Control Message Protocol (ICMP)
			 */
            icmp = IPPROTO_ICMP,
            /**
			 * @brief The Transmission Control Protocol (TCP).
			 */
            tcp = IPPROTO_TCP,
            /**
			 * @brief The User Datagram Protocol (UDP)
			 */
            udp = IPPROTO_UDP,
		#if LWIP_IPV6 == 1
			/**
			 * @brief The Internet Control Message Protocol Version 6 (ICMPv6)
			 */
            icmpv6 = IPPROTO_ICMPV6,
		#endif // LWIP_IPV6

			/**
			 * @brief This is an implementation of the Lightweight User Datagram Protocol (UDP-Lite), as described
			 * in RFC 3828.
			 */
            udp_lite = IPPROTO_UDPLITE,
       		/**
			 * @brief Raw sockets allow new IPv4 protocols to be implemented in user space.
			 */
            raw = IPPROTO_RAW
        };

		 /**
         * @brief Defines socket socket types for creating
         * @ingroup socket
         */
        enum class socket_type : int {
            /**
             * @brief Socket type for stream (tcp)
             */
            stream = SOCK_STREAM,
            /**
             * @brief Socket type for datagram (udp for example)
             */
            dgram = SOCK_DGRAM,
            /**
             * @brief A socket type that provides a raw socket that allows an application to manipulate the next
             * upper-layer protocol header
             */
            raw = SOCK_RAW,
        };
        /**
         * @brief Specifies socket send and receive behaviors.
         * @ingroup socket
         */
        enum class socket_flags : int {
        	/**
        	 * @brief Use no flags for this call.
        	 */
            none = 0x00,
            /**
             * @brief Processes Out Of Band (OOB) data.
             */
            out_of_band = MSG_OOB,
            /**
             * @brief Peeks at the incoming data. The data is copied into the buffer, but is not removed from the input
             * queue.
             */
            peek = MSG_PEEK,
            /**
             * @ brief The receive request will complete only when one of the following events occurs:
			 *			- The buffer supplied by the caller is completely full.
			 *			- The connection has been closed.
			 *			- The request has been canceled or an error occurred.
             */
            wait_all = MSG_WAITALL,
            /**
             * @brief Nonblocking i/o for this operation only
             */
            non_blocking = MSG_DONTWAIT,
            /**
             * @brief Sender will send more
             */
            more = MSG_MORE,
            /**
             * @brief Requests not to send any data on errors on stream oriented sockets when the other end breaks the
             * connection
             */
            no_signal = MSG_NOSIGNAL,
        };
        /**
         * @brief Commands for lwip_..
         * @ingroup socket
         */
        enum class socket_select_mode : int {
        	/**
        	 * @brief receptions will be disallowed
        	 */
            read = SHUT_RD,
            /**
             * @brief transmissions will be disallowed
             */
            write = SHUT_WR,
            /**
             * @brief receptions and transmissions will be disallowed
             */
            rw = SHUT_RDWR
        };
        /**
         * @brief Items for socket option levels for the mn_socket::set_options and mn_socket::get_options
         * @ingroup socket
         */
        enum class socket_option_level : int {
        	/**
        	 * @brief options for socket level
        	 */
            socket = SOL_SOCKET,
            /**
             * @brief options for IPv4 level
             */
            ip = IPPROTO_IP,
        #if LWIP_IPV6 == 1
        	/**
        	 * @brief options for IPv6 level
        	 */
            ipv6 = (int)IPPROTO_IPV6,
        #endif // LWIP_IPV6

        	/**
        	 * @brief options for TCP level
        	 */
            tcp = (int)protocol_type::tcp,
            /**
             * @brief options for UDP level
             */
            udp = (int)protocol_type::udp,
            /**
             * @brief options for UDPLite level
             */
            udp_lite = (int)protocol_type::udp_lite,

        };
        /**
         * @brief Items for socket option name for the mn_socket::set_options and mn_socket::get_options
         * @ingroup socket
         */
        enum class socket_option_name : int {
        	/**
        	 * @brief Enables debug output.
        	 */
			debug = SO_DEBUG,
			/**
			 * @brief Returns whether a socket is in listening mode.
			 */
            acceptconn = SO_ACCEPTCONN,
            /**
             * @brief Sets whether outgoing data should be sent on interface the socket is bound to and not a
             * routed on some other interface
             */
            dont_route = SO_DONTROUTE,
            /**
             * @brief Use the local loopback address when sending data from this socket.
             */
            use_loopback = SO_USELOOPBACK,
            /**
             * @brief Lingers on close if unsent data is present.
             */
            linger = SO_LINGER,
            /**
             * @brief Does not block close waiting for unsent data to be sent. Setting this option is equivalent
             * to setting SO_LINGER with 0
             */
            nolinger = SO_DONTLINGER,
            /**
             * @brief Indicates that out-of-bound data should be returned in-line with regular data. This option is
             * only valid for connection-oriented protocols that support out-of-band data.
             */
            oob_inline = SO_OOBINLINE,
            /**
             * @brief Allows the socket to be bound to an port that is already in use.
             */
            reuse_port = SO_REUSEPORT,
            /**
             * @brief Allows the socket to be bound to an address that is already in use.
             */
            reuse_addr = SO_REUSEADDR,
            /**
             * @brief Specifies the total per-socket buffer space reserved for sends.
             */
            sendbuffer = SO_SNDBUF,
            /**
             * @brief Specifies the total per-socket buffer space reserved for receives.
             */
            recivebuffer = SO_RCVBUF,

        #if LWIP_UDP == 1 && LWIP_UDPLITE == 1
        	/**
        	 * @brief sender checksum coverage
        	 */
            udplite_send_cscov = UDPLITE_SEND_CSCOV,
            /**
             * @brief minimal receiver checksum coverage
             */
            udplite_recv_cscov = UDPLITE_RECV_CSCOV,
        #endif // LWIP_UDP

        	/**
        	 * @brief This option sets the minimum number of bytes to process for socket output operations.
        	 */
            send_lowat = SO_SNDLOWAT,
            /**
             * @brief This option sets the minimum number of bytes to process for socket input operations.
			 */
            recive_lowat = SO_RCVLOWAT,
            /**
             * @brief Sets the timeout, in milliseconds, for blocking send calls.
             */
            send_timeout = SO_SNDTIMEO,
            /**
             * @brief Sets the timeout, in milliseconds, for blocking receive calls
             */
            recive_timeout = SO_RCVTIMEO,
            /**
             * @brief Get last error
             */
            error = SO_ERROR,
            /**
             * @brief Returns the socket type for the given socket (socket_type::stream or socket_type::dgram)
             */
            get_type = SO_TYPE,
            /**
             * @brief Unimplemented: connect timeout
             */
            commected_timeout = SO_CONTIMEO,
            /**
             * @brief don't create UDP checksum
             */
            no_check = SO_NO_CHECK,
            /**
             * @brief TODO: e
             */
            bind_device = SO_BINDTODEVICE,
            /**
             * @brief Configures a socket for sending broadcast data.
             */
            broadcast = SO_BROADCAST,
            /**
             * @brief Enables sending keep-alive packets for a socket connection.
             */
            keepalive = SO_KEEPALIVE,


			/*
			 * @brief Type of Service (TOS) settings should only be set using the Quality of Service API.
			 */
            tos = IP_TOS,
            /*
             * @brief Changes the default value set by the TCP/IP service provider in the TTL field of the IP
             * header in outgoing datagrams
             */
            ttl = IP_TTL,


		#if LWIP_TCP == 1
			/**
			 * @brief Disables the Nagle algorithm for send coalescing.
			 * @note  Options for level IPPROTO_TCP
			 */
			tcp_nodelay = TCP_NODELAY,
			/**
			 * @brief send KEEPALIVE probes when idle for pcb->keep_idle milliseconds
			 * @note  Options for level IPPROTO_TCP
			 */
			tcp_keepalive = TCP_KEEPALIVE,
			/**
			 * @brief Gets or sets the number of seconds a TCP connection will remain idle before keepalive probes
			 *  are sent to the remote.
			 * @note  Options for level IPPROTO_TCP
			 */
			tcp_keepidle = TCP_KEEPIDLE,
			/**
			 * @brief Gets or sets the number of seconds a TCP connection will wait for a keepalive response before
			 * sending another keepalive probe.
			 * @note  Options for level IPPROTO_TCP
			 */
			tcp_keepintvl = TCP_KEEPINTVL,
			/**
			 * @brief Gets or sets the number of TCP keep alive probes that will be sent before the connection is
			 * terminated.
			 * @note  Options for level IPPROTO_TCP
			 */
			tcp_keepcnt = TCP_KEEPCNT,
		#endif

		#ifdef LWIP_MULTICAST_TX_OPTIONS
			/**
			 * @brief Sets/gets the TTL value associated with IP multicast traffic on the socket.
			 */
			multicast_ip4_ttl = IP_MULTICAST_TTL,
			/**
			 * @brief Gets or sets the outgoing interface for sending IPv4 multicast traffic.
			 */
			multicast_ip4_if = IP_MULTICAST_IF,
			/**
			 * @brief Controls whether data sent by an application on the local computer (not necessarily by the same
			 * socket) in a multicast session will be received by a socket joined to the multicast destination group on
			 * the loopback interface
			 */
			multicast_ip4_loop = IP_MULTICAST_LOOP,
		#endif

		#if ESP_LWIP == 1 && LWIP_IPV6_MLD == 1
			/**
			 * @brief Gets or sets the outgoing interface for sending IPv6 multicast traffic.
			 */
			multicast_ip6_if = IPV6_MULTICAST_IF,
			/**
			 * @brief Gets or sets the TTL value associated with IPv6 multicast traffic on the socket
			 */
			multicast_ip6_hops = IPV6_MULTICAST_HOPS,
			/**
			 * @brief Indicates multicast data sent on the socket will be echoed to the sockets receive buffer if it is
			 * also joined on the destination multicast group.
			 */
			multicast_ip6_loop = IPV6_MULTICAST_LOOP,
			/**
			 * @brief IP6 Join the socket to the supplied multicast group on the specified interface. ip6
			 */
			multicast_ip6_add_membership = IPV6_ADD_MEMBERSHIP,
			/**
			 * @brief IP6 Leaves the specified multicast group from the specified interface. Service providers must
			 * support this option when multicast is supported.
			 */
			multicast_ip6_drop_membership = IPV6_DROP_MEMBERSHIP,
		#endif

		#if LWIP_IGMP == 1
			/**
			 * @brief Join the socket to the supplied multicast group on the specified interface.
			 */
			igmp_add_membership = IP_ADD_MEMBERSHIP,
			/**
			 * @brief Leaves the specified multicast group from the specified interface. Service providers must support
			 * this option when multicast is supported.
			 */
            igmp_drop_membership = IP_DROP_MEMBERSHIP,
		#endif // LWIP_IGMP
        };

        /**
         * @ingroup socket
         */
        enum service_provides {
			tos_mask = IPTOS_TOS_MASK,			/*!<  */
			low_delay = IPTOS_LOWDELAY,			/*!<  */
			through_put = IPTOS_THROUGHPUT,		/*!<  */
			reliability = IPTOS_RELIABILITY,	/*!<  */
			low_cost = IPTOS_LOWCOST,			/*!<  */
			min_cost = IPTOS_MINCOST,			/*!<  */
        };
		/**
		 * @brief Commands for ioctlsocket(),  taken from the BSD file fcntl.h.
 		 * lwip_ioctl only supports FIONREAD and FIONBIO, for now
 		 *
         * @ingroup socket
         */
		enum class ioctl_request_type  {
			available = FIONREAD,				/*!< get # bytes to read */
			non_blocking = FIONBIO,				/*!< set/clear non-blocking i/o */
		};


	}
}

#endif

