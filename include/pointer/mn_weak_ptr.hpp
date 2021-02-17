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

namespace mn {
    namespace pointer {
        template <typename T>
        class basic_weak_ptr {
        public:
            using value_type = T;
            using atomic_type = long;
            using element_type = value_type;
            using pointer = value_type*;
            using const_value_type = const value_type;
            using self_type = basic_weak_ptr<value_type>;
            using shared_type = basic_shared_ptr<value_type>;


            explicit basic_weak_ptr(value_type pValue = 0) : _m_ptr(pValue), _m_ref(1)  { }
            basic_weak_ptr( const self_type& r ) : _m_ptr(r._m_ptr), _m_ref(r._m_ref)  { }
            basic_weak_ptr( shared_type& pShrd) : _m_ptr(pShrd.get()), _m_ref( pShrd.ref() ) {}
        
            template <typename other>
            basic_weak_ptr( const basic_weak_ptr<other>& r ) : _m_ptr(r._m_ptr), _m_ref(r._m_ref)  { }
        
            template <typename other>
            basic_weak_ptr( const basic_shared_ptr<other>& pShrd)  : _m_ptr(pShrd.get()), _m_ref(pShrd.ref() ) {}
        
            shared_type lock()                          { return shared_type(_m_ptr); }
            bool expired()                              { return _m_ref.get() == 0; }
            void reset()                                { self_type(0).swap(*this); }

            atomic_type use_count()                     { return _m_ref.get(); }

            void swap(self_type& other) {
                mn::swap<value_type>(_m_ptr, other._m_ptr);
                _m_ref.swap(other._m_ref);
            }
            template<class Y> 
            bool owner_before( const basic_weak_ptr<Y> & rhs ) {
                return _m_ref < rhs._m_ref;
            }
            template<class Y>
            bool owner_before( const basic_shared_ptr<Y> & rhs ) {
                return _m_ref < rhs.ref();
            }

            self_type& operator=( const self_type& r ) {
                _m_ptr = r._m_ptr;
                _m_ref = r._m_ref;
                return *this;
            }

            pointer operator->() {
                assert(_m_ptr != 0);
                return _m_ptr;
	        }
            value_type& operator*() {
                assert(_m_ptr != 0);
                return *_m_ptr;
            }
            operator bool() {
                return _m_ptr != 0;
            }
        private:
            pointer  _m_ptr;
            atomic_t<atomic_type> _m_ref;
        };
    }
}

#endif