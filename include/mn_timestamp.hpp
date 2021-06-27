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

#include "mn_limits.hpp"
#include "mn_timespan.hpp"

#define MINILIB_TIMESTAMP_RESELUTION 	1000000

#define MINILIB_TIMESTAMP_TIME_TYPE		typename basic_timespan::time_type
#define MINILIB_TIMESTAMP_MIN			mn::numeric_limits<MINILIB_TIMESTAMP_TIME_TYPE>::min()
#define MINILIB_TIMESTAMP_MAX			mn::numeric_limits<MINILIB_TIMESTAMP_TIME_TYPE>::max()

namespace mn {
	/**
	 * @brief A Timestamp stores a monotonic* time value with microseconds resolution.
	 * @note  The internal reference time is the Unix epoch,  midnight, January 1, 1970.
	 */
	class basic_timestamp {
	public:
		using time_type = MINILIB_TIMESTAMP_TIME_TYPE;
		using self_type = basic_timestamp;

		/**
		 * @brief Consruct a basic_timestamp with the current time.
		 */
		basic_timestamp();

		/**
		 * @brief Creates a timestamp from the given time value.
		 */
		basic_timestamp(time_type tv);

		/**
		 * @brief Copy constructor.
		 */
		basic_timestamp(const self_type& other);

		void update();
		void swap(self_type& time);

		/**
		 * @brief Creates a timestamp from a mn::time_t.
		 */
		static self_type from_epoch(const mn::time_t t);

		/**
		 * @brief Creates a timestamp from a UTC time value.
		 * @note since midnight 15 October 1582.
		 */
		static self_type from_utc(const time_type val);

		mn::time_t get_epoch() const
			{ return mn::time_t(m_time / MINILIB_TIMESTAMP_RESELUTION); }

		/**
		 * @brief Get  the timestamp expressed in UTC-based time.
		 */
		time_type get_utc() const
			{ return m_time * 10 + (time_type(0x01b21dd2) << 32) + 0x13814000; }

		/**
		 * @brief Get  the timestamp expressed in microseconds.
		 * @note Unix epoch, midnight, January 1, 1970.
		 */
		time_type get_microseconds() const
			{ return m_time; }

		/**
		 * @brief Get the time elapsed since the time denoted by the timestamp.
		 * @note Equivalent to Timestamp() - *this.
		 */
		time_type get_elapsed() const
			{ self_type now; return now - *this; }


		/**
		 * @brief if the given interval has passed since
		 * the time denoted by the timestamp.
		 */
		bool is_elapsed(time_type interval) const
			{ return get_elapsed() >= interval; }

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

		time_type operator -  (const self_type& ts) const
			{ return m_time - ts.m_time; }

		time_type operator +  (const self_type& ts) const
			{ return m_time + ts.m_time; }

		self_type  operator +  (time_type d) const
			{ return self_type(m_time + d); }

		self_type  operator -  (time_type d) const
			{ return self_type(m_time - d); }

		self_type& operator += (time_type d)
			{ m_time += d; return *this; }

		self_type& operator -= (time_type d)
			{ m_time -= d; return *this; }

		self_type  operator +  (const basic_timespan& span) const {
			return self_type(m_time + span.get_total_microseconds());
		}
		self_type  operator -  (const basic_timespan& span) const {
			return self_type(m_time - span.get_total_microseconds());
		}
		self_type& operator += (const basic_timespan& span) {
			m_time += span.get_total_microseconds(); return *this;
		}
		self_type& operator -= (const basic_timespan& span) {
			m_time -= span.get_total_microseconds(); return *this;
		}
		self_type& operator = (const self_type& other) {
			m_time = other.m_time; return *this;
		}
		self_type& operator = (time_type tv) {
			m_time = tv; return *this;
		}

	private:
		time_type m_time;
	};

	void swap(basic_timestamp a, basic_timestamp b) {
		a.swap(b);
	}

	using timestamp_t = basic_timestamp;
}
#endif // _MINLIB_BASIC_TIMESTAMP_H__
