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
#ifndef __MINILIB_EBCO_EBO_STORAGE_H__
#define __MINILIB_EBCO_EBO_STORAGE_H__

#include "../mn_config.hpp"

#include "../mn_functional.hpp"

namespace mn {


	template <typename T, int TAG = 0, typename = void>
	class ebo_storage {
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;

		constexpr ebo_storage() = default;

		template<typename U>
		constexpr ebo_storage(U&& u) noexcept : m_iItem(mn::forward(u) ) {}

				  		reference get() noexcept 		{ return m_iItem; }
		constexpr const_reference get() const noexcept 	{ return m_iItem; }

		ebo_storage& operator=( const ebo_storage& other ) = default;
        ebo_storage& operator=( ebo_storage&& other ) = default;
	private:
		T m_iItem;
	};

	template <typename T, int TAG>
	class ebo_storage<T, TAG, mn::enable_if_t<mn::is_class<T>::value>> : private T {
		using base_type = T;
	public:
		using value_type = T;
		using reference = T&;
		using const_reference = const T&;

		using base_type::T;

		constexpr ebo_storage() = default;
		constexpr ebo_storage(const_reference type)  : base_type(type) { }
		constexpr ebo_storage(const T&& type) 		 : base_type(mn::move(type)) { }

				  reference 	  get() noexcept 		{ return *this; }
		constexpr const_reference get() const noexcept 	{ return *this; }
	};
}
#endif // __MINILIB_EBCO_EBO_STORAGE_H__
