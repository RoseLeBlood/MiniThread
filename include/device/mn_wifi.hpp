/**
 * @file
 * @brief
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
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

#ifndef __MINILIB_NET_BASIC_WIFI_H__
#define __MINILIB_NET_BASIC_WIFI_H__

#include "../mn_config.hpp"


#include <esp_err.h>
#include <esp_event.h>
#include <string>
#include <string.h>

#include "mn_basic_wifi_net_if.hpp"
#include "mn_network_device.hpp"

#include "../mn_eventgroup.hpp"


namespace mn {
	namespace device {
		/**
		 * @brief A basic class for wifi.
		 */
		class basic_wifi : public basic_wifi_net_if {
		public:
			/**
			 * @brief Authenticate modes
			 */
			enum class auth_mode : int {
				Open = WIFI_AUTH_OPEN,						/**< authenticate mode : open */
				WEP = WIFI_AUTH_WEP,						/**< authenticate mode : WEP */
				WPA_PSK = WIFI_AUTH_WPA_PSK,				/**< authenticate mode : WPA_PSK */
				WPA2_PSK = WIFI_AUTH_WPA2_PSK,				/**< authenticate mode : WPA2_PSK */
				//WPA3_PSK = WIFI_AUTH_WPA3_PSK,			/**< authenticate mode : open */
				//WPA2_WPA3_PSK = WIFI_AUTH_WPA2_WPA3_PSK,/**< authenticate mode : open */
				WPA_WPA2_PSK = WIFI_AUTH_WPA_WPA2_PSK,		/**< authenticate mode : WPA_WPA2_PSK */
				WPA2_ENTERPRISE = WIFI_AUTH_WPA2_ENTERPRISE	/**< authenticate mode : WPA2_ENTERPRISE */
			};

			enum class wifi_interface_type {
				Station = 		0,     /**< ESP32 station interface */
    			Acesspoint = 	1,     /**< ESP32 soft-AP interface */
			};
			using auth_mode_t = basic_wifi::auth_mode;
			using interface_t = basic_wifi::wifi_interface_type;

			/**
			 * @brief Construct this object.
			 * @param type Whit type of wifi
			 */
			basic_wifi(const interface_t& type);
			virtual ~basic_wifi();

			virtual bool stop(bool wifioff) { return true; }
	  	protected:
			/**
			 * @brief start the wifi system and set the config.
			 * @param cfg the wifi config.
			 * @param storage Where to save the wifi configurations.
			 * @return
			 *	- ERR_MN_WIFI_OK: No error.
			 *	- ERR_MN_WIFI_CREATE_NETIF: Can create the netif.
			 *	- ERR_MNTHREAD_OUTOFMEM: Not enoug memory for the wifi sub system.
			 *	- ERR_MN_WIFI_REGISTER_EVENT: Can not set the event handler for WIFI_EVENT & IP_EVENT.
			 *  - ERR_MNTHREAD_INVALID_ARG: An invalid argument (storage).
			 *	- ERR_MN_WIFI_SET_MODE: set the current mode.
			 *	- ERR_MNTHREAD_NULL The argument cfg is nullptr.
			 */
			int          startup(wifi_init_config_t* cfg, wifi_storage_t storage = WIFI_STORAGE_RAM);
			/**
			 * @brief Deregister the wifi_event and ip_event.
			 * @return
			 *	- ERR_MN_WIFI_OK: No error
			 *  - all other: error.
			 */
			int          unregist();

			virtual void on_wifi_event(esp_event_base_t event_base, int32_t event_id, void* event_data) { return; }
        	virtual void on_ip_event(esp_event_base_t event_base, int32_t event_id, void* event_data) { return; }
		private:
			static void _wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
			static void _ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
		protected:
			mn::event_group_t   m_eGroup;
		};
	}
}

#endif // __MINILIB_NET_BASIC_WIFI_H__
