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
#include "net/mn_basic_ip4_address.hpp"


namespace mn {
	namespace net {
		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address()
			: basic_ip4_address(IPADDR_NONE) { }

		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address(uint32_t newAddress) noexcept
			: basic_ip_address(address_family::inet_v4), as_int32(newAddress) { }


		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address(uint8_t address[MNNET_IPV4_ADDRESS_BYTES]) noexcept
			: basic_ip_address(address_family::inet_v4){

			as_array[0] = address[0];
			as_array[1] = address[1];
			as_array[2] = address[2];
			as_array[3] = address[3];
		}
		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d ) noexcept
			: basic_ip_address(address_family::inet_v4) {
			as_array[0] = a;
			as_array[1] = b;
			as_array[2] = c;
			as_array[3] = d;
		}

		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address(const char* address) noexcept
			: basic_ip_address(address_family::inet_v4) {

			uint16_t 	_acc = 0; // Accumulator
			uint8_t 	_dots = 0;
			uint32_t 	_length = strlen(address);
			char		_char = ' ';

			for(int i = 0; i < _length; i++) {
				_char = address[i];
				if (_char >= '0' && _char <= '9') {
					_acc = _acc * 10 + (_char - '0');
					if (_acc > 255) { as_int32 = 0; return; }
				} else if (_char == '.') {
					if (_dots == 3) { as_int32 = 0; return; }
					as_array[_dots++] = _acc;
					_acc = 0;
				}
				else { as_int32 = 0; return; }
			}

			if (_dots != 3) { as_int32 = 0; return; }
			as_array[3] = _acc;
		}

		//-----------------------------------
		//  basic_ip4_address
		//-----------------------------------
		basic_ip4_address::basic_ip4_address(const basic_ip4_address& other) noexcept
			: basic_ip_address(address_family::inet_v4), as_int32(other.as_int32) { }

		//-----------------------------------
		//  get_bytes
		//-----------------------------------
		uint8_t* basic_ip4_address::get_bytes() {
			return as_array;
		}

		//-----------------------------------
		//  lenght
		//-----------------------------------
		int basic_ip4_address::length() const {
			return MNNET_IPV4_ADDRESS_BYTES;
		}

		//-----------------------------------
		//  is_equel
		//-----------------------------------
		bool basic_ip4_address::is_equel(const basic_ip4_address& ipOther) {
			if(m_aFamily != ipOther.m_aFamily) return false;
			return as_int32 == ipOther.as_int32;
		}

		//-----------------------------------
		//  operator =
		//-----------------------------------
		basic_ip4_address& basic_ip4_address::operator = (const uint32_t& address) {
			//
			as_int32 = address;
			return *this;
		}
		//-----------------------------------
		//  operator !=
		//-----------------------------------
		basic_ip4_address& basic_ip4_address::operator=(const basic_ip4_address& address) {
			m_aFamily = address.m_aFamily;
			as_int32 = address.as_int32;
			return *this;
		}

		//-----------------------------------
		//  operator ==
		//-----------------------------------
		bool basic_ip4_address::operator == (const uint8_t* addr) {
			return memcmp(addr, as_array, sizeof(as_array) )  == 0;
		}

		//-----------------------------------
		//  operator &
		//-----------------------------------
		basic_ip4_address 	basic_ip4_address::operator & (const basic_ip4_address& other)const {
			return basic_ip4_address(as_int32 & other.as_int32);
		}

		//-----------------------------------
		//  operator |
		//-----------------------------------
		basic_ip4_address 	basic_ip4_address::operator | (const basic_ip4_address& other)const {
			return basic_ip4_address(as_int32 | other.as_int32);
		}

		//-----------------------------------
		//  operator ^
		//-----------------------------------
		basic_ip4_address 	basic_ip4_address::operator ^ (const basic_ip4_address& other)const {
			return basic_ip4_address(as_int32 ^ other.as_int32);
		}

		//-----------------------------------
		//  operator ~
		//-----------------------------------
		basic_ip4_address 	basic_ip4_address::operator ~ () const {
			return basic_ip4_address(as_int32 ^ 0xffffffff);
		}

		//-----------------------------------
		//  to_string
		//-----------------------------------
		const char* basic_ip4_address::to_string() {
			char* szRet = (char*)malloc(IP4ADDR_STRLEN_MAX * sizeof(char));
			if(szRet == 0) return "";

			snprintf(szRet, IP4ADDR_STRLEN_MAX, "%u.%u.%u.%u", as_array[0], as_array[1], as_array[2], as_array[3]);
			return szRet;
		}

		//-----------------------------------
		//  calc_broadcast
		//-----------------------------------
		basic_ip4_address basic_ip4_address::calc_broadcast(const basic_ip4_address& subnet) {
			uint8_t 		   _broadcast[MNNET_IPV4_ADDRESS_BYTES];
			basic_ip4_address* _ip = this;

			for (int i = 0; i < MNNET_IPV4_ADDRESS_BYTES; i++) {
				_broadcast[i] = ~subnet[i] | _ip[i];
			}
			return basic_ip4_address(_broadcast);
		}
		//-----------------------------------
		//  calc_network_id
		//-----------------------------------
		basic_ip4_address basic_ip4_address::calc_network_id(const basic_ip4_address& subnet) {
			uint8_t 		   _network_id[MNNET_IPV4_ADDRESS_BYTES];
			basic_ip4_address* _ip = this;

			for (int i = 0; i < MNNET_IPV4_ADDRESS_BYTES; i++) {
				_network_id[i] = subnet[i] & _ip[i];
			}
			return basic_ip4_address(_network_id);
		}
		//-----------------------------------
		//  get_subnet_cidr
		//-----------------------------------
		uint8_t basic_ip4_address::get_subnet_cidr(const basic_ip4_address& subnet) {
			uint8_t _cidr = 0;

			for (uint8_t i = 0; i < MNNET_IPV4_ADDRESS_BYTES; i++) {
				if (subnet[i] == 0x80)  // 128
					_cidr += 1;
				else if (subnet[i] == 0xC0)  // 192
					_cidr += 2;
				else if (subnet[i] == 0xE0)  // 224
					_cidr += 3;
				else if (subnet[i] == 0xF0)  // 242
					_cidr += 4;
				else if (subnet[i] == 0xF8)  // 248
					_cidr += 5;
				else if (subnet[i] == 0xFC)  // 252
					_cidr += 6;
				else if (subnet[i] == 0xFE)  // 254
					_cidr += 7;
				else if (subnet[i] == 0xFF)  // 255
					_cidr += 8;
			}

			return _cidr;
		}
	}
}
