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
#include "device/mn_wifi_ap.hpp"
#include "mn_config.hpp"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <esp_err.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <lwip/ip_addr.h>
#include <esp_event.h>
#include <esp_log.h>

#include "mn_algorithm.hpp"

#define ESP2ERR mn::device::__esp32error_to_mnerror

namespace mn {
	namespace device {

		//-----------------------------------
        //  __esp32error_to_mnerror
        //-----------------------------------
		inline int __esp32error_to_mnerror(int err) {
			switch(err) {
			case ESP_ERR_WIFI_NOT_INIT: return ERR_MN_WIFI_NOT_INIT;
			case ESP_ERR_INVALID_ARG: 	return ERR_MNTHREAD_INVALID_ARG;
			case ESP_ERR_WIFI_IF: 		return ERR_MN_WIFI_IF;
			case ESP_ERR_WIFI_MODE: 	return ERR_MN_WIFI_MODE;
			case ESP_ERR_WIFI_PASSWORD: return ERR_MN_WIFI_PASSWORD;
			case ESP_ERR_WIFI_NVS: 		return ERR_MN_WIFI_NVS;
			case ESP_ERR_NO_MEM: 		return ERR_MNTHREAD_OUTOFMEM;
			case ESP_ERR_WIFI_CONN: 	return ERR_MN_WIFI_CONN;
			};
			return ERR_MNTHREAD_UNKN;
		}



		//-----------------------------------
        //  basic_wifi_ap::start
        //-----------------------------------
		int basic_wifi_ap::start(const char* ssid, auth_mode_t mode, const char* passwd, bool storeCfgToFlash,
					   bool ipv6, int channel, bool ssid_hidden, int max_connection) {

			int err = 0;
			wifi_config_t wifi_config;

			size_t ssid_len = mn::min(strlen(ssid), ::size_t(32));
			size_t passwd_len = mn::min(strlen(passwd), ::size_t(64));

			if( ssid_len < 8) return ERR_MN_WIFI_SSID;
			if( mode != auth_mode_t::Open && passwd_len < 8) return ERR_MN_WIFI_PASSWORD;

			wifi_init_config_t _cfg = WIFI_INIT_CONFIG_DEFAULT();
			basic_wifi::startup(&_cfg, storeCfgToFlash ? WIFI_STORAGE_FLASH : WIFI_STORAGE_RAM);

			wifi_config.ap.authmode = (wifi_auth_mode_t)mode;
			memcpy(wifi_config.ap.ssid, ssid, ssid_len);
			wifi_config.ap.ssid_len = ssid_len;

			if(wifi_config.ap.authmode != WIFI_AUTH_OPEN) {
				memcpy(wifi_config.ap.password, passwd, passwd_len);
			}

			wifi_config.ap.ssid_hidden = !ssid_hidden;
			wifi_config.ap.channel = channel;
			wifi_config.ap.max_connection = max_connection;
			wifi_config.ap.beacon_interval = 100;

			ESP_LOGI("metronome::wifiAp", "Setting WiFi configuration SSID %s...", wifi_config.sta.ssid);

			if ( (err = esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config)) != ESP_OK)
				return ESP2ERR(err);

			if ( (err = esp_wifi_start()) != ESP_OK)
				return ESP2ERR(err);

			if(ipv6) {
				if ( (err = tcpip_adapter_create_ip6_linklocal(TCPIP_ADAPTER_IF_AP)) != ESP_OK)
					return ESP2ERR(err);

				ESP_LOGI("metronome::wifiAp", "IPv6 enabled");
			}

			return ERR_MN_WIFI_OK;
		}

		//-----------------------------------
        //  basic_wifi_ap::set_ipinfo
        //-----------------------------------
		int basic_wifi_ap::set_ipinfo(const ip4_adress_t& local_ip, const ip4_adress_t& gateway,
										const ip4_adress_t& subnet) {
			tcpip_adapter_ip_info_t info;
			info.ip.addr 		= (uint32_t)local_ip;
			info.gw.addr 		= (uint32_t)gateway;
			info.netmask.addr 	= (uint32_t)subnet;
			tcpip_adapter_dhcps_stop(TCPIP_ADAPTER_IF_AP);

			if(tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_AP, &info) == ESP_OK) {
				dhcps_lease_t lease;
				lease.enable = true;
				lease.start_ip.addr = ((uint32_t)local_ip) + (1 << 24);
				lease.end_ip.addr = ((uint32_t)local_ip) + (11 << 24);

				tcpip_adapter_dhcps_option(
					TCPIP_ADAPTER_OP_SET,
					ESP_NETIF_REQUESTED_IP_ADDRESS,
					(void*)&lease, sizeof(dhcps_lease_t)
				);

				return tcpip_adapter_dhcps_start(TCPIP_ADAPTER_IF_AP) == ESP_OK ? 0 : 1;
			}
			return -1;
		}

		//-----------------------------------
        //  basic_wifi_ap::stop
        //-----------------------------------
		bool basic_wifi_ap::stop(bool wifioff) {
			if(get_mode() == WIFI_MODE_NULL) return false;

			bool ret;

			wifi_config_t wifi_config;
			*wifi_config.ap.ssid = 0;
			*wifi_config.ap.password = 0;

			wifi_config.ap.ssid_len = 0;
			wifi_config.ap.authmode = WIFI_AUTH_OPEN;

			ret = esp_wifi_set_config(WIFI_IF_AP, &wifi_config) == ESP_OK;
			if(wifioff) esp_wifi_stop();

			base_type::unregist();
			return ret;
		}

		//-----------------------------------
        //  basic_wifi_ap::get_hostname
        //-----------------------------------
		const char * basic_wifi_ap::get_hostname() {
			if(get_mode() == WIFI_MODE_NULL) return nullptr;
			const char * hostname = nullptr;

			tcpip_adapter_get_hostname(TCPIP_ADAPTER_IF_AP, &hostname);
			return hostname;
		}

		//-----------------------------------
        //  basic_wifi_ap::set_hostname
        //-----------------------------------
		bool basic_wifi_ap::set_hostname(const char* hostname) {
			if(get_mode() == WIFI_MODE_NULL) return false;

			return tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_AP, hostname ) == ESP_OK;
		}

		//-----------------------------------
        //  basic_wifi_ap::get_mac
        //-----------------------------------
		uint8_t* basic_wifi_ap::get_mac(uint8_t* mac) {
			if(get_mode() == WIFI_MODE_NULL) return nullptr;

			esp_wifi_get_mac(WIFI_IF_AP, mac);
			return mac;
		}

		//-----------------------------------
        //  basic_wifi_ap::get_connected
        //-----------------------------------
		uint32_t basic_wifi_ap::get_connected() {
			if(get_mode() == WIFI_MODE_NULL) return 0;
			wifi_sta_list_t clients;

			if(esp_wifi_ap_get_sta_list(&clients) == ESP_OK) {
				 return clients.num;
			}
			return 0;
		}

		//-----------------------------------
        //  basic_wifi_ap::get_ip_infos
        //-----------------------------------
		tcpip_adapter_ip_info_t basic_wifi_ap::get_ip_infos() {
			tcpip_adapter_ip_info_t ip;
			tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_AP, &ip);

			return ip;
		}

		//-----------------------------------
        //  basic_wifi_ap::get_ip
        //-----------------------------------
		typename basic_wifi_ap::ip4_adress_t basic_wifi_ap::get_ip() {
			if(get_mode() == WIFI_MODE_NULL) return ip4_adress_t();
			tcpip_adapter_ip_info_t ip = get_ip_infos();

			return ip4_adress_t(ip.ip.addr);
		}

		//-----------------------------------
        //  basic_wifi_ap::get_broadcast
        //-----------------------------------
		typename basic_wifi_ap::ip4_adress_t basic_wifi_ap::get_broadcast() {
			if(get_mode() == WIFI_MODE_NULL) return ip4_adress_t();

			return get_gateway().calc_broadcast( get_netmask() );
		}
		typename basic_wifi_ap::ip4_adress_t basic_wifi_ap::get_gateway() {
			if(get_mode() == WIFI_MODE_NULL) return ip4_adress_t();
			tcpip_adapter_ip_info_t ip = get_ip_infos();

			return ip4_adress_t(ip.gw.addr);
		}

		//-----------------------------------
        //  basic_wifi_ap::get_netmask
        //-----------------------------------
		typename basic_wifi_ap::ip4_adress_t basic_wifi_ap::get_netmask() {
			if(get_mode() == WIFI_MODE_NULL) return ip4_adress_t();
			tcpip_adapter_ip_info_t ip = get_ip_infos();

			return ip4_adress_t(ip.netmask.addr);
		}

		//-----------------------------------
        //  basic_wifi_ap::get_mac
        //-----------------------------------
		const char* basic_wifi_ap::get_mac() {
			if(get_mode() == WIFI_MODE_NULL) return nullptr;

			uint8_t mac[6];
			char macStr[18] = { 0 };
			esp_wifi_get_mac(WIFI_IF_AP, mac);

			sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

			return std::string(macStr).c_str();
		}

		//-----------------------------------
        //  basic_wifi_ap::on_wifi_event
        //-----------------------------------
		void basic_wifi_ap::on_wifi_event(esp_event_base_t event_base, int32_t event_id, void* event_data) {
			switch(event_id) {
			case WIFI_EVENT_AP_STACONNECTED: on_connected(event_base, event_data); break;
			case WIFI_EVENT_AP_STADISCONNECTED: on_disconnected(event_base,  event_data); break;
			case WIFI_EVENT_AP_START: on_start(event_base, event_data); break;
			case WIFI_EVENT_AP_STOP: on_stop(event_base, event_data); break;
			case WIFI_EVENT_AP_PROBEREQRECVED: on_probereqrec(event_base, event_data); break;
			default: break;
			}
		}
	}
}

