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
#ifndef MINLIB_22d7d658_9601_4b2d_afb1_c4b43d387f33_H_
#define MINLIB_22d7d658_9601_4b2d_afb1_c4b43d387f33_H_

#include "../mn_def.hpp"
#include "../mn_algorithm.hpp"
#include "mn_atomic.hpp"

namespace mn {
    namespace pointer {

        template <typename T> 
        struct pointer_ptr { 
            using value_type = T;
            using ref_type = T&;
            using const_value_type = const value_type;
            using pointer = value_type*;

            pointer_ptr(const pointer _pValue) : __ptr(_pValue) { }
            
            pointer get() const { 
		        return static_cast<T*>(__ptr);
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
                return __ptr != 0;
            }
            pointer __ptr;
        };
    }
}


#endif