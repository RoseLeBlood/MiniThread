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


#if CONFIG_LWIP_SLIP_SUPPORT
#include "net/mn_basic_slip_driver.hpp"

namespace mn {
	namespace net {

		//-----------------------------------
		//  basic_slip_driver::basic_slip_driver
		//-----------------------------------
		basic_slip_driver::basic_slip_driver(uint32_t uart_num, uint8_t tx_pin, uint8_t rx_pin,
											uint32_t baud, uint32_t buffer_len)
			: basic_slip_net_if() {
			m_slipConfig.uart_tx_pin = tx_pin;
			m_slipConfig.uart_rx_pin = rx_pin;
			m_slipConfig.uart_dev = uart_num;
			m_slipConfig.uart_baud = baud;
			m_slipConfig.rx_buffer_len = buffer_len;
		}

		//-----------------------------------
		//  basic_slip_driver::set_pin
		//-----------------------------------
		void basic_slip_driver::set_pin(const uint8_t& tx_pin, const uint8_t& rx_pin) {
			m_slipConfig.uart_tx_pin = tx_pin;
			m_slipConfig.uart_rx_pin = rx_pin;
		}

		//-----------------------------------
		//  basic_slip_driver::set_serial
		//-----------------------------------
		void basic_slip_driver::set_serial(const uint8_t& uart_num, const uint32_t& baud) {
			m_slipConfig.uart_dev = uart_num;
			m_slipConfig.uart_baud = baud;
		}

		//-----------------------------------
		//  basic_slip_driver::set_buffer
		//-----------------------------------
		void basic_slip_driver::set_buffer(const uint32_t& buffer_len) {
			m_slipConfig.rx_buffer_len = buffer_len;
		}

		//-----------------------------------
		//  basic_slip_driver::set_ip
		//-----------------------------------
		void basic_slip_driver::set_ip(const ip4_address& ip) {
			MN_UNUSED_VARIABLE(ip);
		}

		//-----------------------------------
		//  basic_slip_driver::start
		//-----------------------------------
		int basic_slip_driver::start() {
			if(!create_default() ) return ERR_MNTHREAD_NULL;

			// Apply updated slip config
			ESP_ERROR_CHECK(esp_netif_slip_set_params(m_pNetIf, &m_slipConfig));
			void* slip_driver = esp_slip_new_netif_glue(m_pNetIf);

			// TODO: do we need to do any of this?
			// We _could_ have the uart / thread be part of the driver perhaps
			//ESP_ERROR_CHECK(esp_slip_driver_install(&config, &slip_ctx));

			ESP_ERROR_CHECK(esp_netif_attach(m_pNetIf, slip_driver));

			return ESP_OK;
		}
	}
}
#endif // CONFIG_LWIP_PPP_SUPPORT

