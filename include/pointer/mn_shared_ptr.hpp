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

#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {

        template < typename T, typename TRefType = atomic_size_t > 
        class basic_shared_ptr  : pointer_ptr<T>  {
            using base_type = pointer_ptr<T>;
        public:
            using value_type = typename base_type::value_type;
            using ref_type = typename base_type::ref_type;
            using const_value_type = typename base_type::const_value_type;
            using pointer = typename base_type::pointer;
            using reference_type = TRefType;

            using self_type = basic_shared_ptr<value_type, reference_type>;
            
            using base_type::__ptr;
            using base_type::operator bool;
            using base_type::operator*;
            using base_type::operator->;
            using base_type::get;

            explicit basic_shared_ptr(pointer ptr ) 
                : base_type(ptr), _m_ref(1)  { }
            
            basic_shared_ptr(const self_type& sp) {
                assert( (++_m_ref != 0) );
                __ptr = sp.__ptr;
            }

            ~basic_shared_ptr() { 
                if (--_m_ref == 0) delete __ptr;
            }

            pointer release() {
                pointer __px = this->get();
                if (--_m_ref == 0) delete __ptr;

                return __px;
            }
            void reset( pointer pValue = 0) 
                { self_type(pValue).swap(*this); }

            reference_type ref() {
                return _m_ref;
            }
            void swap(self_type& b) {
                mn::swap<pointer>(__ptr, b.__ptr);
                mn::swap<reference_type >(_m_ref, b._m_ref);
            }

            self_type& operator = (self_type& sp) {
                release();                
                __ptr = sp.__ptr;
                _m_ref = sp._m_ref;
                return *this;
            }  
        private:
            reference_type _m_ref;
        };
    }
}

#endif