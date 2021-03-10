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
*<https://www.gnu.org/licenses/>;.  
*/
#ifndef _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_
#define _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_

#include "mn_def.hpp"
#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__) || defined(__xlc__)
#include "atomic/mn_atomic_gcc.hpp"
#else
#error Unsupported compiler / system.
#endif

namespace mn {
    
#if defined(__GNUC__) || defined(__clang__) || defined(__xlc__)
    template <typename T>
    using atomic  = basic_atomic_gcc<T>;
    using atomic_flag = basic_atomic_gcc_flag;
#endif

    //using atomic_ptr            = atomic<void*>;
    using atomic_bool           = atomic<bool>;
    using atomic_char	        = atomic<char>;
    using atomic_schar	        = atomic<signed char>;
    using atomic_uchar	        = atomic<unsigned char>;
    using atomic_short	        = atomic<short>;
    using atomic_ushort	        = atomic<unsigned short>;
    using atomic_int	        = atomic<int>;
    using atomic_uint	        = atomic<unsigned int>;
    using atomic_long	        = atomic<long>;
    using atomic_ulong	        = atomic<unsigned long>;

    using atomic_char16_t   	= atomic<char16_t>;
    using atomic_char32_t	    = atomic<char32_t>;
    using atomic_wchar_t	    = atomic<wchar_t>;

    using atomic_int_least8_t	= atomic<int_least8_t>;
    using atomic_uint_least8_t	= atomic<uint_least8_t>;
    using atomic_int_least16_t	= atomic<int_least16_t>;
    using atomic_uint_least16_t	= atomic<uint_least16_t>;
    using atomic_int_least32_t	= atomic<int_least32_t>;
    using atomic_uint_least32_t	= atomic<uint_least32_t>;
    using atomic_int_least64_t	= atomic<int_least64_t>;
    using atomic_uint_least64_t	= atomic<uint_least64_t>;

    using atomic_int_fast8_t	= atomic<int_fast8_t>;
    using atomic_uint_fast8_t	= atomic<uint_fast8_t>;
    using atomic_int_fast16_t	= atomic<int_fast16_t>;
    using atomic_uint_fast16_t	= atomic<uint_fast16_t>;
    using atomic_int_fast32_t	= atomic<int_fast32_t>;
    using atomic_uint_fast32_t	= atomic<uint_fast32_t>;
    using atomic_int_fast64_t	= atomic<int_fast64_t>;
    using atomic_uint_fast64_t	= atomic<uint_fast64_t>;

    using atomic_intptr_t	    = atomic<intptr_t>;
    using atomic_uintptr_t	    = atomic<uintptr_t>;
    using atomic_size_t	        = atomic<size_t>;
    using atomic_ptrdiff_t	    = atomic<ptrdiff_t>;
    using atomic_intmax_t	    = atomic<intmax_t>;
    using atomic_uintmax_t	    = atomic<uintmax_t>;
}


#endif