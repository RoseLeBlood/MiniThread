#include "net/mn_basic_netif.hpp"

#include <esp_eth.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_log.h>

#include <esp_netif_ppp.h>
#include <cstring>



namespace mn {
	namespace net {
		basic_net_if::basic_net_if() {
			//esp_netif_init();
		}
		basic_net_if& basic_net_if::operator = (const basic_net_if& other) {
			m_ifConfig = other.m_ifConfig;
			m_pNetIf = other.m_pNetIf;

			return *this;
		}
		bool basic_net_if::is_up() {
			return esp_netif_is_netif_up(m_pNetIf);
		}
		bool basic_net_if::is_down() {
			return !( is_up() );
		}
		esp_err_t basic_net_if::get_impl_name(char name[6]) {
			return esp_netif_get_netif_impl_name(m_pNetIf, name);
		}
		esp_err_t basic_net_if::set_mac(uint8_t mac[6]) {
			return esp_netif_set_mac(m_pNetIf, mac);
		}
		esp_err_t basic_net_if::get_mac(uint8_t mac[6]) {
			return esp_netif_get_mac(m_pNetIf, mac);
		}
		const char* basic_net_if::get_hostname() {
			const char* _hostname = "";

			if(esp_netif_get_hostname(m_pNetIf, &_hostname) != ESP_OK) return "";
			return _hostname;
		}
		bool basic_net_if::set_hostname(const char* hostname) {
			return esp_netif_set_hostname(m_pNetIf, hostname) == ESP_OK;
		}

		///----------------------------WIFI
		basic_wifi_net_if::basic_wifi_net_if(esp_interface_t type)
			: m_ifInterface(type) {		}

		bool basic_wifi_net_if::create_default() {
			if(get_mode()  == WIFI_MODE_AP) {
				m_pNetIf = esp_netif_create_default_wifi_ap();
			} else if(get_mode()  == WIFI_MODE_STA) {
				m_pNetIf = esp_netif_create_default_wifi_sta();
			}
			return m_pNetIf != NULL;
		}
		const char* basic_wifi_net_if::get_ssid(char ssid[32]) {
			wifi_config_t info;
			auto _tmode = get_mode();

			esp_wifi_get_config((_tmode == WIFI_MODE_AP) ? ESP_IF_WIFI_AP : ESP_IF_WIFI_STA, &info);

			auto _ssid = (_tmode == WIFI_MODE_AP) ? (reinterpret_cast<char*>(info.ap.ssid))
											: (reinterpret_cast<char*>(info.sta.ssid));
			strcpy(ssid, _ssid);

			return ssid;
		}

		wifi_mode_t basic_wifi_net_if::get_mode() {
			return m_ifInterface == ESP_IF_WIFI_AP ? WIFI_MODE_AP : WIFI_MODE_STA;
		}


		///----------------------------ETH
#if 0
		esp_netif_t* basic_ethernet_nef_if::create_default() {

			return nullptr;
		}
#endif // CONFIG_ETH_ENABLED

#if CONFIG_LWIP_SLIP_SUPPORT
		bool basic_slip_net_if::create_default() {
			esp_netif_config_t cfg = ESP_NETIF_BASE_DEFAULT_SLIP();
			m_pNetIf = esp_netif_new(&cfg);

			return m_pNetIf != NULL;
		}
#endif // CONFIG_LWIP_SLIP_SUPPORT

#ifdef CONFIG_LWIP_PPP_SUPPORT
		bool basic_slip_net_ppp::create_default() {
			esp_netif_config_t cfg = ESP_NETIF_BASE_DEFAULT_PPP();
			m_pNetIf = esp_netif_new(&cfg);

			return m_pNetIf != NULL;
		}
#endif // CONFIG_LWIP_PPP_SUPPORT
	}
}
