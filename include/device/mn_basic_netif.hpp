#ifndef _MINTHREAH_BASIC_NETIF_HPP_
#define _MINTHREAH_BASIC_NETIF_HPP_

#include "mn_config.hpp"


#include <esp_interface.h>
#include <esp_netif.h>
#include <esp_wifi.h>

#if CONFIG_ETH_USE_SPI_ETHERNET
#include <driver/spi_master.h>
#endif // CONFIG_ETH_USE_SPI_ETHERNET

#include <net/mn_basic_ip_address.hpp>
#include <net/mn_basic_ip4_address.hpp>
#include <net/mn_basic_ip6_address.hpp>

#include "mn_lock.hpp"

namespace mn {
	namespace device {

		/**
		 * @brief Wrapper class around esp-idf implementation of a esp-netif device.
		 *
		 * @note To use this, you need to subclass it. All of your task should
   		 * be derived from the basic_net_if class. Then implement the virtual
		 * create_default function.
		 *
		 * @ingroup device
		 */
		class basic_net_if {
			public:
				using ip4_address_t = net::basic_ip4_address;

				struct ip_info {
					ip4_address_t ip;      /**< Interface IPV4 address */
					ip4_address_t netmask; /**< Interface IPV4 netmask */
					ip4_address_t gw;      /**< Interface IPV4 gateway address */
				};
				using ip_info_t = ip_info;

				/**
				 * constructor
				 */
				basic_net_if();
				/**
				 * copy constructor
				 */
				basic_net_if(const basic_net_if& other)
					:  m_ifConfig(other.m_ifConfig), m_pNetIf(other.m_pNetIf) { }
				basic_net_if& operator = (const basic_net_if& other);

				/**
				 * @brief   Creates an instance of new esp-netif object
				 * @note    Must be overwrite, with the driver spcezific version and set m_pNetIf
				 *
				 * @return
				 *         - true: on success
				 *         - false: if initializing failed
				 */
				virtual bool    create_default() = 0;

				/**
				 * @brief Initialize the underlying TCP/IP stack
				 *
				 * @return
				 *         - true: on success
				 *         - false: if initializing failed

				 * @note This function should be called exactly once from application code, when the application starts up.
				 */
				static bool start();

				/**
				 * @brief  	Deinitialize the esp-netif component (and the underlying TCP/IP stack)
				 *
				 * @note 	Deinitialization is not supported yet
				 *
				 * @return
				 *         - true: on success
				 *         - false: if initializing failed
				 */
				static bool stop();
				/**
				 * @brief Set the mac address for the interface instance

				 * @param[in]  mac Desired mac address for the related network interface
				 * @return
				 *         - NO_ERROR: succeed
				 *         - ERR_MNTHREAD_NETIF_NOT_INIT - interface status error
				 *         - ERR_MNTHREAD_NOT_SUPPORTED - mac not supported on this interface
				 */
				int    			set_mac(uint8_t mac[6]);

				/**
				 * @brief Get the mac address for the interface instance

				 * @param[out]  mac Resultant mac address for the related network interface
				 * @return
				 *         - NO_ERROR: succeed
				 *         - ERR_MNTHREAD_NETIF_NOT_INIT - interface status error
				 *         - ERR_MNTHREAD_NOT_SUPPORTED - mac not supported on this interface
				 */
				esp_err_t       get_mac(uint8_t mac[6]);

				/**
				 * @brief Get the hostname of an interface
				 *
				 * @return Returns the hostname of an interface
				 */
				const char*     get_hostname();

				/**
				 * @brief Set the hostname of an interface
				 *
				 * The configured hostname overrides the default configuration value CONFIG_LWIP_LOCAL_HOSTNAME.
				 * Please note that when the hostname is altered after interface started/connected the changes
				 * would only be reflected once the interface restarts/reconnects
				 *
				 * @param hostname New hostname for the interface. Maximum length 32 bytes.
				 * @return
				 *		- true The hostname was changed
				 *      - false The hostname was not changed
				 */
				bool            set_hostname(const char* hostname);

				/**
				 * @brief Test if supplied interface is up
				 *
				 * @return - true - Interface is up
 				 *         - false - Interface is down
				 */
				bool 			is_up();

				/**
				 * @brief Get net interface name from network stack implementation
				 *
				 * @note This name could be used in `setsockopt()` to bind socket with appropriate interface
				 *
				 * @param[out]  name Interface name as specified in underlying TCP/IP stack. Note that the
				 * actual name will be copied to the specified buffer, which must be allocated to hold
				 * maximum interface name size (6 characters for lwIP)
				 *
				 * @return
				 *		- NO_ERROR: succeed
				 *		- ERR_MNTHREAD_INVALID_ARG: The given arcuments are wrong
				 */
				int						get_impl_name(char name[6]);
				/**
				 * @brief  Get net interface index from network stack implementation
				 *
				 * @note This index could be used in `setsockopt()` to bind socket with multicast interface
				 *
				 *
				 * @return implementation specific index of interface represented with supplied esp_netif
				 */
				int 					get_impl_index();

				/**
				 * @brief  Get the Handle to esp-netif instance.
				 * @return The Handle to esp-netif instance.
				 */
				virtual esp_netif_t*    	get_netif() { return m_pNetIf; }

				/**
				 * @brief  Get the config of the esp-netif instance.
				 * @return The config of the esp-netif instance.
				 */
				virtual esp_netif_config_t* get_config() { return m_ifConfig; }

				/**
				 * @brief  Get interface's IP address information.
				 *
				 * If the interface is up, IP information is read directly from the TCP/IP stack.
				 * If the interface is down, IP information is read from a copy kept in the ESP-NETIF instance
				 *
				 * @param  ip_info If successful, IP information will be returned in this argument.
				 *
				 * @return
          	     *		- NO_ERROR: succeed
				 *		- ERR_MNTHREAD_INVALID_ARG: The given arcuments are wrong
				 *		- ERR_MNTHREAD_UNKN: Unbekannter fehler
				 */
				int 						get_ip_infos(ip_info_t& ip_info);

				/**
				 * @brief  Set interface's IP address information
				 *
				 * This function is mainly used to set a static IP on an interface.
				 *
				 * If the interface is up, the new IP information is set directly in the TCP/IP stack.
				 *
				 * The copy of IP information kept in the ESP-NETIF instance is also updated (this
				 * copy is returned if the IP is queried while the interface is still down.)
				 *
				 * @note DHCP client/server must be stopped (if enabled for this interface) before setting new IP information.
				 * @note Calling this interface for may generate a SYSTEM_EVENT_STA_GOT_IP or SYSTEM_EVENT_ETH_GOT_IP event.
				 *
				 * @param[in]  esp_netif Handle to esp-netif instance
				 * @param[in] ip_info IP information to set on the specified interface
				 *
				 * @return
				 *      - NO_ERROR: succeed
				 *		- ERR_MNTHREAD_INVALID_ARG: The given arcuments are wrong
				 *		- ERR_MNTHREAD_UNKN: If DHCP server or client is still running
				 */
				int 						set_ip_infos(const ip_info_t& ip_info);

				operator bool() 		 { return is_up(); }
				operator esp_netif_t* () { return m_pNetIf;}

				bool operator == (const basic_net_if& other);
				bool operator != (const basic_net_if& other);

				bool operator == (const esp_netif_t* other);
				bool operator != (const esp_netif_t* other);
			protected:
				/**
				 * A saved / cached copy of esp-netif config
				 */
				esp_netif_config_t* m_ifConfig;
				/**
				 *  Reference to the underlying esp-netif handle.
				 */
				esp_netif_t*    	m_pNetIf;
		};
	}
}

#endif // _MINTHREAH_BASIC_NETIF_HPP_
