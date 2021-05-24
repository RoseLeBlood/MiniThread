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
#ifndef __MINILIB_VOID_TYPE_H__
#define __MINILIB_VOID_TYPE_H__

#define config_HAVEVOIDT 1

#include "mn_config.hpp"

namespace mn {
	/**
	 * @brief A dummy class with value-type semantics, mostly useful as a template argument.
	 */
	class basic_void_type {
	public:
		/**
		 * @brief Construct the void type.
		 */
		basic_void_type();

		/**
		 * @brief Creates the Void from another Void.
		 * @note Do nothing.
		 */
		basic_void_type(const basic_void_type& other) noexcept;
		/**
		 * @brief Deconstrut the void type.
		 */
		 ~basic_void_type();

		/**
		 * @brief Assigns another void.
		 */
		basic_void_type& operator = (const basic_void_type& v) noexcept;
		/**
		 * @brief Will return always true due to Voids having no members.
		 */
		bool operator ==(const basic_void_type& v) const noexcept;
		/**
		 * @brief Will return always false due to Voids having no members.
		 */
		bool operator !=(const basic_void_type& v) const noexcept;
	};

	template <class ...ts>
	struct make_void { using type = basic_void_type; };

    template <class ...ts>
    using void_t = typename make_void<ts ...>::type;
}

#endif // __MINILIB_VOID_TYPE_H__
