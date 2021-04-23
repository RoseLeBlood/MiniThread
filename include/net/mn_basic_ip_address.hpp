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
#ifndef _MINLIB_BASIC_IP_ADDRESS_HPP_
#define _MINLIB_BASIC_IP_ADDRESS_HPP_

#include "../mn_config.hpp"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sockets.h"

#include <array>
#include "mn_net_types.hpp"


#define MNNET_IPV4_ADDRESS_BYTES        4
#define MNNET_IPV6_ADDRESS_BYTES        16
#define MNNET_NUMBER_OF_LABELS          8

#define MNNET_IPV4_ADDRESS_ANY          mn::net::basic_ip4_address( IPADDR_ANY )
#define MNNET_IPV4_ADDRESS_LOOPBACK     mn::net::basic_ip4_address( IPADDR_LOOPBACK )
#define MNNET_IPV4_ADDRESS_BROADCAST    mn::net::basic_ip4_address( IPADDR_BROADCAST )
#define MNNET_IPV4_ADDRESS_NONE         mn::net::basic_ip4_address( IPADDR_NONE )

#define MNNET_IS_CLASSA(ipa)  			IP_CLASSA( ((uint32_t)ipa) )
#define MNNET_IS_CLASSB(ipa)  			IP_CLASSB( ((uint32_t)ipa) )
#define MNNET_IS_CLASSC(ipa)  			IP_CLASSC( ((uint32_t)ipa) )
#define MNNET_IS_CLASSD(ipa)  			IP_CLASSD( ((uint32_t)ipa) )
#define MNNET_IP_MULTICAST(ipa) 		MNNET_IS_CLASSD(ipa)
#define MNNET_IP_EXPERIMENTAL(ipa)		IP_EXPERIMENTAL( ((uint32_t)ipa) )
#define MNNET_IP_BADCLASS(ipa)			IP_BADCLASS( ((uint32_t)ipa) )

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
	#define MNNET_IPV6_NEW_ARRAY_NULL		new uint8_t[MNNET_IPV6_ADDRESS_BYTES]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }
	#define MNNET_IPV6_NEW_ARRAY_LOOP		new uint8_t[MNNET_IPV6_ADDRESS_BYTES]{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }

#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
	#define MNNET_IPV6_ADDRESS_ANY          mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_NULL, 0 )
	#define MNNET_IPV6_ADDRESS_LOOPBACK     mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_LOOP, 0 )
#else
	#define MNNET_IPV6_ADDRESS_ANY          mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_NULL )
	#define MNNET_IPV6_ADDRESS_LOOPBACK     mn::net::basic_ip6_address( MNNET_IPV6_NEW_ARRAY_LOOP )
#endif
	#define MNNET_IPV6_ADDRESS_NONE         MNNET_IPV6_ADDRESS_ANY
#endif

namespace mn {
	namespace net {

		/**
		 * @brief Wrapper class around lwip implementation of a ip address.
		 *
		 * @note To use this, you need to subclass it. All of your ip address should
   		 * be derived from the basic_ip_address class. Then implement the all virtual abstracted
		 * functions.
		 *
		 * @ingroup socket
		 * @author Amber-Sophia Schröck
		 */
		class basic_ip_address {
		public:
			/**
			 * @brief Constructor
			 */
			basic_ip_address(address_family fam) : m_aFamily(fam) { }

			/**
			 * @brief Get the ip address as string
			 *
			 * @return The ip address as string
			 */
			virtual const char*     to_string() = 0;
			/**
			 * @brief Get the ip address as byte array
			 *
			 * @return The ip address as byte array
			 */
			virtual uint8_t*        get_bytes() = 0;
			/**
			 * @brief Is the this address a loopback address?
			 *
			 * @return if true then yes and false if not
			 */
			virtual bool            is_loopback() = 0;
			/**
			 * @brief Is the this address a broadcast address?
			 *
			 * @return if true then yes and false if not
			 */
			virtual bool            is_broadcast() = 0;
			/**
			 * @brief Get the address family
			 *
			 * @return The address family
			 */
			virtual address_family  get_family()    { return m_aFamily; }

		protected:
			address_family  m_aFamily;
		};

		/**
		 * @brief Class for the Internet Protocal version 4
		 */
		class basic_ip4_address : public basic_ip_address {
		protected:
			/**
			 * The type for a basic_ip4_address, as union - for easer internal use
			 */
			union {
				uint8_t  as_array[MNNET_IPV4_ADDRESS_BYTES]; /**!  The ip4 adress as array */
				uint32_t as_int32; /**! The ip4 adress as uint32_t */
			};
		public:
			/**
			 * Construtor
			 */
			basic_ip4_address();
			/**
			 * Construtor
			 * @brief construct a ip4 address from uint32_t
			 * @param newAddress The ip4 address as uint32_t
			 */
			explicit basic_ip4_address(uint32_t newAddress);

			/**
			 * Construtor
			 * @brief construct a ip4 address from array
			 * @param adress The ip4 address as array
			 */
			basic_ip4_address(uint8_t adress[MNNET_IPV4_ADDRESS_BYTES]);
			/**
			 * Construtor
			 * @brief construct a ip4 address from bytes
			 * @param a The first octan
			 * @param b The second octan
			 * @param c The third octan
			 * @param d The 4. octan
			 */
			basic_ip4_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d );
			/**
			 * Construtor
			 * @brief construct a ip4 address from uint32_t
			 * @param newAddress The ip4 address as uint32_t
			 */
			basic_ip4_address(const char* str_ip);

			/**
			 * Get the ip4 address as array
			 * @return the ip4 address as array
			 */
			uint8_t*       		get_bytes();

			inline void			set_zero() 		{ as_int32 = 0; }
			inline void			set_any()  		{ as_int32 = IPADDR_ANY; }
			inline void			set_broadcast() { as_int32 = IPADDR_BROADCAST; }
			inline void			set_loop() 		{ as_int32 = IPADDR_LOOPBACK; }

			/**
			 * is the given ip4address equel with this
			 * @return If true then equel and if false then not
			 */
			virtual bool   		is_equel(const basic_ip4_address& ipOther);
			/**
			 * is the ip4 address a loopback address?
			 * @return If true then yes and false when not
			 */
			inline bool 			is_loopback()  { return (*this == MNNET_IPV4_ADDRESS_LOOPBACK); }
			/**
			 * is the ip4 address a broadcast address?
			 * @return If true then yes and false when not
			 */
			inline bool 			is_broadcast() { return (*this == MNNET_IPV4_ADDRESS_BROADCAST); }
			/**
			 * is the ip4 address a multicast address?
			 * @return If true then yes and false when not
			 */
			inline bool 			is_multicast() { return ( as_int32 & PP_HTONL(0xf0000000UL) ) == PP_HTONL(0xe0000000UL); }
			/**
			 * is the ip4 address link local?
			 * @return If true then yes and false when not
			 */
			inline bool 			is_linklocal() { return ( as_int32 & PP_HTONL(0xf0000000UL) ) == PP_HTONL(0xa9fe0000UL); }

			/**
			 * get the ip adress as string
			 * @return The ip4 address as string
			 */
			virtual const char* to_string();
			/**
			 * calculate a broadcast address from this address and the given subnet address
			 * @return The calculated broadcast address
			 */
			basic_ip4_address 	calc_broadcast(const basic_ip4_address& subnet);
			/**
			 * calculate a  network id address from this address and the given subnet address
			 * @return The calculated network id address
			 */
			basic_ip4_address 	calc_network_id(const basic_ip4_address& subnet);
			/**
			 * @brief Get the subnet_cidr from the given netmask
			 *
			 * @return The calculate subnet_cidr
			 */
			static uint8_t 		get_subnet_cidr(const basic_ip4_address& subnet);

			uint8_t 			operator[](int index) const { return as_array[index]; }
			uint8_t& 			operator[](int index) { return as_array[index]; }
			bool 				operator == (const basic_ip4_address& ipOther){ return  is_equel(ipOther); }
			bool 				operator != (const basic_ip4_address& ipOther){ return !is_equel(ipOther); }
			bool 				operator == (const uint8_t* addr);
			basic_ip4_address& 	operator=(uint32_t address);
			basic_ip4_address 	operator&(const basic_ip4_address& other)const;
			basic_ip4_address 	operator|(const basic_ip4_address& other)const;
			basic_ip4_address 	operator^(const basic_ip4_address& other)const;
			basic_ip4_address 	operator~() const;
			operator uint32_t() const { return as_int32; }
		};


#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES
		/**
		 * @brief Class for the Internet Protocal version 6
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
			virtual const char*     to_string();
			/**
			 * Get this ip6 address as array
			 * @return the ip6 address as array
			 */
			virtual uint8_t*        get_bytes();
			virtual uint32_t		get_int(int index)  { return index < 4 ? as_int[index] : 0; }

			inline void				set_zero()
				{ as_int[0] = 0; as_int[1] = 0; as_int[2] = 0; as_int[3] = 0; }
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


			static basic_ip4_address to_ip4(const basic_ip6_address& ip4) {
				return basic_ip4_address(lwip_ntohl(ip4.as_int[3]) );
			}
			static basic_ip6_address from_ip4(const basic_ip4_address& ip4) {
				return basic_ip6_address(0x00000000, 0x00000000, htonl(0xffff), (uint32_t)ip4);
			}
			/**
			 * Create an IPv6 netmask
			 */
			static basic_ip6_address netmask(int mask);
		};

#endif
	}
}

#endif // BASIC_IP_ADDRESS_HPP
