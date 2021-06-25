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

#ifndef _MINLIB_BASIC_TIMESTAMP_H__
#define _MINLIB_BASIC_TIMESTAMP_H__

#include "mn_config.hpp"
#include "mn_basic_timespan.hpp"

#define MINILIB_TIMESTAMP_RESELUTION 	1000000

namespace mn {
	/**
	 * @brief A Timestamp stores a monotonic* time value with microseconds resolution.
	 * @note  The internal reference time is the Unix epoch,  midnight, January 1, 1970.
	 */
	class basic_timestamp {
	public:
		using value_type = int64_t;
		using self_type = basic_timestamp;
		using timepan_type = basic_timespan;

		/**
		 * @brief Consruct a basic_timestamp with the current time.
		 */
		basic_timestamp();

		/**
		 * @brief Creates a timestamp from the given time value.
		 */
		basic_timestamp(value_type tv);

		/**
		 * @brief Copy constructor.
		 */
		basic_timestamp(const self_type& other);

		/**
		 * @brief Destroys the timestamp
		 */
		~basic_timestamp();



		void update();

		mn::time_t get_epoch() const
			{ return mn::time_t(m_time / MINILIB_TIMESTAMP_RESELUTION)); }

		/**
		 * @brief Get  the timestamp expressed in UTC-based time.
		 */
		value_type get_utc() const
			{ return m_time * 10 + (value_type(0x01b21dd2) << 32) + 0x13814000; }

		/**
		 * @brief Get  the timestamp expressed in microseconds.
		 * @note Unix epoch, midnight, January 1, 1970.
		 */
		value_type get_microseconds() const
			{ return m_time; }

		/**
		 * @brief Get the time elapsed since the time denoted by the timestamp.
		 * @note Equivalent to Timestamp() - *this.
		 */
		value_type get_elapsed() const
			{ Timestamp now; return now - *this; }


		/**
		 * @brief if the given interval has passed since
		 * the time denoted by the timestamp.
		 */
		bool is_elapsed(value_type interval) const
			{ return elapsed() >= interval; }

		void swap(self_type& ts) {
			mn::swap(m_time, ts.m_time);
		}

		static self_type from_epoch(mn::time_t t);
			/// Creates a timestamp from a mn::time_t.

		static self_type from_utc(UtcTimeVal val);
			/// Creates a timestamp from a UTC time value
			/// (100 nanosecond intervals since midnight,
			/// October 15, 1582).


		bool operator == (const self_type& ts) const
			{ return m_time == ts.m_time; }

		bool operator != (const self_type& ts) const
			{ return m_time != ts.m_time; }

		bool operator >= (const self_type& ts) const
			{ return m_time >= ts.m_time; }

		bool operator <= (const self_type& ts) const
			{ return m_time <= ts.m_time; }

		bool operator <  (const self_type& ts) const
			{ return m_time < ts.m_time; }

		bool operator >  (const self_type& ts) const
			{ return m_time > ts.m_time; }

		value_type operator -  (const self_type& ts) const
			{ return m_time - ts.m_time; }

		value_type operator +  (const self_type& ts) const
			{ return m_time + ts.m_time; }

		self_type  operator +  (value_type d) const
			{ return self_type(m_time + d); }

		self_type  operator -  (value_type d) const
			{ return self_type(m_time - d); }

		self_type& operator += (value_type d)
			{ m_time += d; return *this; }

		self_type& operator -= (value_type d)
			 m_time = d; return *this; }

		self_type  operator +  (const timepan_type& span) const;
		self_type  operator -  (const timepan_type& span) const;
		self_type& operator += (const timepan_type& span);
		self_type& operator -= (const timepan_type& span);


		self_type& operator = (const self_type& other);
		self_type& operator = (value_type tv);
	private:
		value_type m_time;
	};

	using timestamp_t = basic_timestamp;
}
#endif // _MINLIB_BASIC_TIMESTAMP_H__
