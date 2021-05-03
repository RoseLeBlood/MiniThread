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
#ifndef __MINILIB_BASIC_MULTICAST_IP_SOCKET_H__
#define __MINILIB_BASIC_MULTICAST_IP_SOCKET_H__

#include "../mn_config.hpp"
#include "mn_basic_dgram_socket.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief A multicast_ip_socket is a special dgram_ip_socket
		 * that can be used to send packets to and receive packets from multicast groups.
		 */
		class basic_multicast_ip_socket : public basic_dgram_ip_socket {
		public:
			using self_typ = basic_multicast_ip_socket;
			using base_type = basic_dgram_ip_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			/**
			 * @brief Construt an unconnected, unbound multicast socket.
			 */
			basic_multicast_ip_socket()
				: base_type() { }

			/**
			 * @brief Joins the specified multicast group
			 */
			int join_group(const ipaddress_type& groupAddress,
						   const ipaddress_type& infAddress = MNNET_IPV4_ADDRESS_ANY);

			/**
			 * @brief Leaves the specified multicast group
			 */
			int leave_group();

			/**
			 * @brief Leaves the specified multicast group
			 */
			int leave_group(const ipaddress_type& groupAddress,
							const ipaddress_type& infAddress = MNNET_IPV4_ADDRESS_ANY);

			/**
        	 * @brief Set the option socket_option_name::loopback
        	 * @param flag if true then enable the option and false when not
        	 */
			void 		set_loopback(bool flag);

			/**
        	 * @brief Set the option socket_option_name::ttl
        	 * @param value The value for the option in millis
        	 */
			void 		set_time_to_live(int value);

			/**
        	 * @brief Set the option IP_MULTICAST_IF
        	 * @param value The value for the option in millis
        	 */
			void 		set_interface(const ipaddress_type& infAddress);

			/**
			 * @brief Get the value from option socket_option_name::multicast_loop
			 * @return If true then enabled the option multicast_loop and if false when not
			 */
			bool 		get_loopback();

			/**
			 * @brief Get the option IP_MULTICAST_TTL.
			 * @return The value of the option  IP_MULTICAST_TTL.
			 */
			int 		get_time_to_live();

			/**
			 * @brief Get the option IP_MULTICAST_IF
			 * @return The interface address.
			 */
			ipaddress_type	get_interface();

			virtual void swap(self_typ& rhs) noexcept {
        		base_type::swap(rhs);
        		m_ipMulticast.swap(rhs.m_ipMulticast);
			}
		protected:
			basic_multicast_ip_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_multicast_ip_socket(const protocol_type& protocol)
				: base_type(protocol) { }
		private:
			ipaddress_type m_ipMulticast;
		};

	#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		/**
		 * @brief A basic_multicast_ip6_socket is a special dgram_ip6_socket
		 * that can be used to send packets to and receive packets from multicast groups.
		 */
		class basic_multicast_ip6_socket : public basic_dgram_ip6_socket {
		public:
			using self_typ = basic_multicast_ip6_socket;
			using base_type = basic_dgram_ip6_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			/**
			 * @brief Construt an unconnected, unbound multicast socket.
			 */
			basic_multicast_ip6_socket()
				: base_type() { }

			/**
			 * @brief Joins the specified multicast group
			 */
			int join_group(const ipaddress_type& groupAddress, const uint8_t& uiInterface);

			/**
			 * @brief Leaves the specified multicast group
			 */
			int leave_group();

			/**
			 * @brief Leaves the specified multicast group
			 */
			int leave_group(const ipaddress_type& groupAddress, const uint8_t& uiInterface);

			/**
        	 * @brief Set the option socket_option_name::loopback
        	 * @param flag if true then enable the option and false when not
        	 */
			void 		set_loopback(bool flag);

			/**
        	 * @brief Set the option socket_option_name::ttl
        	 * @param value The value for the option in millis
        	 */
			void 		set_time_to_live(int value);

			/**
        	 * @brief Set the option IP_MULTICAST_IF
        	 * @param value The value for the option in millis
        	 */
			void 		set_interface(const uint8_t& infAddress);

			/**
			 * @brief Get the value from option socket_option_name::multicast_loop
			 * @return If true then enabled the option multicast_loop and if false when not
			 */
			bool 		get_loopback();

			/**
			 * @brief Get the option IP_MULTICAST_TTL.
			 * @return The value of the option  IP_MULTICAST_TTL.
			 */
			int 		get_time_to_live();

			/**
			 * @brief Get the option IP_MULTICAST_IF
			 * @return The interface address.
			 */
			uint8_t	get_interface();

			virtual void swap(self_typ& rhs) noexcept {
        		base_type::swap(rhs);
        		m_ipMulticast.swap(rhs.m_ipMulticast);
			}
		protected:
			basic_multicast_ip6_socket(handle_type& hndl, endpoint_type* endp = nullptr)
				: base_type(hndl, endp) { }
			basic_multicast_ip6_socket(const protocol_type& protocol)
				: base_type(protocol) { }
		private:
			ipaddress_type m_ipMulticast;
		};
	#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE




	#if LWIP_UDP && LWIP_UDPLITE
		/**
		 * @brief Basic dram lite multicast socket class
		 * @ingroup socket
		 */
		class basic_multicast_lite_ip_socket : public basic_multicast_ip_socket {
		public:
			using self_typ = basic_multicast_lite_ip_socket;
			using base_type = basic_multicast_ip_socket;

			using endpoint_type = typename base_type::endpoint_type;
			using ipaddress_type = typename base_type::ipaddress_type;
			using handle_type = typename base_type::handle_type;

			basic_dgramlite_ip_socket() : base_type(protocol_type::udp_lite) { }

			virtual void swap(self_typ& rhs) noexcept {
        		base_type::swap(rhs);
			}
		};
	#endif
	}
}

#endif // __MINILIB_BASIC_MULTICAST_IP_SOCKET_H__
