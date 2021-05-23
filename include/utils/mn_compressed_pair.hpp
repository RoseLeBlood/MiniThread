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
#ifndef __MINILIB_EBCO_COMPRESSED_PAIR_H__
#define __MINILIB_EBCO_COMPRESSED_PAIR_H__

#include "../mn_config.hpp"

#include "../mn_algorithm.hpp"
#include "mn_ebo_storage.hpp"

namespace mn {

	/**
	 * A simple compressed pair
	 *
	 * @tparam T type for first value.
	 * @tparam U type for second value.
	 */
	template <typename T, typename U, typename = void>
	class compressed_pair  {
	public:
		using self_type = compressed_pair<T, U>;

		using first_value_type 		 = typename type_traits<T>::value_type;
		using first_reference 		 = typename type_traits<T>::reference;
		using first_const_reference  = typename type_traits<T>::const_reference;
		using second_value_type 	 = typename type_traits<U>::value_type;
		using second_reference 		 = typename type_traits<U>::reference;
		using second_const_reference = typename type_traits<U>::const_reference;

		constexpr compressed_pair() = default;
		explicit  compressed_pair(first_const_reference f) noexcept
			: m_first(f) { }

		explicit compressed_pair(second_const_reference s) noexcept
			: m_second(s) { }

		constexpr compressed_pair(first_const_reference f, second_const_reference s) noexcept
			: m_first(f), m_second(s) { }

		constexpr compressed_pair(const self_type& other) noexcept
				: compressed_pair(other.first(), other.second() ) { }


				  first_reference 		 first()  noexcept 		 { return m_first.get(); }
		constexpr first_const_reference  first()  const noexcept { return m_first.get(); }

				  second_reference 		 second() noexcept 		 { return m_second.get(); }
		constexpr second_const_reference second() const noexcept { return m_second.get(); }

		void swap(self_type& o) {
			mn::swap(m_first,  o.m_first);
			mn::swap(m_second, o.m_second);
		}
	private:
		ebo_storage<T, 0>  m_first;
		ebo_storage<U, 1>  m_second;
	};
}

#endif // __MINILIB_EBCO_COMPRESSED_PAIR_H__
