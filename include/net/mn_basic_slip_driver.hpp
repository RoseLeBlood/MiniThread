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
#ifndef __MINILIB_BASIC_SLIP_DRIVER_H__
#define __MINILIB_BASIC_SLIP_DRIVER_H__

#include "../mn_config.hpp"
#include "net/mn_basic_netif.hpp"

#ifdef CONFIG_LWIP_PPP_SUPPORT


#include <esp_netif.h>
#include <esp_log.h>
#include <esp_netif_slip.h>
#include <esp_netif_net_stack.h>
#include <esp_netif_lwip_slip.h>

namespace mn {
	namespace net {
		class basic_slip_driver : public basic_ppp_net_if {
		public:
			basic_slip_driver(uint8_t uart_num = UART_NUM_2)
				: basic_slip_driver(uart_num, 4, 36, 115200, 1024) { }
			basic_slip_driver(uint8_t uart_num, uint8_t tx_pin, uint8_t rx_pin)
				: basic_slip_driver(uart_num, tx_pin, rx_pin, 115200, 1024) { }
			basic_slip_driver(uint8_t uart_num, uint8_t tx_pin, uint8_t rx_pin,
							  uint32_t baud, uint32_t buffer_len) ;

			virtual int start();

			virtual ip4_address     get_ip() 			{ return MNNET_IPV4_ADDRESS_ANY;  }
			virtual ip4_address     get_broadcast() 	{ return MNNET_IPV4_ADDRESS_ANY;  }
			virtual ip4_address     get_gateway() 		{ return MNNET_IPV4_ADDRESS_ANY;  }
			virtual ip4_address     get_netmask() 		{ return MNNET_IPV4_ADDRESS_ANY;  }

			void set_pin(const uint8_t& pin, const uint8_t& rx_pin);
			void set_serial(const uint8_t& uart_num, const uint32_t& baud);
			void set_buffer(const uint32_t& buffer_len);
			void set_ip(const ip4_address& ip);
		private:
			esp_netif_slip_config_t m_slipConfig;
			esp_netif_netstack_config_t m_slipNetStackConfig;
		};
	}
}
#endif // CONFIG_LWIP_PPP_SUPPORT
#endif // __MINILIB_BASIC_SLIP_DRIVER_H__
