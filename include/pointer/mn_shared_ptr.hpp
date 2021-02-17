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
#ifndef _MINLIB_831159bd_3f35_4a00_8d46_f3fd737a5797_H_
#define _MINLIB_831159bd_3f35_4a00_8d46_f3fd737a5797_H_


#include "mn_atomic.hpp"
#include "mn_algorithm.hpp"

namespace mn {
    namespace pointer {

        template < typename T > 
        class basic_shared_ptr {
        public:
            using value_type = T;
            using ref_type = T&;
            using const_value_type = const value_type;
            using pointer = value_type*;
            using self_type = basic_shared_ptr<T>;
            using atomic_type = long;
            
            explicit basic_shared_ptr(pointer ptr ) : _m_ptr(ptr), _m_ref(1)  { }
            
            basic_shared_ptr(const self_type& sp) {
                assert( (++_m_ref != 0) );
                _m_ptr = sp._m_ptr;
            }

            ~basic_shared_ptr() { 
                if (--_m_ref == 0) delete _m_ptr;
            }

            pointer release() {
                pointer __px = this->get();
                if (--_m_ref == 0) delete _m_ptr;

                return __px;
            }
            void reset( pointer pValue = 0) 
                { self_type(pValue).swap(*this); }
           
            pointer get() const { 
		        return static_cast<T*>(_m_ptr);
	        }

            atomic_type ref() {
                return _m_ref->get();
            }
            void swap(self_type& b) {
                mn::swap<value_type*>(_m_ptr, b._m_ptr);
                mn::swap<atomic<atomic_type>>(_m_ref, b._m_ref);
            }

            self_type& operator = (self_type& sp) {
                release();                
                _m_ptr = sp._m_ptr;
                _m_ref = sp._m_ref;
                return *this;
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
                return _m_ptr != 0;
            }
        private:
            pointer  _m_ptr;
            atomic_t<atomic_type> _m_ref;
        };
    }
}

#endif