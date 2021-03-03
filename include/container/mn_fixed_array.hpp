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
#ifndef _MINLIB_498a12b5_e707_49ba_9f89_2ea37b8e28aa_H_
#define _MINLIB_498a12b5_e707_49ba_9f89_2ea37b8e28aa_H_

#include "mn_array.hpp"

namespace mn {
    namespace container {
        
        /**
         * @brief Fixed Array type 
         * 
         * @tparam T The holding type of value
         * @tparam N The Number of Elements
         */
        template <typename T, size_t N>
        class basic_fixed_array : basic_array<T, N> {
        public:
            /**
             * @brief Construct a new basic fixed array object
             */
            basic_fixed_array() : basic_array() { }
            /**
             * @brief Construct a new basic fixed array object
             */
            basic_fixed_array(const T a[N]) : basic_array(a) {}  
        };

        /**
         * @brief Fixed Array type 
         * 
         * @tparam T The holding type of value
         * @tparam N The Number of Elements
         */
        template <typename T, size_t N> 
        using fixed_array = basic_fixed_array<T, N>;
    }
}

#endif