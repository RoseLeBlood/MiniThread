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
#include "mn_timestamp.hpp"

#include "mn_config.hpp"


#include "mn_def.hpp"


namespace mn {
	basic_timestamp::basic_timestamp() {
		update();
	}

	basic_timestamp::basic_timestamp(time_type tv)
		: m_time(tv) { }

	basic_timestamp::basic_timestamp(const self_type& other)
		: m_time(other.m_time)  { }

	void basic_timestamp::update() {
		struct timeval tv;
		mn::gettimeofday(&tv, NULL);

		m_time = time_type(tv.tv_sec) * MINILIB_TIMESTAMP_RESELUTION + tv.tv_usec;
	}
	void basic_timestamp::swap(self_type& time) {
		mn::swap(m_time, time.m_time);
	}

	basic_timestamp::self_type basic_timestamp::from_epoch(const mn::time_t t) {
		time_type _time = time_type(t);
		return self_type( _time * MINILIB_TIMESTAMP_RESELUTION );
	}

	basic_timestamp::self_type basic_timestamp::from_utc(const time_type val) {
		time_type _time = (val - ( time_type(0x01b21dd2) << 32 ) + 0x13814000);
		return self_type( (_time / 10) );
	}
}
