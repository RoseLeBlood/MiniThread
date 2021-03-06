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
#ifndef _MINILIB_96612aaf_c4c6_4113_b496_477cb506a2b8_H_
#define _MINILIB_96612aaf_c4c6_4113_b496_477cb506a2b8_H_

#include "../mn_typetraits.hpp"


namespace mn {

	namespace container {
        
		template <typename TFIRST, typename TSECOND, typename TTHIRD> 
		struct basic_triple {
			using first_type = TFIRST;
			using pointer_first = TFIRST*;
			using reference_first = TFIRST&;

			using second_type = TSECOND;
			using pointer_second = TSECOND*;
			using reference_second = TSECOND&;

            using third_type = TTHIRD;
            using pointer_third = TTHIRD*;
			using reference_third = TTHIRD&;

			using self_type = basic_triple<TFIRST, TSECOND, TTHIRD>;

			basic_triple() { }

			explicit basic_triple(const reference_first a) 
				: first(a) { }
			basic_triple(const reference_first a, reference_second b, reference_third c)
				: first(a), second(b), third(c) { }

			basic_triple(const self_type& other) 
				: first(other.first), second(other.second), third(other.third) { }
			
			void swap(const self_type& other) {
				self_type _temp(this);
				other = this;
				this = _temp;
			}
			self_type& operator = (const self_type& rhs) {
				first = rhs.first;
				second = rhs.second;
                third = rhs.third;
				return *this;
			}

			bool operator == (const self_type& rhs) {
				if(first != rhs.first) return false;
				return second == rhs.second;
			}
			bool operator != (const self_type& rhs) {
				if(first == rhs.first) return false;
				return second != rhs.second;
			}


			first_type first;
			second_type second;
            third_type third;
		};

		template<typename TFIRST, typename TSECOND, typename TTHIRD> 
        using triple = basic_triple<TFIRST, TSECOND, TTHIRD>;

		template<typename TFIRST, typename TSECOND, typename TTHIRD> 
        triple<TFIRST, TSECOND, TTHIRD> make_triple(const TFIRST& a, const TSECOND& b, const TTHIRD& c) {
            return triple<TFIRST, TSECOND>(a, b, c);
        }
	}
	template<typename TFIRST, typename TSECOND, typename TTHIRD> 
    struct is_pod< container::triple<TFIRST, TSECOND, TTHIRD> > {
        enum { 
            value = ( (is_pod<TFIRST>::value || is_fundamental<TFIRST>::value) && 
                      (is_pod<TSECOND>::value || is_fundamental<TSECOND>::value) &&
                      (is_pod<TTHIRD>::value || is_fundamental<TTHIRD>::value) )
        };
    };
}

#endif