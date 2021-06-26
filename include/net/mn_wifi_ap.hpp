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
#ifndef __MINILIB_NET_WIFI_AP_H__
#define __MINILIB_NET_WIFI_AP_H__

#include "../mn_config.hpp"

#include "mn_basic_ip4_address.hpp"
#include "mn_wifi.hpp"

namespace mn {
	namespace net {

		/**
		 * @brief Wifi AP class for the esp32
		 */
		class basic_wifi_ap : public basic_wifi {
			using base_type = basic_wifi;
		public:
			using ip4_adress_t = basic_ip4_address;
			using auth_mode_t = typename base_type::auth_mode;
			using interface_t = typename base_type::wifi_interface_type;

			/**
			 * @brief Construct the wifi ap object.
			 */
			basic_wifi_ap() : basic_wifi(interface_t::Acesspoint) { }
			/**
			 * @brief Basic destructor, stopped the Wifi.
			 */
			~basic_wifi_ap() { stop(true); }

			basic_wifi_ap(const basic_wifi_ap& other) = delete;
			basic_wifi_ap& operator = (const basic_wifi_ap& other) = delete;

			/**
			 * @brief
			 * @param ssid The ssid for the wifi (min lenght 8)
			 * @param mode authenticate mode.
			 * @param passphrase The password for the wifi, only use when mode not 'auth_mode_t::Open' is.
			 * @param storeCfgToFlash If true then store the config to memory and nvs.
			 * @param ipv6 When true then enable IPv6 after wifi creating.
			 * @param channel channel of target AP.
			 * @param ssid_hidden Broadcast SSID or not, default true, broadcast the SSID.
			 * @param max_connection Max number of stations allowed to connect in, default 4, max 10.
			 * @return
			 *		- ERR_MN_WIFI_SSID: SSID is invalid
			 *		- ERR_MN_WIFI_PASSWORD: Password is to short.
			 *		- ERR_MN_WIFI_NOT_INIT: WiFi is not initialized.
			 *		- ERR_MNTHREAD_INVALID_ARG: invalid argument.
			 *		- ERR_MN_WIFI_IF: invalid interface.
			 *		- ERR_MN_WIFI_MODE: invalid mode.
			 *		- ERR_MN_WIFI_NVS: WiFi internal NVS error.
			 *		- ERR_MNTHREAD_OUTOFMEM: out of memory.
			 *		- ERR_MN_WIFI_CONN: WiFi internal error, station or soft-AP control block wrong.
			 *		- ERR_MN_WIFI_OK: No error.
			 */
			int start(const char* ssid, auth_mode_t mode = auth_mode_t::Open, const char* passphrase = "",
				   bool storeCfgToFlash = false,
                   bool ipv6 = false, int channel = 13, bool ssid_hidden = false,
                   int max_connection = 4);

			/**
			 * @brief Set IPv4 infos, local ip, gateway and subnet.
			 * @return If true then set all address anh false if not.
			 */
			bool set_ipinfo(const ip4_adress_t& local_ip, const ip4_adress_t& gateway, const ip4_adress_t& subnet);
			/**
			 * @brief
			 * @return
			 */
        	bool stop(bool wifioff = false) override;

        	/**
        	 * @brief Get Tte IPv4 address of the WiFi AP device.
			 * @return The IP address or MNNET_IPV4_ADDRESS_NONE on error.
        	 */
        	virtual ip4_adress_t      get_ip();
        	/**
        	 * @brief Get the broadcast IP address,
			 * @return The broadcast IP address or MNNET_IPV4_ADDRESS_NONE on error.
        	 */
			virtual ip4_adress_t      get_broadcast();
			/**
        	 * @brief Get the gateway IP address,
			 * @return The gateway IP address or MNNET_IPV4_ADDRESS_NONE on error.
        	 */
			virtual ip4_adress_t      get_gateway();
			/**
        	 * @brief Get the netmask IP address,
			 * @return The netmask IP address or MNNET_IPV4_ADDRESS_NONE on error.
        	 */
			virtual ip4_adress_t      get_netmask();

			/**
			 * @brief Get the hostname.
			 * @pre start
			 * @return The hostname, on error then return nullptr.
			 */
			virtual const char*       get_hostname();
			/**
			 * @brief Set the hostname, only work when wifi are started.
			 * @pre start
			 * @return If true then set the hostname and false on error.
			 */
			virtual bool              set_hostname(const char*  hostname);
			/**
			 * @brief Get the MAC of the WiFi AP device.
			 * @pre start
			 * @param [out] mac The MAC of the WiFi AP device.
			 * @return the MAC of the WiFi AP device, on error then return nullptr.
			 */
			virtual uint8_t*          get_mac(uint8_t* mac);
			/**
			 * @brief Get the MAC of the WiFi AP device as string.
			 * @pre start
			 * @return The MAC of the WiFi AP device as string, on error return nullptr.
			 */
			virtual const char*       get_mac();
			/**
			 * @brief Get the number of connected stations.
			 * @pre start
			 * @return The number of connected stations.
			 */
			virtual uint32_t          get_connected();

			/**
			 * @return The number of connected stations.
			 */
			operator uint32_t()     { return get_connected(); }
			/**
			 * @return The IPv4 address of the WiFi AP device.
			 */
			operator ip4_adress_t() { return get_ip(); }
			/**
			 * @return The hostname of the WiFi AP device.
			 */
			operator const char*()  { return get_hostname(); }
		protected:
			/**
			 * @brief  Get interface's IP address information.
			 * @return IP information.
			 * @note If the interface is up, IP information is read directly from the TCP/IP stack and if down
			 * then read from a copy kept in the TCP/IP adapter library itself.
			 */
			tcpip_adapter_ip_info_t get_ip_infos();

			/**
			 * @brief The basic wife event handler.
			 */
			virtual void on_wifi_event(esp_event_base_t event_base, int32_t event_id, void* event_data) override;

			/**
			 * @brief Call on WIFI_EVENT_AP_START event.
			 */
			virtual void on_start(esp_event_base_t event_base, void* event_data) { }
			/**
			 * @brief Call on WIFI_EVENT_AP_STOP event.
			 */
			virtual void on_stop(esp_event_base_t event_base, void* event_data) { }
			/**
			 * @brief Call on WIFI_EVENT_AP_STACONNECTED event.
			 */
			virtual void on_connected(esp_event_base_t event_base,void* event_data) { }
			/**
			 * @brief Call on WIFI_EVENT_AP_STADISCONNECTED event.
			 */
			virtual void on_disconnected(esp_event_base_t event_base, void* event_data) { }
			/**
			 * @brief Call on WIFI_EVENT_AP_PROBEREQRECVED event.
			 */
			virtual void on_probereqrec(esp_event_base_t event_base, void* event_data) { }

		};

		using wifi_ap = basic_wifi_ap;
	}
}


#endif // __MINILIB_NET_WIFI_AP_H__
