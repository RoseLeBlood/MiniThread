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

#include "mn_algorithm.hpp"
#include "mn_net_types.hpp"


#define MNNET_IS_CLASSA(ipa)  			IP_CLASSA( ((uint32_t)ipa) )
#define MNNET_IS_CLASSB(ipa)  			IP_CLASSB( ((uint32_t)ipa) )
#define MNNET_IS_CLASSC(ipa)  			IP_CLASSC( ((uint32_t)ipa) )
#define MNNET_IS_CLASSD(ipa)  			IP_CLASSD( ((uint32_t)ipa) )
#define MNNET_IP_MULTICAST(ipa) 		MNNET_IS_CLASSD(ipa)
#define MNNET_IP_EXPERIMENTAL(ipa)		IP_EXPERIMENTAL( ((uint32_t)ipa) )
#define MNNET_IP_BADCLASS(ipa)			IP_BADCLASS( ((uint32_t)ipa) )



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
			 * @brief Returns a string containing a representation of the address in presentation format.
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
			 * @brief Get the address family (AF_INET or AF_INET6) of the address.
			 * @return The address family
			 */
			virtual address_family  get_family()    { return m_aFamily; }

			/**
			 * @brief Returns the length in bytes of the ip address
			 * @return The length in bytes of the ip address
			 */
			virtual int length() const = 0;

			virtual void swap(basic_ip_address& rhs) noexcept {
				mn::swap<address_family>(m_aFamily, rhs.m_aFamily);
			}
		protected:
			/**
			 * @brief  A saved / cached copy of the address family type of this socket.
			 */
			address_family  m_aFamily;
		};
	}
}

#endif // BASIC_IP_ADDRESS_HPP
