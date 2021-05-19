/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef _MINLIB_4fa7b6b8_4e0e_4bd2_817c_08c6775c3ec2_H_
#define _MINLIB_4fa7b6b8_4e0e_4bd2_817c_08c6775c3ec2_H_

#include "../mn_config.hpp"


#include "mn_shared_ptr.hpp"

namespace mn {
    namespace pointer {
        template <typename T, typename TRefType = atomic_size_t >
        class basic_weak_ptr {
        public:
            using value_type = T;
            using element_type = T;
            using reference = T&;
            using const_value_type = const value_type;
            using pointer = value_type*;
            using count_type = TRefType;


            using self_type = basic_weak_ptr<value_type, count_type>;
            using shared_type = basic_shared_ptr<value_type, count_type>;


            explicit basic_weak_ptr(value_type pValue = 0)
                : m_ptr(pValue), m_ref(1)  { }

            basic_weak_ptr( const self_type& r )
                : m_ptr(r.m_ptr), m_ref(r.m_ref)  { }

            basic_weak_ptr( shared_type& pShrd)
                : m_ptr(pShrd.get()), m_ref( pShrd.ref() ) {}

            template<class U, typename YRefType = count_type>
            basic_weak_ptr( const basic_weak_ptr<U, YRefType>& r )
                : m_ptr(r.m_ptr), m_ref(r.m_ref)  { }

            template<class U, typename YRefType = count_type>
            basic_weak_ptr( const basic_shared_ptr<U, YRefType>& pShrd)
                : m_ptr(pShrd.get()), m_ref(pShrd.ref() ) {}

            shared_type lock()                          { return shared_type(m_ptr); }
            bool expired()                              { return m_ref.get() == 0; }
            void reset()                                { self_type(0).swap(*this); }

            count_type use_count()                  	{ return m_ref.get(); }

            void swap(self_type& other) {
                mn::swap<pointer>(m_ptr, other.m_ptr);
                mn::swap<count_type >(m_ref, other.m_ref);
            }
            template<class Y, typename YRefType = atomic_size_t>
            bool owner_before( const basic_weak_ptr<Y, YRefType> & rhs ) {
                return m_ref < rhs.m_ref;
            }
            template<class Y, typename YRefType = atomic_size_t>
            bool owner_before( const basic_shared_ptr<Y, YRefType> & rhs ) {
                return m_ref < rhs.ref();
            }

            pointer get() const {
		        return static_cast<T*>(m_ptr);
	        }

            pointer operator->() const {
                assert(get() != 0);
                return this->get();
            }
            const_value_type& operator*() {
                assert(get() != 0);
                return *this->get();
            }

            operator bool() {
                return m_ptr != 0;
            }

            self_type& operator=( const self_type& r ) {
                m_ptr = r.m_ptr;
                m_ref = r.m_ref;
                return *this;
            }


        private:
        	pointer m_ptr;
            count_type m_ref;
        };
    }
}

#endif
