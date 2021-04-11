#ifndef _MINTHREAH_BASIC_NETIF_HPP_
#define _MINTHREAH_BASIC_NETIF_HPP_

#include "mn_config.hpp"
#include "mn_lock.hpp"

#include <esp_interface.h>
#include <esp_wifi.h>
#include <esp_netif.h>
#include "sdkconfig.h"

#include "mn_basic_ip_address.hpp"

#if CONFIG_ETH_USE_SPI_ETHERNET
#include "driver/spi_master.h"
#endif // CONFIG_ETH_USE_SPI_ETHERNET

namespace mn {
	namespace net {
		class basic_net_if {
			public:
				basic_net_if();
				basic_net_if(const basic_net_if& other)
					:  m_ifConfig(other.m_ifConfig), m_pNetIf(other.m_pNetIf) { }

				virtual esp_netif_t*    create_default() = 0;

				esp_err_t    	set_mac(uint8_t mac[6]);
				esp_err_t       get_mac(uint8_t mac[6]);
				const char*     get_hostname();
				bool            set_hostname(const char* hostname);
				bool 			is_up();
				bool 			is_down();

				esp_err_t		get_impl_name(char name[6]);
				virtual ip4_address     get_ip() = 0;
				virtual ip4_address     get_broadcast() = 0;
				virtual ip4_address     get_gateway() = 0;
				virtual ip4_address     get_netmask() = 0;

				virtual esp_netif_t*    get_netif() { return m_pNetIf; }


				basic_net_if& operator = (const basic_net_if& other);
			protected:
				esp_netif_config_t* m_ifConfig;
				esp_netif_t*    	m_pNetIf;
		};
#if 0
		class basic_ethernet_nef_if : public basic_net_if {
		protected:
			ethernet_interface() : basic_net_if() { }

			virtual esp_netif_t* create_default();
		};
#endif // 0

		class basic_wifi_net_if : public basic_net_if {
		protected:
			basic_wifi_net_if(esp_interface_t type);

			virtual esp_netif_t*    create_default();

			virtual wifi_mode_t     get_mode();
			virtual const char*     get_ssid(char ssid[32]);

			esp_interface_t&        get()       { return m_ifInterface; }

			const char*     get_hostname();
			bool            set_hostname(const char* hostname);
		protected:
			esp_interface_t 	m_ifInterface;
		};

		class basic_ppp_net_if : public basic_net_if {
		protected:
			basic_ppp_net_if() : basic_net_if() { }

			virtual esp_netif_t*    create_default();
		};

		/*class basic_slip_net_if : public basic_net_if {
		protected:
			basic_slip_net_if() : basic_net_if() { }

			virtual esp_netif_t*    create_default();
		};*/
	}
}

#endif // _MINTHREAH_BASIC_NETIF_HPP_
