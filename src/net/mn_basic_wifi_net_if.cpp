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
#include "net/mn_basic_wifi_net_if.hpp"


namespace mn {
	namespace net {
		//-----------------------------------
		//  construtor
		//-----------------------------------
		basic_wifi_net_if::basic_wifi_net_if(esp_interface_t type)
			: m_ifInterface(type) {		}

		//-----------------------------------
		//  create_default
		//-----------------------------------
		bool basic_wifi_net_if::create_default() {
			if(get_mode()  == WIFI_MODE_AP) {
				m_pNetIf = esp_netif_create_default_wifi_ap();
			} else if(get_mode()  == WIFI_MODE_STA) {
				m_pNetIf = esp_netif_create_default_wifi_sta();
			}
			return m_pNetIf != NULL;
		}

		//-----------------------------------
		//  get_ssid
		//-----------------------------------
		const char* basic_wifi_net_if::get_ssid(char ssid[32]) {
			wifi_config_t info;
			auto _tmode = get_mode();

			esp_wifi_get_config((_tmode == WIFI_MODE_AP) ? ESP_IF_WIFI_AP : ESP_IF_WIFI_STA, &info);

			auto _ssid = (_tmode == WIFI_MODE_AP) ? (reinterpret_cast<char*>(info.ap.ssid))
											: (reinterpret_cast<char*>(info.sta.ssid));
			strcpy(ssid, _ssid);

			return ssid;
		}

		//-----------------------------------
		//  get_mode
		//-----------------------------------
		wifi_mode_t basic_wifi_net_if::get_mode() {
			return m_ifInterface == ESP_IF_WIFI_AP ? WIFI_MODE_AP : WIFI_MODE_STA;
		}

		//-----------------------------------
		//  get()
		//-----------------------------------
		esp_interface_t& basic_wifi_net_if::get() {
			 return m_ifInterface;
		}

		//-----------------------------------
		//  set_power_save_mode
		//-----------------------------------
		int	basic_wifi_net_if::set_power_save_mode(const wifi_ps_type_t mode) {
			return esp_wifi_set_ps(mode);
		}

		//-----------------------------------
		//  get_power_save_mode
		//-----------------------------------
		wifi_ps_type_t basic_wifi_net_if::get_power_save_mode() {
			wifi_ps_type_t mode = WIFI_PS_MIN_MODEM;
			esp_wifi_get_ps(&mode);
			return wifi_ps_type_t(mode);
		}

		//-----------------------------------
		// set_protocol
		//-----------------------------------
		int	basic_wifi_net_if::set_protocol(const wifi_protocol& prot) {
			int _error = NO_ERROR;

			if(get_mode()  == WIFI_MODE_AP) {
				_error = esp_wifi_set_protocol(WIFI_IF_AP, static_cast<uint8_t>(prot) );
			} else if(get_mode()  == WIFI_MODE_STA) {
				_error = esp_wifi_set_protocol(WIFI_IF_STA, static_cast<uint8_t>(prot) );
			}

			MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
 			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_WIFI_NOT_INIT, ERR_MNTHREAD_NULL);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_WIFI_IF, ERR_MNTHREAD_INVALID_ARG);
			MN_ESP2MNTHREAD_ERROR_END(_error);


			return _error;
		}

		//-----------------------------------
		// get_protocol
		//-----------------------------------
		basic_wifi_net_if::wifi_protocol basic_wifi_net_if::get_protocol() {
			int _error = NO_ERROR;
			uint8_t _prot = 0;

			if(get_mode()  == WIFI_MODE_AP) {
				_error = esp_wifi_get_protocol(WIFI_IF_AP, &_prot );
			} else if(get_mode()  == WIFI_MODE_STA) {
				_error = esp_wifi_get_protocol(WIFI_IF_STA, &_prot );
			}
			if(_error == ESP_OK) {
				return wifi_protocol(_prot);
			} else {
				return wifi_protocol::none;
			}
		}

	}
}

