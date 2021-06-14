/**
 * @file
 * @brief a mustimap container.
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

#ifndef __MINILIB_BASIC_MULTIMAP_H__
#define __MINILIB_BASIC_MULTIMAP_H__

#include "../mn_config.hpp"
#include "mn_array.hpp"
#include "mn_vector.hpp"
#include "mn_pair.hpp"

#include "../utils/mn_utils.hpp"

namespace mn {
	namespace container {

		/**
		 * @brief A sorted associative container that may container multiple entries for each key.
		 * @ingroup container
		 *
		 */
		template <typename TKEY, typename TVAL, class TAllocator, TComp = mn::less<TKEY> >
		class basic_multimap : public vector< pair<TKEY, TVAL>, TAllocator > {
			using base_type = vector<pair<TKEY, TVAL>>;
		public:
			using key_type = TKEY;
			using value_type = TVAL;

			using key_reference = TKEY&;
			using key_const_reference = const TKEY&;
			using value_reference = TVAL&;
			using value_const_reference = const TVAL&;

			using self_type = basic_multimap<TKEY, TVAL, TAllocator, TComp>;

			using iterator_category = typename  base_type::iterator_category;
			using value_type = typename  base_type::value_type;
			using pointer = typename  base_type::pointer;
			using reference = typename  base_type::reference;
			using const_reference = typename  base_type::const_reference;
			using difference_type = typename  base_type::difference_type;
			using iterator = typename  base_type::iterator;
			using const_iterator = typename  base_type::const_iterator;
			using allocator_type = typename  base_type::allocator_type;
			using size_type = typename  base_type::size_type;

			using const_range_t = pair<const_iterator,const_iterator>	;
			using range_t = pair<iterator,iterator>	;
			using key_compare = TComp;

			using initlist_t = mn::initializer_list<value_type>;

			basic_multimap (const allocator_type& allocator = allocator_type())
				: base_type(allocator) {}

			explicit basic_multimap (size_type initialSize, const allocator_type& allocator = allocator_type())
				: base_type (initialSize, allocator) {}

			basic_multimap (const_iterator first, const_iterator last,
							const allocator_type& allocator = allocator_type())
				: base_type(first, last, allocator) { }

			basic_multimap (const self_type& rhs, const allocator_type& allocator = allocator_type())
				: base_type (rhs, allocator) {}

			basic_multimap (basic_multimap&& v)
				: base_type (mn::move(v)) {}

			using base_type::size;
			using base_type::begin;
			using base_type::end;
			using base_type::empty;
			using base_type::capacity;
			using base_type::front;
			using base_type::cfront;
			using base_type::back;
			using base_type::cback;
			using base_type::clear;
			using base_type::erase;
			using base_type::insert;
			using base_type::push_back;
			using base_type::operator[];
			using base_type::operator= ;
			using base_type::swap ;
			using base_type::at;

			void assign (const_iterator i1, const_iterator i2) {
				clear(); insert (i1, i2);
			}
			void erase (const self_type& k) {
				erase (const_cast<iterator>(k), const_cast<iterator>(k));
			}

			const_iterator lower_bound (key_const_reference k) const {
				return mn::lower_bound (begin(), end(), k, value_key_compare());
			}

			iterator lower_bound (key_const_reference k) {
				return mn::lower_bound (begin(), end(), k, value_key_compare());
			}

			const_iterator upper_bound (key_const_reference k) const	{
				return mn::upper_bound (begin(), end(), k, value_key_compare());
			}
			iterator upper_bound (key_const_reference k) {
				return mn::upper_bound (begin(), end(), k, value_key_compare());
			}

			const_iterator	find (key_const_reference k) const {
				 const_iterator i = lower_bound (k);
				 return (i < end() && TComp()(k, i->first)) ? end() : i;
			}
			const_iterator find_value (value_const_reference v, const_iterator first, const_iterator last) const {
				if (!first) first = begin();
				if (!last) last = end();
				for (; first != last && first->second != v; ++first) {}
				return first;
			}
			value_reference operator[] (key_const_reference k) {
				iterator _ip = lower_bound (k);

				if (ip == end() || TComp()(k, _ip->first) )
					ip = base_type::insert (ip, make_pair<TKEY, TVAL> (k, TVAL()));
				return ip->second;
			}

			template <typename... Args>
			iterator emplace (Args&&... args) {
				return insert (value_type(mn::forward<Args>(args)...));
			}

			template <typename... Args>
			iterator emplace_hint (const_iterator h, Args&&... args) {
				return insert (h, value_type(mn::forward<Args>(args)...));
			}

			template <typename... Args>
			iterator emplace_back (Args&&... args) {
				return insert (value_type(mn::forward<Args>(args)...));
			}
		};

		template <typename TKEY, typename TVAL, class TAllocator >
		using multimap = basic_multimap<TKEY, TVAL, TAllocator>;

	}
}


#endif // __MINILIB_BASIC_MULTIMAP_H__
