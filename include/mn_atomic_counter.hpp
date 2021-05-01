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
#ifndef __MINLIB_ATOMIC_COUNTER_H__
#define __MINLIB_ATOMIC_COUNTER_H__

#include "mn_config.hpp"
#include "mn_atomic.hpp"

namespace mn {

	/**
	 * @brief This class implements a simple counter, whit atomic operations
	 * for use in a multithreaded environment.
	 */
	class atomic_counter {
	public:
		using value_type = int;
		/**
		 * @brief Construct a new atomic_counter and initializes it to zero.
		 */
		atomic_counter();
		/**
		 * @brief Construct a new atomic_counter and initializes it with the given
		 * value.
		 * @param value The start value for this the counter.
		 */
		atomic_counter(const value_type& value);
		/**
		 * @brief Construct a new atomic_counter from a other atomic_counter.
		 * @param other The other counter from copyed it.
		 */
		atomic_counter(const atomic_counter& other);

		/**
		 * @brief Converts the atomic_counter to value_type.
		 */
		inline operator value_type () const 			{ return m_atomicCount.load(); }
		/**
		 * @brief Get the value of the counter.
		 * @return The value of the counter.
		 */
		inline value_type value() const					{ return m_atomicCount.load(); }

		/**
		 * @brief Increments the counter.
		 * @return The result of the increment.
		 */
		inline value_type operator ++ ()				{ return ++m_atomicCount; }
		/**
		 * @brief Decrements the counter.
		 * @return The result of the decrement.
		 */
		inline value_type operator -- ()				{ return --m_atomicCount; }

		/**
		 * @brief Increments the counter.
		 * @return The previous value.
		 */
		inline value_type operator ++ (int) 			{ return m_atomicCount++; }
		/**
		 * @brief Decrements the counter.
		 * @return The previous value.
		 */
		inline value_type operator -- (int) 			{ return m_atomicCount--; }
		/**
		 * @brief If the counter zero?
		 * @return If true the counter is zero and if false otherwise.
		 */
		inline bool operator ! () const					{ return m_atomicCount.load() == 0; }
		/**
		 * @brief Assigns the value of another atomic_counter.
		 */
		atomic_counter& operator = (const atomic_counter& other);
		/**
		 * @brief Assigns a value to the counter.
		 * @param value The value for assigns the counter.
		 */
		atomic_counter& operator = (const value_type& value);
	private:
		_atomic<value_type>	m_atomicCount;
	};
}
#endif // __MINLIB_ATOMIC_COUNTER_H__
