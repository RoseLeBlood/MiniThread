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


#include "mn_shared_ptr.hpp"
#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {
        template <typename T, typename TRefType = atomic_size_t >
        class basic_weak_ptr  : pointer_ptr<T>  {
            using base_type = pointer_ptr<T>;
        public:
            using value_type = typename base_type::value_type;
            using ref_type = typename base_type::ref_type;
            using const_value_type = typename base_type::const_value_type;
            using pointer = typename base_type::pointer;


            using reference_type = TRefType;
            using element_type = value_type;

            using self_type = basic_weak_ptr<value_type, TRefType>;
            using shared_type = basic_shared_ptr<value_type, TRefType>;

            using base_type::__ptr;
            using base_type::operator bool;
            using base_type::operator*;
            using base_type::operator->;
            using base_type::get;

            explicit basic_weak_ptr(value_type pValue = 0) 
                : base_type(pValue), _m_ref(1)  { }
            basic_weak_ptr( const self_type& r ) 
                : base_type(r.__ptr), _m_ref(r._m_ref)  { }
            basic_weak_ptr( shared_type& pShrd) 
                : base_type(pShrd.get()), _m_ref( pShrd.ref() ) {}
        
            template<class Y, typename YRefType = atomic_size_t>
            basic_weak_ptr( const basic_weak_ptr<Y, YRefType>& r ) 
                : base_type(r.__ptr), _m_ref(r._m_ref)  { }
        
            template<class Y, typename YRefType = atomic_size_t> 
            basic_weak_ptr( const basic_shared_ptr<Y, YRefType>& pShrd)  
                : base_type(pShrd.get()), _m_ref(pShrd.ref() ) {}
        
            shared_type lock()                          { return shared_type(__ptr); }
            bool expired()                              { return _m_ref.get() == 0; }
            void reset()                                { self_type(0).swap(*this); }

            reference_type use_count()                  { return _m_ref.get(); }

            void swap(self_type& other) {
                mn::swap<pointer>(__ptr, other.__ptr);
                mn::swap<reference_type >(_m_ref, other._m_ref);
            }
            template<class Y, typename YRefType = atomic_size_t> 
            bool owner_before( const basic_weak_ptr<Y, YRefType> & rhs ) {
                return _m_ref < rhs._m_ref;
            }
            template<class Y, typename YRefType = atomic_size_t> 
            bool owner_before( const basic_shared_ptr<Y, YRefType> & rhs ) {
                return _m_ref < rhs.ref();
            }

            self_type& operator=( const self_type& r ) {
                __ptr = r.__ptr;
                _m_ref = r._m_ref;
                return *this;
            }
        private:
            reference_type _m_ref;
        };
    }
}

#endif