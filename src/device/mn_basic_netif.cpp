#include "mn_config.hpp"
#include "device/mn_basic_netif.hpp"

#include <esp_eth.h>
#include <esp_wifi.h>
#include <esp_system.h>
#include <esp_event.h>
#include <esp_log.h>

//#include <esp_netif_ppp.h>

#include <cstring>



namespace mn {
	namespace device {
		//-----------------------------------
		//  create_default
		//-----------------------------------
		basic_net_if::basic_net_if() {
			//esp_netif_init();
		}

		//-----------------------------------
		//  is_up
		//-----------------------------------
		bool basic_net_if::is_up() {
			return esp_netif_is_netif_up(m_pNetIf);
		}

		//-----------------------------------
		//  get_impl_name
		//-----------------------------------
		int basic_net_if::get_impl_name(char name[6]) {
			return esp_netif_get_netif_impl_name(m_pNetIf, name) == ESP_ERR_ESP_NETIF_INVALID_PARAMS ?
				ERR_MNTHREAD_INVALID_ARG : NO_ERROR;
		}

		//-----------------------------------
		//  get_impl_index
		//-----------------------------------
		int basic_net_if::get_impl_index() {
			return esp_netif_get_netif_impl_index(m_pNetIf);
		}

		//-----------------------------------
		//  set_mac
		//-----------------------------------
		int basic_net_if::set_mac(uint8_t mac[6]) {
			int _error = esp_netif_set_mac(m_pNetIf, mac);

			MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_ESP_NETIF_IF_NOT_READY, ERR_MNTHREAD_NETIF_NOT_INIT);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_NOT_SUPPORTED, ERR_MNTHREAD_NOT_SUPPORTED);
			MN_ESP2MNTHREAD_ERROR_END(_error);

			return _error;
		}

		//-----------------------------------
		//  get_mac
		//-----------------------------------
		int basic_net_if::get_mac(uint8_t mac[6]) {
			int _error = esp_netif_get_mac(m_pNetIf, mac);

			MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_ESP_NETIF_IF_NOT_READY, ERR_MNTHREAD_NETIF_NOT_INIT);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_NOT_SUPPORTED, ERR_MNTHREAD_NOT_SUPPORTED);
			MN_ESP2MNTHREAD_ERROR_END(_error);

			return _error;
		}

		//-----------------------------------
		//  get_hostname
		//-----------------------------------
		const char* basic_net_if::get_hostname() {
			const char* _hostname = "";

			if(esp_netif_get_hostname(m_pNetIf, &_hostname) != ESP_OK) return "";
			return _hostname;
		}

		//-----------------------------------
		//  set_hostname
		//-----------------------------------
		bool basic_net_if::set_hostname(const char* hostname) {
			return esp_netif_set_hostname(m_pNetIf, hostname) == ESP_OK;
		}

		//-----------------------------------
		//  get_ip_infos
		//-----------------------------------
		int basic_net_if::get_ip_infos(ip_info_t& ip_info) {
			esp_netif_ip_info_t _info_t;

			int _error = esp_netif_get_ip_info(m_pNetIf, &_info_t);

			MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_ESP_NETIF_INVALID_PARAMS, ERR_MNTHREAD_INVALID_ARG);
			MN_ESP2MNTHREAD_ERROR_END(_error);

			if(_error == NO_ERROR) {
				ip_info.gw = ip4_address_t(_info_t.gw.addr);
				ip_info.ip = ip4_address_t(_info_t.ip.addr);
				ip_info.netmask = ip4_address_t(_info_t.netmask.addr);
			}

			return _error;
		}

		//-----------------------------------
		//  set_ip_infos
		//-----------------------------------
		int basic_net_if::set_ip_infos(const ip_info_t& ip_info) {
			esp_netif_ip_info_t _info_t;

			_info_t.ip.addr 	 = (uint32_t)ip_info.ip;
			_info_t.gw.addr 	 = (uint32_t)ip_info.gw;
			_info_t.netmask.addr = (uint32_t)ip_info.netmask;

			int _error = esp_netif_get_ip_info(m_pNetIf, &_info_t);

			MN_ESP2MNTHREAD_ERROR_BEGIN(_error)
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_OK, NO_ERROR);
			MN_ESP2MNTHREAD_ERROR_VAL(_error, ESP_ERR_ESP_NETIF_INVALID_PARAMS, ERR_MNTHREAD_INVALID_ARG);
			MN_ESP2MNTHREAD_ERROR_END(_error);

			return _error;
		}

		//-----------------------------------
		//  start
		//-----------------------------------
		bool basic_net_if::start() {
			return esp_netif_init() == ESP_OK;
		}

		//-----------------------------------
		//  stop
		//-----------------------------------
		bool basic_net_if::stop() {
			return esp_netif_deinit() == ESP_OK;
		}

		//-----------------------------------
		//  operator ==
		//-----------------------------------
		bool basic_net_if::operator == (const basic_net_if& other) {
			if(m_ifConfig != other.m_ifConfig) return false;
			return m_pNetIf == other.m_pNetIf;
		}

		//-----------------------------------
		//  operator !=
		//-----------------------------------
		bool basic_net_if::operator != (const basic_net_if& other) {
			if(m_ifConfig == other.m_ifConfig) return false;
			return m_pNetIf != other.m_pNetIf;
		}

		//-----------------------------------
		//  operator ==
		//-----------------------------------
		bool basic_net_if::operator == (const esp_netif_t* other) {
			return m_pNetIf == other;
		}

		//-----------------------------------
		//  operator !=
		//-----------------------------------
		bool basic_net_if::operator != (const esp_netif_t* other) {
			return m_pNetIf != other;
		}

		//-----------------------------------
		//  operator =
		//-----------------------------------
		basic_net_if& basic_net_if::operator = (const basic_net_if& other) {
			m_ifConfig = other.m_ifConfig;
			m_pNetIf = other.m_pNetIf;

			return *this;
		}


#if 0
		bool basic_slip_net_ppp::create_default() {
			esp_netif_config_t cfg = ESP_NETIF_BASE_DEFAULT_PPP();
			m_pNetIf = esp_netif_new(&cfg);

			return m_pNetIf != NULL;
		}
#endif // CONFIG_LWIP_PPP_SUPPORT
	}
}
