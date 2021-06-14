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
#ifndef _MINILIB_cd8c0484_ab57_11eb_9d1f_f714def23089_H__
#define _MINILIB_cd8c0484_ab57_11eb_9d1f_f714def23089_H__

#include "mn_config.hpp"
#include "container/mn_list.hpp"

namespace mn {

	class auto_release_object {
	public:
		using pointer = void*;
		using selt_type = auto_release_object;

		virtual pointer 	release()  { return nullptr; }
		virtual selt_type* 	duplicate() { return this; }
	};

	/**
	 * @brief A simple garbage collector - pool based, a object for this colector must be
	 * extends auto_release_object.
	 * @tparam TRELOBJ The object
	 * @tparam TALLOCATOR The using allocator for the list
	 */
	template <class TRELOBJ = auto_release_object, class TALLOCATOR = memory::default_allocator_t >
	class auto_release {
	public:
		using self_type = auto_release<TRELOBJ, TALLOCATOR>;
		using value_type = TRELOBJ;
		using pointer = TRELOBJ*;
		using reference = TRELOBJ&;
		using list_type = mn::container::list<TRELOBJ, TALLOCATOR>;

		auto_release() { }
		/**
		 * @brief Destroy this pool and release all objects
		 */
		~auto_release() { release(); }

		/**
		 * @brief Add a Object to the pool
		 */
		void add(pointer pObject) {
			if(pObject) m_listReleaseble.push_back(pObject);
		}

		/**
		 * @brief Release all objects in the pool
		 */
		void release() {
			while(!m_listReleaseble.empty()) {
				m_listReleaseble.front()->release();
				m_listReleaseble.pop_front();
			}
		}

		pointer front() {
			return m_listReleaseble.front();
		}
		pointer back() {
			return m_listReleaseble.back();
		}


	private:
		list_type 	m_listReleaseble;
	};
}

#endif // _MINILIB_cd8c0484_ab57_11eb_9d1f_f714def23089_H__
