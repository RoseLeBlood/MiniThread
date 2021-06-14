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

#ifndef __MINILIB_INITIALIZER_LIST_H__
#define __MINILIB_INITIALIZER_LIST_H__

namespace mn {
	template <typename T>
	class basic_initializer_list	 {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
		using size_type = mn::size_t;
		using iterator = T*;
		using const_iterator = const T*;

		/**
		 * @brief The default construtor.
		 */
		constexpr basic_initializer_list() noexcept
      		: m_pArray(0), m_szSize(0) { }

		/**
		 * @brief Get the number of elements in the list.
		 * @return The number of elements in the list.
		 */
		constexpr size_type 	 size() const noexcept 	{ return m_szSize; }

		/**
		 * @brief Get a pointer from the first element of the list.
		 * @return The pointer from the first element of the list.
		 */
		constexpr const_iterator begin() const noexcept { return m_pArray; }

		/**
		 * @brief Get a pointer from the last element of the list.
		 * @return The pointer from the last element of the list.
		 */
		constexpr const_iterator end() const noexcept 	{ return begin() + size(); }

		/**
		 * @brief Get a pointer from the first element of the list.
		 * @param pOther  Initializer list.
		 * @return The pointer from the first element of the list.
		 */
		template<class U>
		constexpr const U* begin(basic_initializer_list<U> pOther) noexcept {
			return pOther.begin();
		}

		/**
		 * @brief Get a pointer from the last element of the list.
		 * @param pOther  Initializer list.
		 * @return The pointer from the last element of the list.
		 */
		template<class U>
		constexpr const U* end(basic_initializer_list<U> pOther) noexcept {
			return pOther.end();
		}
	private:
		/**
		 * @brief A private constructor.
		 * @param A A pointer of the array for the list.
		 * @param l The number of elements in the array.
		 */
		constexpr basic_initializer_list(const_pointer a, size_type l)
      		: m_pArray(a), m_szSize(l) { }
	private:
		pointer m_pArray;
		size_type m_szSize;
	};

	template <typename T>
	using initializer_list = basic_initializer_list<T>;
}

#endif // __MINILIB_INITIALIZER_LIST_H__
