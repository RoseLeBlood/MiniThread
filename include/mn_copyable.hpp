/**
 * @file
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

#ifndef __MINILIB_TAG_3ba66bf1_9f2a_4ae7_a110_aa6d418b914__
#define __MINILIB_TAG_3ba66bf1_9f2a_4ae7_a110_aa6d418b914__

#include "mn_config.hpp"

#define	MN_ONCOPYABLE_CLASS		public mn::only_copyable_tag<void>
#define MN_ONMOVABLE_CLASS		public mn::only_move_tag<void>
#define MN_ONSIGLETN_CLASS		public mn::only_oneable_tag<void>

#define MN_DEFAULT_CLASS		public mn::copy_move_tag<void>

namespace mn {
	/**
	 * @brief Not movable
	 */
	template <typename t>
	class only_copyable_tag {
 	public:
		using this_type = only_copyable_tag;

		only_copyable_tag() = default;

	  	only_copyable_tag(const this_type& other) = default;
	  	this_type& operator=(const this_type& other) = default;
	};

	/**
	 * @brief Not copyable
	 */
	template <typename t>
	class only_move_tag {
	public:
		using this_type = only_move_tag;

		only_move_tag() = default;

		only_move_tag(this_type&& other) = default;
		only_move_tag(const this_type&) = delete;

		this_type& operator=(const this_type&) = delete;
		this_type& operator=(this_type&& other) = default;
	};

	/**
	 * @brief Not copyable or movable
	 */
	template <typename t>
	class only_oneable_tag {
	public:
		using this_type = only_oneable_tag;

		only_oneable_tag() = default;

		only_oneable_tag(this_type&& other) = delete;
		only_oneable_tag(const this_type&) = delete;

		this_type& operator=(const this_type&) = delete;
		this_type& operator=(this_type&& other) = delete;
	};

	/**
	 * @brief copyable and movable
	 */
	template <typename t>
	class copy_move_tag {
	public:
		using this_type = copy_move_tag;

		copy_move_tag() = default;
	};
}

#endif // __MINILIB_TAG_3ba66bf1_9f2a_4ae7_a110_aa6d418b914__
