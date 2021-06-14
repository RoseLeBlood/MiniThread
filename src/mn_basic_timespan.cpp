/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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

#include "mn_config.hpp"
#include "mn_basic_timespan.hpp"

#include <stdio.h>
#include <sys/time.h>
#include <ctime>


#include <freertos/FreeRTOS.h>

/**
 * @brief The number of microseconds in a millisecond.
 */
#define _MINILIB_TIMEDIFF_MILLISECONDS 				1000

/**
 * @brief The number of microseconds in a seconds.
 * @note (1000 * MILLISECONDS)
 */
#define _MINILIB_TIMEDIFF_SECONDS  					1000000

/**
 * @brief The number of microseconds in a minute.
 * @note (60 * SECONDS)
 */
#define _MINILIB_TIMEDIFF_MINUTES  					60000000
/**
 * @brief The number of microseconds in a hour.
 * @note (60 * MINUTES)
 */
#define _MINILIB_TIMEDIFF_HOURS    					3600000000

/**
 * @brief The number of microseconds in a day.
 * @note (24 * HOUR)
 */
#define _MINILIB_TIMEDIFF_DAYS     					86400000000


namespace mn {

	//-----------------------------------
	//  basic_timespan
	//-----------------------------------
	basic_timespan::basic_timespan()
		: m_timeSpan(0) { }

	//-----------------------------------
	//  basic_timespan
	//-----------------------------------
	basic_timespan::basic_timespan(time_type ms)
		: m_timeSpan(ms) { }

	//-----------------------------------
	//  basic_timespan
	//-----------------------------------
	basic_timespan::basic_timespan(struct timeval& val)
		: m_timeSpan( ( (int_type)val.tv_sec * _MINILIB_TIMEDIFF_SECONDS) + (int_type)val.tv_usec ) { }

	//-----------------------------------
	//  basic_timespan
	//-----------------------------------
	basic_timespan::basic_timespan(uint16_t days, uint8_t hours, uint8_t minutes,
								   uint8_t seconds, uint16_t microSeconds) : m_timeSpan(microSeconds) {

		m_timeSpan +=  seconds * _MINILIB_TIMEDIFF_SECONDS;
		m_timeSpan +=  minutes * _MINILIB_TIMEDIFF_MINUTES;
		m_timeSpan +=  hours * _MINILIB_TIMEDIFF_HOURS;
		m_timeSpan +=  days * _MINILIB_TIMEDIFF_DAYS;

	}

	basic_timespan basic_timespan::now() {
		struct timeval _now;
		::gettimeofday(&_now, NULL);

		return basic_timespan(_now);
	}

	//-----------------------------------
	//  from_ticks
	//-----------------------------------
	basic_timespan basic_timespan::from_ticks(const unsigned int& ticks) {
		return basic_timespan( ticks * portTICK_PERIOD_MS );
	}

	//-----------------------------------
	//  to_ticks
	//-----------------------------------
	basic_timespan::time_type basic_timespan::to_ticks() const {
		return m_timeSpan / portTICK_PERIOD_MS;
	}

	//-----------------------------------
	//  basic_timespan
	//-----------------------------------
	basic_timespan::basic_timespan(const self_type& other)
		: m_timeSpan(other.m_timeSpan) { }

	//-----------------------------------
	//  get_days
	//-----------------------------------
	uint16_t  basic_timespan::get_days() const {
		return uint16_t (m_timeSpan/_MINILIB_TIMEDIFF_DAYS);
	}

	//-----------------------------------
	//  get_hours
	//-----------------------------------
	uint8_t  basic_timespan::get_hours() const {
		return uint8_t ((m_timeSpan/_MINILIB_TIMEDIFF_HOURS) % 24);
	}

	//-----------------------------------
	//  get_minutes
	//-----------------------------------
	uint8_t  basic_timespan::get_minutes() const {
		return uint8_t ((m_timeSpan/_MINILIB_TIMEDIFF_MINUTES) % 60);
	}

	//-----------------------------------
	//  get_seconds
	//-----------------------------------
	uint8_t  basic_timespan::get_seconds() const {
		return uint8_t ((m_timeSpan/_MINILIB_TIMEDIFF_SECONDS) % 60);
	}

	//-----------------------------------
	//  get_milliseconds
	//-----------------------------------
	uint16_t  basic_timespan::get_milliseconds() const {
		return uint16_t ((m_timeSpan/_MINILIB_TIMEDIFF_MILLISECONDS) % 1000);
	}

	//-----------------------------------
	//  get_microseconds
	//-----------------------------------
	basic_timespan::int_type basic_timespan::get_microseconds() const {
		return basic_timespan::int_type (m_timeSpan % 1000);
	}

	//-----------------------------------
	//  get_total_hours
	//-----------------------------------
	basic_timespan::int_type  basic_timespan::get_total_hours() const {
		return basic_timespan::int_type (m_timeSpan/_MINILIB_TIMEDIFF_HOURS);
	}

	//-----------------------------------
	//  get_total_minutes
	//-----------------------------------
	basic_timespan::int_type  basic_timespan::get_total_minutes() const {
		return basic_timespan::int_type (m_timeSpan/_MINILIB_TIMEDIFF_MINUTES);
	}

	//-----------------------------------
	//  get_total_seconds
	//-----------------------------------
	basic_timespan::int_type  basic_timespan::get_total_seconds() const {
		return basic_timespan::int_type (m_timeSpan/_MINILIB_TIMEDIFF_SECONDS);
	}

	//-----------------------------------
	//  get_total_milliseconds
	//-----------------------------------
	basic_timespan::time_type basic_timespan::get_total_milliseconds() const {
		return m_timeSpan/_MINILIB_TIMEDIFF_MILLISECONDS;
	}

	//-----------------------------------
	//  operator =
	//-----------------------------------
	basic_timespan& basic_timespan::operator = (const basic_timespan& timespan) {
		m_timeSpan = timespan.m_timeSpan;
		return *this;
	}

	//-----------------------------------
	//  operator =
	//-----------------------------------
	basic_timespan& basic_timespan::operator = (time_type microSeconds) {
		m_timeSpan = microSeconds;
		return *this;
	}

	//-----------------------------------
	//  operator =
	//-----------------------------------
	basic_timespan& basic_timespan::operator = (struct timeval val) {
		return assign(val);
	}

	//-----------------------------------
	//  assign
	//-----------------------------------
	basic_timespan& basic_timespan::assign(uint16_t days, uint8_t hours, uint8_t minutes,
								   		   uint8_t seconds, uint16_t microSeconds) {
		m_timeSpan = microSeconds;
		m_timeSpan +=  seconds * _MINILIB_TIMEDIFF_SECONDS;
		m_timeSpan +=  minutes * _MINILIB_TIMEDIFF_MINUTES;
		m_timeSpan +=  hours * _MINILIB_TIMEDIFF_HOURS;
		m_timeSpan +=  days * _MINILIB_TIMEDIFF_DAYS;

		return *this;
	}


	//-----------------------------------
	//  assign
	//-----------------------------------
	basic_timespan& basic_timespan::assign(struct timeval val) {
		m_timeSpan = ( (int_type)val.tv_sec * _MINILIB_TIMEDIFF_SECONDS) + (int_type)val.tv_usec;
		return *this;
	}


}
