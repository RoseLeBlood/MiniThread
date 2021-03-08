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
#ifndef _MINLIB_f19367b0_5308_4f90_a8b9_5bb631bdb591_H_
#define _MINLIB_f19367b0_5308_4f90_a8b9_5bb631bdb591_H_

#include "mn_vector.hpp"

namespace mn {
    namespace container {
        template<typename T, class TAllocator, int TCapacity> 
        struct fixed_vector_storage {
            using allocator_type = TAllocator;
            using self_type = fixed_vector_storage<T, TAllocator, TCapacity>;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using size_type = mn::size_t;
            using etype_t = typename aligned_as<value_type>::res; 

            explicit fixed_vector_storage(const TAllocator& allocator)
                : m_begin((pointer)&m_data[0]),  
                  m_end(m_begin), 
                  m_capacityEnd(m_begin + TCapacity), 
                  m_allocator(allocator) , 
                  m_max_size(0) { }       

            void reallocate(size_type newCapacity, size_type oldSize) { 
                assert(!"fixed_vector cannot grow");
            }
            void reallocate_discard_old(size_type newCapacity) {
                assert(!"fixed_vector cannot grow");
            }
            inline void destroy(pointer ptr, size_type n) {
                mn::destruct_n(ptr, n);
                if ( (etype_t*)ptr != &m_data[0] ) 
                     m_allocator.free(ptr);
            }
            bool invariant() const {
                return m_end >= m_begin;
            }

            pointer m_begin;       
            pointer m_end;
            allocator_type m_data[(TCapacity * sizeof(T)) / sizeof(etype_t)];
            pointer m_capacityEnd;
            TAllocator m_allocator;
            size_type  m_max_size;
        };

        template<typename T, int TCapacity, class TAllocator> 
		class basic_fixed_vector : public basic_vector<T, TAllocator, fixed_vector_storage<T, TAllocator, TCapacity> > {
            using base_type = basic_vector<T, TAllocator, fixed_vector_storage<T, TAllocator, TCapacity> >;
        public:
            using iterator_category = random_access_iterator_tag ;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;

            using iterator = pointer;
            using const_iterator = const pointer;

            using allocator_type = TAllocator;
            using size_type = mn::size_t;
            using self_type = basic_fixed_vector<T, TCapacity, TAllocator>;

            explicit basic_fixed_vector(const allocator_type& allocator = allocator_type())
                : base_type(allocator) { }

            explicit basic_fixed_vector(size_type initialSize, const allocator_type& allocator = allocator_type())
                : base_type(initialSize, allocator) { }

            basic_fixed_vector(const pointer first, const pointer last, const allocator_type& allocator = allocator_type())
                : base_type(first, last, allocator) { }

            basic_fixed_vector(const self_type& rhs, const allocator_type& allocator = allocator_type())
                : base_type(rhs, allocator) { }

            using base_type::begin;
            using base_type::end;
            using base_type::size;
            using base_type::empty;
            using base_type::capacity;
            using base_type::clear;
            using base_type::get_allocator;
            using base_type::set_allocator;
            
            self_type& operator=(const self_type& rhs) {
                if (&rhs != this) {
                    base_type::copy(rhs);
                }
                return *this;
            }
        };

        template<typename T, int TCapacity> 
		using fixed_vector =  basic_fixed_vector<T, TCapacity, mn::memory::default_allocator_t>; 
    }
}

#endif