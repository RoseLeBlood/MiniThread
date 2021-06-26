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
#include "net/mn_wifi.hpp"
#include "mn_config.hpp"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

namespace mn {
	namespace net {

		//-----------------------------------
		// basic_wifi::basic_wifi
		//-----------------------------------
		basic_wifi::basic_wifi(const interface_t& type)
			: basic_wifi_net_if( (type == interface_t::Station ?
									ESP_IF_WIFI_STA :
									ESP_IF_WIFI_AP)
							   ) { }

		//-----------------------------------
		// basic_wifi::~basic_wifi
		//-----------------------------------
		basic_wifi::~basic_wifi() {
			stop();
		}

		//-----------------------------------
		// basic_wifi::startup
		//-----------------------------------
		int basic_wifi::startup(wifi_init_config_t* cfg, wifi_storage_t storage) {
			if(cfg == nullptr) return ERR_MNTHREAD_NULL;

			auto _tMode = get_mode();

			esp_netif_init();
			ESP_ERROR_CHECK(esp_event_loop_create_default());

			if(create_default()) {

				if(esp_wifi_init(cfg) != ESP_OK) return ERR_MNTHREAD_OUTOFMEM;

				if(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &_wifi_event_handler, this) != ESP_OK)
					return ERR_MN_WIFI_REGISTER_EVENT;

				if(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &_ip_event_handler, this)  != ESP_OK)
					return ERR_MN_WIFI_REGISTER_EVENT;

				if(esp_wifi_set_storage(storage) != ESP_OK)
					return ERR_MNTHREAD_INVALID_ARG;

				if(esp_wifi_set_mode( _tMode )  != ESP_OK)
					return ERR_MN_WIFI_SET_MODE;

			} else {
				return ERR_MN_WIFI_CREATE_NETIF;
			}
			return ERR_MN_WIFI_OK;
		}

		//-----------------------------------
		// basic_wifi::unregist
		//-----------------------------------
		int basic_wifi::unregist() {
			if(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &_wifi_event_handler) != ESP_OK)
				return ERR_MNTHREAD_UNKN;
        	if(esp_event_handler_unregister(IP_EVENT, ESP_EVENT_ANY_ID, &_ip_event_handler) != ESP_OK)
				return ERR_MNTHREAD_UNKN;

        	return ERR_MN_WIFI_OK;
		}

		//-----------------------------------
		// basic_wifi::_wifi_event_handler
		//-----------------------------------
		void basic_wifi::_wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
			if(arg != NULL) {
				basic_wifi* _ctx = static_cast<basic_wifi*>(arg);
				_ctx->on_wifi_event(event_base, event_id, event_data);
			}
		}

		//-----------------------------------
		// basic_wifi::_ip_event_handler
		//-----------------------------------
		void basic_wifi::_ip_event_handler(void* arg, esp_event_base_t event_base,
										   int32_t event_id, void* event_data) {
			if(arg != NULL) {
				basic_wifi* _ctx = static_cast<basic_wifi*>(arg);
				_ctx->on_ip_event(event_base, event_id, event_data);
			}
		}
	}
}
