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
#ifndef __MINILIB_BASIC_WIFI_NET_IF_H__
#define __MINILIB_BASIC_WIFI_NET_IF_H__

#include "../mn_config.hpp"
#include "mn_basic_netif.hpp"

#include <esp_eth.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_log.h>


#include <cstring>

namespace mn {
	namespace net {
		/**
		 * @brief Wrapper basic WiFi class around esp-idf implementation of a esp-netif device.
		 * @note Class for STA and soft AP class
		 * @ingroup device
		 */
		class basic_wifi_net_if : public basic_net_if {
		public:
			/**
			 * @brief list of the wifi protocols
			 */
			enum class wifi_protocol : uint8_t {
				none = 0,

				b = WIFI_PROTOCOL_11B,			/**< wifi protocoll .11B, use only B */
				g = WIFI_PROTOCOL_11G,			/**< wifi protocoll .11G, use only G */
				n = WIFI_PROTOCOL_11N,			/**< wifi protocoll .11N, use only N */
				bg = WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G,		/**< protocoll .11BG */
				bgn = WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N, /**< protocoll .11BGN */
				lr = WIFI_PROTOCOL_LR, /**< protocol for lang range */
			};
			/**
			 * @brief Construt
			 * @param type The type of the using interface
			 */
			basic_wifi_net_if(const esp_interface_t& type) noexcept;

			/**
			 * @brief Set the hostname
			 * @param hostname The new hostname
			 * @return true If set the new hostname and false if not
			 */
			virtual bool    		create_default(void);

			/**
			 * @brief get the type of the interface
			 * @return reference of the type
			 */
			esp_interface_t&        get(void);
			/**
			 * @brief Get interface hostname.
			 * @return Returns a pointer to the hostname
			 */
			const char*     		get_hostname(void);
			/**
			 * @brief Get current operating mode of WiFi
			 * @return The current WiFi mode
			 */
			wifi_mode_t     		get_mode(void);
			/**
			 * @brief Get the ssid for the wifi
			 * @param [out] ssid  store the ssid this wifi
			 * @return the ssid for the wifi
			 */
			const char*     		get_ssid(char ssid[32]);

			/**
			 * @brief Get the current protocol bitmap
			 *
			 * @return The current protocol bitmapfx or wifi_protocol::none on error
			 */
			wifi_protocol 			get_protocol();
			/**
			  * @brief     Get current WiFi power save type
			  * @attention Default power save type is WIFI_PS_MIN_MODEM.
			  * @return    The current power save type
			  */
			wifi_ps_type_t			get_power_save_mode();


			/**
			  * @brief     Set current WiFi power save type
			  * @attention Default power save type is WIFI_PS_MIN_MODEM.
			  * @param     mode  power save type
			  * @return    NO_ERROR: succeed
			  */
			int						set_power_save_mode(const wifi_ps_type_t mode);
			/**
			  * @brief     Set protocol type of specified interface
			  *            The default protocol is (WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N)
			  *
			  * @attention Currently we only support 802.11b or 802.11bg or 802.11bgn mode
			  * @param     wifi_protocol  WiFi protocol
			  *
			  * @return
			  *    - NO_ERROR: succeed
			  *    - ERR_MNTHREAD_NULL: WiFi is not initialized by esp_wifi_init
			  *    - ERR_MNTHREAD_INVALID_ARG: invalid interface
			  */
			int						set_protocol(const wifi_protocol& prot = wifi_protocol::bgn);


		protected:
			/**
			 * @brief NET_IF intarface of the device
			 */
			esp_interface_t 	m_ifInterface;
		};
	}
}

#endif // __MINILIB_BASIC_WIFI_NET_IF_H__
