/*
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 3, or (at your option) any later version.
*
* The Mini Thread Library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.
*/
#ifndef __MINLIB_BASIC_IP4_ADDRESS_H__
#define __MINLIB_BASIC_IP4_ADDRESS_H__

#include "../mn_config.hpp"

#define MNNET_IPV4_ADDRESS_BYTES        4

#define MNNET_IPV4_ADDRESS_ANY          mn::net::basic_ip4_address( IPADDR_ANY )
#define MNNET_IPV4_ADDRESS_LOOPBACK     mn::net::basic_ip4_address( IPADDR_LOOPBACK )
#define MNNET_IPV4_ADDRESS_BROADCAST    mn::net::basic_ip4_address( IPADDR_BROADCAST )
#define MNNET_IPV4_ADDRESS_NONE         mn::net::basic_ip4_address( IPADDR_NONE )

#include "mn_basic_ip_address.hpp"

namespace mn {
	namespace net {
		/**
		 * @brief This class represents an internet (IP) version 6 host address.
		 * @ingroup socket
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
			 * @brief Construct a none IPv4 basic_ip4_address.
			 */
			basic_ip4_address();
			/**
			 * @brief Construct a ip4 address from uint32_t
			 * @param newAddress The ip4 address as uint32_t
			 */
			explicit basic_ip4_address(uint32_t newAddress) noexcept;

			/**
			 * @brief Construct a ip4 address from array
			 * @param adress The ip4 address as array
			 */
			basic_ip4_address(uint8_t adress[MNNET_IPV4_ADDRESS_BYTES]) noexcept;
			/**
			 * @brief Construct a ip4 address from bytes
			 * @param a The first octan
			 * @param b The second octan
			 * @param c The third octan
			 * @param d The 4. octan
			 */
			basic_ip4_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d ) noexcept;
			/**
			 * @brief Construct an basic_ip4_address from the string containing
			 */
			basic_ip4_address(const char* str_ip) noexcept;
			/**
			 *
			 * @brief Construct an basic_ip4_address by copying another one.
			 * @param other The soucre of this copying address
			 */
			basic_ip4_address(const basic_ip4_address& other) noexcept;

			/**
			 * @brief Get the ip4 address as array
			 * @return the ip4 address as array
			 */
			uint8_t*       		get_bytes();

			/**
			 * @brief Returns the length in bytes of the ip address
			 * @return The length in bytes of the ip address
			 */
			int length() const;
			/**
			 * @brief Get the ip4 address as array
			 */
			inline void			set_zero() 		{ as_int32 = 0; }
			/**
			 * @brief Set the ip as ANY Address
			 */
			inline void			set_any()  		{ as_int32 = IPADDR_ANY; }
			/**
			 * @brief Set the ip as BROADCAST Address
			 */
			inline void			set_broadcast() { as_int32 = IPADDR_BROADCAST; }
			/**
			 * @brief Set the ip as LOOPBACK Address
			 */
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
			 * @brief Returns a string containing a representation of the address in presentation format.
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

			/**
			 * @brief array get opertor on the uint8_t array[4]
			 */
			uint8_t 			operator[](int index) const { return as_array[index]; }
			/**
			 * @brief array get opertor on the uint8_t array[4]
			 */
			uint8_t& 			operator[](int index) { return as_array[index]; }
			/**
			 * @brief equel operator
			 */
			bool 				operator == (const basic_ip4_address& ipOther){ return  is_equel(ipOther); }
			/**
			 * @brief unequel operator
			 */
			bool 				operator != (const basic_ip4_address& ipOther){ return !is_equel(ipOther); }
			/**
			 * @brief equel operator
			 */
			bool 				operator == (const uint8_t* addr);

			/**
			 * @brief Assigns an basic_ip4_address.
			 */
			basic_ip4_address& 	operator=(const uint32_t& address);
			/**
			 * @brief Assigns an basic_ip4_address.
			 */
			basic_ip4_address& 	operator=(const basic_ip4_address& address);
			/**
			 * @brief arametic AND operator
			 * @return this-ip AND other
			 */
			basic_ip4_address 	operator&(const basic_ip4_address& other)const;
			/**
			 * @brief arametic OR operator
			 * @return this-ip OR other
			 */
			basic_ip4_address 	operator|(const basic_ip4_address& other)const;
			/**
			 * @brief arametic XOR operator
			 * @return this-ip XOR other
			 */
			basic_ip4_address 	operator^(const basic_ip4_address& other)const;
			/**
			 * @brief arametic NICHT operator
			 * @return this-ip NICHT other
			 */
			basic_ip4_address 	operator~() const;
			/**
			 * @brief cast operator
			 */
			operator uint32_t() const { return as_int32; }

			virtual void swap(basic_ip4_address& rhs) noexcept {
				basic_ip_address::swap(rhs);
				mn::swap<uint32_t>(as_int32, rhs.as_int32);
			}
		};
	}
}

#endif // __MINLIB_BASIC_IP4_ADDRESS_H__
