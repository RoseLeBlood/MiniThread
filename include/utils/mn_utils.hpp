/** 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
 *
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.

 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
*/

#ifndef MINLIB_STL_UTILS_H_
#define MINLIB_STL_UTILS_H_

#include "mn_inttokey.hpp"

/// Shorthand for container iteration.
#define foreach(type,i,ctr)	for (type i = (ctr).begin(); i != (ctr).end(); ++ i)
/// Shorthand for container reverse iteration.
#define eachfor(type,i,ctr)	for (type i = (ctr).rbegin(); i != (ctr).rend(); ++ i)

#define TEMPLATE_FULL_DECL_ONE(d1,t1)		        template <d1 t1>
#define TEMPLATE_FULL_DECL_TWO(d1,t1,d2,t2)	        template <d1 t1, d2 t2>
#define TEMPLATE_FULL_DECL_THREE(d1,t1,d2,t2,d3,t3)	template <d1 t1, d2 t2, d3 t3>
#define TEMPLATE_DECL_ONE(t1)		                TEMPLATE_FULL_DECL_ONE(typename,t1)
#define TEMPLATE_DECL_TWO(t1,t2)		            TEMPLATE_FULL_DECL_TWO(typename,t1,typename,t2)
#define TEMPLATE_DECL_THREE(t1,t2,t3)	            TEMPLATE_FULL_DECL_THREE(typename,t1,typename,t2,typename,t3)
#define TEMPLATE_TYPE_ONE(type,a1)		            type<a1>
#define TEMPLATE_TYPE_TWO(type,a1,a2)	            type<a1,a2>
#define TEMPLATE_TYPE_THREE(type,a1,a2,a3)	        type<a1,a2,a3>


namespace mn {
    namespace internal {

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy_n(const T* first, size_t n, T* result, int_to_type<false>) {
            const T* last = first + n;

            while (first != last) {
                switch (n & 0x3) {
                    case 0: *result++ = *first++;
                    case 3: *result++ = *first++;
                    case 2: *result++ = *first++;
                    case 1: *result++ = *first++;
                }
            }
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy_n(const T* first, size_t n, T* result, int_to_type<true>) {
            assert(result >= first + n || result < first);
            memcpy(result, first, n * sizeof(T));
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy(const T* first, const T* last, T* result, int_to_type<false>) {
            T* localResult = result;
            while (first != last)
                *localResult++ = *first++;
        }
        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy(const T* first, const T* last, T* result, int_to_type<true>) {
            const size_t n = reinterpret_cast<const char*>(last) - reinterpret_cast<const char*>(first);
            memcpy(result, first, n);
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void move_n(const T* from, size_t n, T* result, int_to_type<false>) {
            for (int i = int(n) - 1; i >= 0; --i)
                    result[i] = from[i];
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void move_n(const T* first, size_t n, T* result, int_to_type<true>) {
            memmove(result, first, n * sizeof(T));
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void move(const T* first, const T* last, T* result, int_to_type<false>) {
            result += (last - first);
            while (--last >= first)
                    *(--result) = *last;
        }
        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline  void move(const T* first, const T* last, T* result, int_to_type<true>) {
            const size_t n = reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(first);
            memmove(result, first, n);
        }
		

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy_construct_n(const T* first, size_t n, T* result, int_to_type<false>) {
            for (size_t i = 0; i < n; ++i)
                new (result + i) T(first[i]);
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void copy_construct_n(const T* first, size_t n, T* result, int_to_type<true>) {
            assert(result >= first + n || result < first);
            memcpy(result, first, n * sizeof(T));
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void destruct_n(T* first, size_t n, int_to_type<false>) {
            sizeof(first);
            for (size_t i = 0; i < n; ++i)
                    (first + i)->~T();
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void destruct_n(T*, size_t, int_to_type<true>) {
            // Nothing to do, no destructor needed.
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void destruct(T* mem, int_to_type<false>) {
            sizeof(mem);
            mem->~T();
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void destruct(T*, int_to_type<true>) {
             // Nothing to do, no destructor needed.
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void construct(T* mem, int_to_type<false>) {
            new (mem) T();
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void construct(T*, int_to_type<true>) {
            // Nothing to do
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void copy_construct(T* mem, const T& orig, int_to_type<false>) {
            new (mem) T(orig);
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void copy_construct(T* mem, const T& orig, int_to_type<true>) {
            mem[0] = orig;
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        void construct_n(T* to, size_t count, int_to_type<false>) {
            sizeof(to);
            for (size_t i = 0; i < count; ++i)
                    new (to + i) T();
        }

        TEMPLATE_FULL_DECL_ONE(typename, T)
        inline void construct_n(T*, int, int_to_type<true>) {
            // trivial ctor, nothing to do.
        }

        TEMPLATE_FULL_DECL_TWO(class, TIter, class, TPred)
        void test_ordering(TIter first, TIter last, const TPred& pred) {
        	sizeof(first); sizeof(last); sizeof(pred);
        }

        TEMPLATE_FULL_DECL_THREE(typename, T1, typename, T2, class, TPred)
        inline bool debug_pred(const TPred& pred, const T1& a, const T2& b) {
         	return pred(a, b);
        }
    }
}

#endif