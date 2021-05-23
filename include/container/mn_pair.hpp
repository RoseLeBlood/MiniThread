/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.
*/
#ifndef _MINILIB_19ee7665_8c63_48b7_803f_9c056afce956_H_
#define _MINILIB_19ee7665_8c63_48b7_803f_9c056afce956_H_

#include "../mn_typetraits.hpp"


namespace mn {

	namespace container {
		template <typename TFIRST, typename TSECOND>
		struct basic_pair {
			using first_type = TFIRST;
			using pointer_first = TFIRST*;
			using reference_first = TFIRST&;
			using const_reference_first = const TFIRST&;

			using second_type = TSECOND;
			using pointer_second = TSECOND*;
			using reference_second = TSECOND&;
			using const_reference_second = const TSECOND&;

			using self_type = basic_pair<TFIRST, TSECOND>;

			basic_pair() { }

			explicit basic_pair(const reference_first a) noexcept
				: first(a) { }
			basic_pair(const reference_first a, reference_second b)
				: first(a), second(b) { }

			basic_pair(const self_type& other) noexcept
				: first(other.first), second(other.second) { }

			void swap(const self_type& other) noexcept {
				self_type _temp(this);
				other = this;
				this = _temp;
			}
			self_type& operator = (const self_type& rhs) noexcept {
				first = rhs.first;
				second = rhs.second;
				return *this;
			}

			bool operator == (const self_type& rhs) noexcept {
				if(first != rhs.first) return false;
				return second == rhs.second;
			}
			bool operator != (const self_type& rhs) noexcept {
				if(first == rhs.first) return false;
				return second != rhs.second;
			}


			first_type first;
			second_type second;
		};

		template<typename TFIRST, typename TSECOND>
        basic_pair<TFIRST, TSECOND> make_pair(const TFIRST& a, const TSECOND& b) {
            return basic_pair<TFIRST, TSECOND>(a, b);
        }

        template <typename TFIRST, typename TSECOND>
    	struct is_pod< basic_pair<TFIRST, TSECOND> > {
			enum {
				value = ( (is_pod<TFIRST>::value || is_fundamental<TFIRST>::value) &&
						  (is_pod<TSECOND>::value || is_fundamental<TSECOND>::value) )
			};
    	};

    	template<typename TFIRST, typename TSECOND>
        using pair = basic_pair<TFIRST, TSECOND>;
	}

}

#endif
