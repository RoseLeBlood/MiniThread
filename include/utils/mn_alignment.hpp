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
#ifndef _MINLIB_ALIGNMENT_H_
#define _MINLIB_ALIGNMENT_H_

#include <stddef.h>
#include <stdint.h>

namespace mn {
    namespace internal {
        
        template<typename T>
	    struct alignof_helper {
	        char    x;
	        T 		y;
	    };

        struct __attribute__ ((aligned (32))) aligned32 { uint64_t member[4]; } ;
        struct __attribute__ ((aligned (16))) aligned16 { uint64_t member[2]; } ;


        template<size_t N> struct type_with_alignment {
	        typedef char err_invalid_alignment[N > 0 ? -1 : 1];
	    };
        template<> struct type_with_alignment<0> {};
        template<> struct type_with_alignment<1> { uint8_t member; };
        template<> struct type_with_alignment<2> { uint16_t member; };
        template<> struct type_with_alignment<4> { uint32_t member; };
        template<> struct type_with_alignment<8> { uint64_t member; };
        template<> struct type_with_alignment<16> { aligned16 member; };
        template<> struct type_with_alignment<32> { aligned32 member; };
    }
    template<typename T>
	struct aligof {
	    enum {
	        res = offsetof(internal::alignof_helper<T>, y)
	    };
	};

    template<typename T>
	struct aligned_as {
	    using res = typename internal::type_with_alignment<aligof<T>::res>;
	};
}


#endif