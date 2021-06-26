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

#ifndef __MINILIB_NET_NETWORK_H__
#define __MINILIB_NET_NETWORK_H__

#include "mn_wifi.hpp"
#include "mn_wifi_ap.hpp"
//#include "mn_wifi_sta.hpp"
//#include "mn_wifi_multi.hpp"

//#include "mn_eth.hpp"
//#include "mn_serial_net.hpp"

namespace mn {
	namespace net {

		using wifi = basic_wifi;
		using wifi_ap = basic_wifi_ap;
		using wifi_sta = void;
		using wifi_multi = void;


		using eth = void;
		using serial_net = void;
	}
}
#endif // __MINILIB_NET_NETWORK_H__
