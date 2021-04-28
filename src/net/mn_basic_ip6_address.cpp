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

#if MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE == MN_THREAD_CONFIG_YES

#include "net/mn_basic_ip6_address.hpp"


namespace mn {
	namespace net {
		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address()
        	: basic_ip_address(address_family::inet_v6) {
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
        	m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
#endif
        	as_int[0] = 0;
  			as_int[1] = 0;
  			as_int[2] = 0;
  			as_int[3] = 0;
		}
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(uint8_t address[16], int scopid)
			: basic_ip_address(address_family::inet_v6) {

			for (signed char i = 0; i < MNNET_NUMBER_OF_LABELS; i++) {
				m_Numbers[i] = (adress[i * 2] * 256 + adress[i * 2 + 1]);
			}
			m_ScopeId = scopid;

		}
#else
		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(uint8_t adress[MNNET_IPV6_ADDRESS_BYTES])
			: basic_ip_address(address_family::inet_v6) {

			for (signed char i = 0; i < MNNET_NUMBER_OF_LABELS; i++) {
				m_Numbers[i] = (adress[i * 2] * 256 + adress[i * 2 + 1]);
			}
		}
#endif

		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(uint32_t ax, uint32_t bx, uint32_t cx, uint32_t dx)
			: basic_ip_address(address_family::inet_v6) {
		#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
		#endif
        	as_int[0] = ax;
  			as_int[1] = bx;
  			as_int[2] = cx;
  			as_int[3] = dx;

		}

		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(uint32_t aa[4])
			: basic_ip_address(address_family::inet_v6) {
		#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
		#endif
        	as_int[0] = aa[0];
  			as_int[1] = aa[1];
  			as_int[2] = aa[2];
  			as_int[3] = aa[3];

		}


		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(const basic_ip6_address& ip)
			: basic_ip_address(address_family::inet_v6) {
			mempcpy(m_Numbers, ip.m_Numbers, sizeof(m_Numbers));
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
#endif
		}

		//-----------------------------------
		//  basic_ip6_address
		//-----------------------------------
		basic_ip6_address::basic_ip6_address(const char* str_ip)
			: basic_ip_address(address_family::inet_v6) {
			char * _pos = (char*)str_ip;
			int8_t _len = static_cast<int8_t>(strlen(str_ip));

			if(_len == 39) {
				for(int8_t i = 0; i < 16; i+=2, _pos += 5) {
					if(!sscanf(_pos, "%2hhx", &m_Numbers[i]) || !sscanf(_pos + 2, "%2hhx", &m_Numbers[i+1])) {
						memset(m_Numbers, 0, sizeof(m_Numbers)); return;
					}
				}
			} else {
				memset(m_Numbers, 0, sizeof(m_Numbers));
			}
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
        	m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
#endif
		}

		//-----------------------------------
		//  get_bytes
		//-----------------------------------
		uint8_t* basic_ip6_address::get_bytes() {
			uint8_t* _retBytes;

			_retBytes = new uint8_t[MNNET_IPV6_ADDRESS_BYTES];

			for ( int i = 0, j = 0; i < MNNET_NUMBER_OF_LABELS; i++) {
				_retBytes[j++] = ((m_Numbers[i] >> 8) & 0xFF);
				_retBytes[j++] = ((m_Numbers[i]     ) & 0xFF);
			}

			return _retBytes;
		}

		//-----------------------------------
		//  lenght
		//-----------------------------------
		int basic_ip6_address::length() const {
			return MNNET_IPV6_ADDRESS_BYTES;
		}

		//-----------------------------------
		//  is_ip4mapped
		//-----------------------------------
		bool basic_ip6_address::is_ip4mapped() {
			return (
						(as_int[0] == 0) &&
						(as_int[1] == 0) &&
						(as_int[2]) == PP_HTONL(0x0000FFFFUL)
				   );
		}

		//-----------------------------------
		//  is_equel
		//-----------------------------------
		bool basic_ip6_address::is_equel(const basic_ip6_address& ipOther) {
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			if(m_ScopeId != ipOther.m_ScopeId) return false;
#endif

			return memcmp(as_int, &ipOther.as_int, sizeof(as_int)) == 0;
		}

		//-----------------------------------
		//  is_equel
		//-----------------------------------
		bool basic_ip6_address::is_equel(const basic_ip6_address& ipOther, const basic_ip6_address& mask) {
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
			if(m_ScopeId != ipOther.m_ScopeId) return false;
#endif
			uint32_t masked[4] = { ipOther.as_int[0] & mask.as_int[0],
								   ipOther.as_int[1] & mask.as_int[0],
								   ipOther.as_int[2] & mask.as_int[0],
								   ipOther.as_int[3] & mask.as_int[0] };

			return memcmp(as_int, &masked, sizeof(masked)) == 0;
		}

		//-----------------------------------
		//  operator =
		//-----------------------------------
		basic_ip6_address& basic_ip6_address::operator = (const basic_ip6_address& ip) {
			m_aFamily = ip.m_aFamily;
			mempcpy(m_Numbers, ip.m_Numbers, sizeof(m_Numbers) );
#if MN_THREAD_CONFIG_NET_IPADDRESS6_USE_SCOPEID  == MN_THREAD_CONFIG_YES
        	m_ScopeId = MN_THREAD_CONFIG_NET_IPADDRESS6_SCOPEID_VAL;
#endif
			return *this;
		}

		//-----------------------------------
		//  netmask
		//-----------------------------------
		basic_ip6_address basic_ip6_address::netmask(int mask) {
			uint32_t _netmask[4] = {0};
			if (mask < 0) mask = 0; else if (128 < mask) mask = 128;

			for (int i=0; 32 < mask; i++) {
				_netmask[i] = 0xffffffff;
				mask -= 32;
			}
			if (mask != 0) {
				*_netmask = htonl(0xFFFFFFFF << (32 - mask));
			}

			return basic_ip6_address(_netmask);
		}

		//-----------------------------------
		//  to_string
		//-----------------------------------
		const char* basic_ip6_address::to_string() {
			char* szRet = new char[40];
			sprintf(szRet,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
					m_Numbers[0], m_Numbers[1], m_Numbers[2], m_Numbers[3],
					m_Numbers[4], m_Numbers[5], m_Numbers[6], m_Numbers[7],
					m_Numbers[8], m_Numbers[9], m_Numbers[10], m_Numbers[11],
					m_Numbers[12], m_Numbers[13], m_Numbers[14], m_Numbers[15]);
			return szRet;
		}

		//-----------------------------------
		//  operator &
		//-----------------------------------
		basic_ip6_address 	basic_ip6_address::operator & (const basic_ip6_address& other)const {
			return basic_ip6_address(as_int[0] & other.as_int[0],
									 as_int[1] & other.as_int[1],
									 as_int[2] & other.as_int[2],
									 as_int[3] & other.as_int[3]);
		}

		//-----------------------------------
		//  operator |
		//-----------------------------------
		basic_ip6_address 	basic_ip6_address::operator | (const basic_ip6_address& other)const {
			return basic_ip6_address(as_int[0] | other.as_int[0],
									 as_int[1] | other.as_int[1],
									 as_int[2] | other.as_int[2],
									 as_int[3] | other.as_int[3]);
		}

		//-----------------------------------
		//  operator ^
		//-----------------------------------
		basic_ip6_address 	basic_ip6_address::operator ^ (const basic_ip6_address& other)const {
			return basic_ip6_address(as_int[0] ^ other.as_int[0],
									 as_int[1] ^ other.as_int[1],
									 as_int[2] ^ other.as_int[2],
									 as_int[3] ^ other.as_int[3]);
		}

		//-----------------------------------
		//  operator ~
		//-----------------------------------
		basic_ip6_address 	basic_ip6_address::operator ~ () const {
			return basic_ip6_address(as_int[0] ^ 0xffffffff,
									 as_int[1] ^ 0xffffffff,
									 as_int[2] ^ 0xffffffff,
									 as_int[3] ^ 0xffffffff);
		}


	}
}

#endif // MN_THREAD_CONFIG_NET_IPADDRESS6_ENABLE
