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
#ifndef _MINLIB_BASIC_ENDPOINT_HPP_
#define _MINLIB_BASIC_ENDPOINT_HPP_

#include "../mn_config.hpp"
#include "../mn_def.hpp"


#include "mn_basic_ip_address.hpp"
#include "mn_basic_ip4_address.hpp"
#include "mn_basic_ip6_address.hpp"
#include "mn_net_types.hpp"


namespace mn {
	namespace net {
		/**
		 * @brief This abstract basic class represents an endpoint/socket address.
		 *
		 * @note To use this, you need to subclass it. All of your endpoint should
   		 * be derived from the basic_endpoint class. Then implement the all virtual abstracted
		 * functions.
		 *
		 * @ingroup socket
		 * @author Amber-Sophia Schröck
		 */
		class basic_endpoint {
		public:
			/**
			 * @brief Get a copy from this
			 * @return The copy of this.
			 */
			virtual basic_endpoint* get_copy() = 0;
		};

		/**
		 * @brief This template basic class represents an ip endpoint/socket address.
		 * @ingroup socket
		 *
		 * @tparam TIPCLASS Type for the ip address class
		 */
		template <class TIPCLASS, address_family TAF>
		class basic_ip_endpoint : public basic_endpoint {
		public:
			using ip_type = TIPCLASS;
			using self_type = basic_ip_endpoint<TIPCLASS, TAF>;

			/**
			 * @brief Construct a basic_ip_endpoint by giving ip address and port
			 */
			basic_ip_endpoint(const ip_type& ip, const uint16_t& port) noexcept
				: m_ipAdress(ip), m_iPort(port) { }

			/**
			 * @brief  Construct a basic_ip_endpoint by copying another one.
			 */
			basic_ip_endpoint(const self_type& pOther) noexcept
				: m_ipAdress(pOther.m_ipAdress), m_iPort(pOther.m_iPort) { }

			/**
			 * @brief  Get the port number.
			 * @return The port number
			 */
			uint16_t 			get_port() { return m_iPort; }
			/**
			 * @brief Get the host IP address.
			 * @return The host IP address.
			 */
			ip_type 			get_host()   { return m_ipAdress; }
			/**
			 * @brief Get the address family of the address.
			 * @return The address family of the address.
			 */
			address_family 		get_famile()   { return TAF; }
			/**
			 * @brief Set a port number, between 0-9999
			 */
			void 				set_port(const uint16_t& port) { if(port < 9999) m_iPort = port; }
			/**
			 * @brief Set a new the host IP address.
			 * @param ip The new host IP address to set.
			 */
			void 				set_host(const ip_type& ip) { m_ipAdress = ip; }
			/**
			 * @brief Is the host address a multicast address
			 * @return If true is the host address a multicast address and false is not
			 */
			bool 				is_multicast() { return m_ipAdress.is_multicast(); }
			/**
			 * @brief Is the host address a loopaddress
			 * @return If true is the host address a loopaddress address and false is not
			 */
			bool 				is_loopback()  { return m_ipAdress.is_loopback(); }



			/**
			 * @brief Assigns another basic_ip_endpoint.
			 */
			self_type& operator = (const self_type& pOther) {
				if (this == &pOther) return *this;
				m_ipAdress = pOther.m_ipAdress; m_iPort = pOther.m_iPort; return *this;
			}
			/**
			 * @brief Is the right basic_endpoint equel with this
			 * @return If true then this basic_endpoint equel with the right basic_endpoint and false if not
			 */
			bool operator == (const self_type& pOther) {
				if(m_iPort != pOther.m_iPort) return false;
				return m_ipAdress == pOther.m_ipAdress;
			}
			/**
			 * @brief Is the right basic_endpoint not equel with this
			 * @return If true then this basic_endpoint not equel with the right basic_endpoint and false if equel
			 */
			bool operator != (const self_type& pOther) {
				return !(*this == pOther);
			}
			/**
			 * @brief smaler as .. - odering
			 */
			bool operator < (const self_type& pOther) const {
				if(m_iPort >= pOther.m_iPort) return false;
				return m_ipAdress < pOther.m_ipAdress;
			}

			virtual void swap(self_type& rhs) noexcept {
				mn::swap<uint16_t>(m_iPort, rhs.m_iPort);
				m_ipAdress.swap(rhs.m_ipAdress);
			}
		protected:
			ip_type m_ipAdress;
			uint16_t m_iPort;
		};
	}
}

#endif // _MINLIB_BASIC_ENDPOINT_HPP_
