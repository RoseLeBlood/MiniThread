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
#ifndef _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_
#define _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_

#include "mn_atomic_gcc.hpp"

namespace mn {
    template <typename T>
    using atomic_gcc_t = basic_atomic_gcc<T>;

    template<typename T>
    using atomic_t = atomic_gcc_t<T>;
}


#endif