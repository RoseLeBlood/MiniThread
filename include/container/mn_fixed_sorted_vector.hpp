/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2021 Amber-Sophia Schroeck
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
#ifndef _MINLIB_346db2dc_fc5f_437f_8cf4_5f2dc34bfcd1_H_
#define _MINLIB_346db2dc_fc5f_437f_8cf4_5f2dc34bfcd1_H_

#include "mn_fixed_vector.hpp"
#include "mn_sorted_vector.hpp"

namespace mn {
    namespace container {

        template<typename TKey, typename TValue,  int TCapacity, class TAllocator, class TCompare = mn::less<TKey> >
		class basic_fixed_sorted_vector : public basic_sorted_vector<TKey, TValue, TAllocator, TCompare,
                                    fixed_vector_storage< basic_pair<TKey, TValue>, TAllocator, TCapacity > > {
            using base_type = basic_sorted_vector<TKey, TValue, TAllocator, TCompare, fixed_vector_storage< basic_pair<TKey, TValue>, TAllocator, TCapacity > >;
        public:
            using iterator_category = typename base_type::iterator_category ;
            using value_type = typename base_type::value_type;
            using pointer = typename base_type::pointer;
            using reference = typename base_type::reference;
            using difference_type = typename base_type::difference_type;

            using iterator = typename base_type::iterator;
            using const_iterator = typename base_type::const_iterator;

            using allocator_type = typename base_type::allocator_type;
            using size_type = typename base_type::size_type;
            using self_type = basic_fixed_sorted_vector<TKey, TValue, TCapacity, TAllocator, TCompare >;

            explicit basic_fixed_sorted_vector(const allocator_type& allocator = allocator_type())
                : base_type(allocator) { }

            basic_fixed_sorted_vector(const self_type& ) = delete;
            self_type& operator = (const self_type& ) = delete;

            using base_type::begin;
            using base_type::end;
            using base_type::size;
            using base_type::empty;
            using base_type::capacity;
            using base_type::clear;
            using base_type::get_allocator;
            using base_type::set_allocator;
        };
    }
}

#endif