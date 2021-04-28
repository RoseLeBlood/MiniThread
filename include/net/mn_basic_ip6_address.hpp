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
#ifndef __MINLIBNET_BASIC_IP6_ADDRESS_H__
#define __MINLIBNET_BASIC_IP6_ADDRESS_H__

#include "../mn_config.hpp"

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

#define MNNET_IPV6_ADDRESS_BYTES        16
#define MNNET_NUMBER_OF_LABELS          8

#define MNNET_IPV6_NEW_ARRAY_NULL		new uint8_t[MNNET_IPV6_ADDRESS_BYTES]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
#define MNNET_IPV6_NEW_ARRAY_LOOP		new uint8_t[MNNET_IPV6_ADDRESS_BYTES]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }

#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
#define MNNET_IPV6_ADDRESS_ANY          mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_NULL, 0 )
#define MNNET_IPV6_ADDRESS_LOOPBACK     mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_LOOP, 0 )
#else
#define MNNET_IPV6_ADDRESS_ANY          mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_NULL )
#define MNNET_IPV6_ADDRESS_LOOPBACK     mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_LOOP )
#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID

#define MNNET_IPV6_ADDRESS_NONE        		 MNNET_IPV6_ADDRESS_ANY

#include "mn_basic_ip4_address.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief This class represents an internet (IP) version 6 host address.
		 * @ingroup socket
		 */
		class basic_ip6_address : public basic_ip_address {
		protected:
			/**
			 * The type for a basic_ip6_address, as union - for easer internal use
			 */
			union {
				uint8_t         m_Numbers[MNNET_IPV6_ADDRESS_BYTES];
				uint16_t        as_short[8];
				uint32_t        as_int[4];
			};
		#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			/**
			 * The scopid
			 */
			long            	m_ScopeId;
		#endif
		public:
			/**
			 * Construtor
			 * @brief construct a ip6 address (any)
			 */
			basic_ip6_address();

			/**
			 * Construtor
			 * @brief construct a ip6 address from uint32_t's
			 */
			basic_ip6_address(uint32_t ax, uint32_t bx, uint32_t cx, uint32_t dx);

			/**
			 * Construtor
			 * @brief construct a ip6 address from uint32_t's
			 */
			basic_ip6_address(uint32_t iarray[4]);
			/**
			 * Construtor
			 * @brief construct a ip6 address from array
			 */
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			basic_ip6_address(uint8_t address[16], int scopid);
#else
			basic_ip6_address(uint8_t address[16]);
#endif
			/**
			 * Construtor
			 * @brief construct a ip6 address from string
			 * @param newAddress The ip6 address as string
			 */
			basic_ip6_address(const char* str_ip);
			/**
			 * Construtor
			 * @brief construct a ip6 address from a other ip6 address
			 * @param ipOther The other object
			 */
			basic_ip6_address(const basic_ip6_address& ipOther);

		#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			/**
			 * Get the scope id
			 * @return The scope id
			 */
			long                    get_scopeid()
				{ return m_ScopeId; }
			/**
			 * set the scope id
			 * @param scope_id The new scope id
			 */
			void                    set_scopeid(long scope_id)
				{ m_ScopeId = id; }
		#endif
			/**
			 * Get this ip6 address as readeble string
			 * @return the ip6 address as readeble string
			 */
			const char*     to_string();
			/**
			 * Get this ip6 address as char array
			 * @return the ip6 address as array
			 */
			uint8_t*        get_bytes();
			/**
			 * Get this ip6 address as int array
			 * @return the ip6 address as array
			 */
			uint32_t*       get_array() { return as_int; }
			/**
			 * @brief Returns the length in bytes of the ip address
			 * @return The length in bytes of the ip address
			 */
			int length() const;
			/**
			 * @brief Get the ip section on the given position as int
			 * @param index The index of the position (0-3)
			 * @return The ip section on the given position as int
			 */
			virtual uint32_t		get_int(int index)  { return index < 4 ? as_int[index] : 0; }

			/**
			 * @brief set the ip to zero
			 */
			inline void				set_zero()
				{ as_int[0] = 0; as_int[1] = 0; as_int[2] = 0; as_int[3] = 0; }
			/**
			 * @brief set the ip to loopback
			 */
			inline void 			set_loopback()
				{ as_int[0] = 0; as_int[1] = 0; as_int[2] = 0; as_int[3] = PP_HTONL(0x00000001UL);; }

			/**
			 * is this ip6 address a loopback address?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_loopback() { return (*this == MNNET_IPV6_ADDRESS_LOOPBACK); }
			/**
			 * is this ip6 address a broadcast address? - return always false
			 */
			virtual bool            is_broadcast() { return false; }
			/**
			 * is this ip6 address a multicast address?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_multicast() { return ( (as_short[0] & 0xFF00 ) == 0xFF00 );  }

			virtual bool 			is_any() { return *this == MNNET_IPV6_ADDRESS_ANY; }
			/**
			 * is the ip6 address  linklocal?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_linklocal() { return ( (as_short[0] & 0xFFC0 ) == 0xFE80 );  }
			/**
			 * is this ip6 address sitelocal?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_sitelocal() { return ( (as_short[0] & 0xFFC0 ) == 0xFEC0 );  }
			/**
			 * is this ip6 address toredo?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_toredo()    { return (  as_short[0] == 0x2001 && as_short[1] == 0  );  }
			/**
			 * is this ip6 address ip4 mapped?
			 * @return If true then yes and false when not
			 */
			virtual bool            is_ip4mapped();
			/**
			 * is this ip6 address equel with the given address
			 * @return If true then equel and if false then not
			 */
			virtual bool            is_equel(const basic_ip6_address& ipOther);
			/**
			 * Compare this ip6 address with (ipOther & netmask)
			 *
			 * @return true if they match, integer different than zero otherwise
			 */
			virtual bool            is_equel(const basic_ip6_address& ipOther, const basic_ip6_address& mask);

			basic_ip6_address& 	operator = (const basic_ip6_address& ip);

			uint8_t 			operator[](int index) const { return m_Numbers[index]; }
			uint8_t& 			operator[](int index) { return m_Numbers[index]; }
			bool 				operator == (const basic_ip6_address& ipOther){ return  is_equel(ipOther); }
			bool 				operator != (const basic_ip6_address& ipOther){ return !is_equel(ipOther); }

			basic_ip6_address 	operator&(const basic_ip6_address& other)const;
			basic_ip6_address 	operator|(const basic_ip6_address& other)const;
			basic_ip6_address 	operator^(const basic_ip6_address& other)const;
			basic_ip6_address 	operator~() const;


			/**
			 * Create a new basic_ip4_address from a given IPv6 address
			 * @param ip6 The IPv6 address to convert
			 * @return The new basic_ip4_address from a given IPv6 address
			 */
			static basic_ip4_address to_ip4(const basic_ip6_address& ip6) {
				return basic_ip4_address(lwip_ntohl(ip6.as_int[3]) );
			}
			/**
			 * @brief Create a new basic_ip6_address from a given IPv4 address
			 * @param ip4 The IPv4 address to convert
			 * @return The new basic_ip6_address from a given IPv4 address
			 */
			static basic_ip6_address from_ip4(const basic_ip4_address& ip4) {
				return basic_ip6_address(0x00000000, 0x00000000, htonl(0xffff), (uint32_t)ip4);
			}
			/**
			 * Create an IPv6 netmask
			 */
			static basic_ip6_address netmask(int mask);
		};
	}
}

#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE

#endif // __MINLIBNET_BASIC_IP6_ADDRESS_H__
