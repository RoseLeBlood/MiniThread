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
#ifndef __MINILIB_BASIC_WIFI_STA_NET_IF_H__
#define __MINILIB_BASIC_WIFI_STA_NET_IF_H__

#include "../mn_config.hpp"
#include "mn_basic_wifi_net_if.hpp"

namespace mn {
	namespace net {


		class basic_wifi_sta_net_if : public basic_wifi_net_if
		{
			public:
				basic_wifi_sta_net_if();
				virtual ~basic_wifi_sta_net_if();

			protected:

			private:
		};
	}
}

#endif // __MINILIB_BASIC_WIFI_STA_NET_IF_H__
