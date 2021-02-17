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

#ifndef _MINLIB_STL_ALGORITHM_H_
#define _MINLIB_STL_ALGORITHM_H_

#include "utils/mn_alignment.hpp"
#include "utils/mn_inttokey.hpp"
#include "utils/mn_utils.hpp"

#include "mn_typetraits.hpp"
#include "mn_iterator.hpp"

namespace mn {
    template<typename T> 
    inline void  copy_construct(T* mem, const T& orig) {
	        internal::copy_construct(mem, orig, int_to_type<has_trivial_copy<T>::value>());
	}

	template<typename T> 
    inline void construct(T* mem) {
	        internal::construct(mem, int_to_type<has_trivial_constructor<T>::value>());
	}

	template<typename T> 
    inline void destruct(T* mem) {
	        internal::destruct(mem, int_to_type<has_trivial_destructor<T>::value>());
	}

    /**
     * @brief Copy N elements from src to dest
     * 
     * @tparam T The type of element
     * @param src The source 
     * @param n How many elements are copy te destination
     * @param dest The destination
     */
	template<typename T> 
    void copy_n(const T* src, size_t n, T* dest) {
	        internal::copy_n(src, n, dest, int_to_type<has_trivial_copy<T>::value>());
	}

	template<typename T> 
    void copy(const T* src, const T* last, T* dest) {
	        internal::copy(src, last, dest, int_to_type<has_trivial_copy<T>::value>());
	}

	template<typename T> 
    void copy_construct_n(T* src, size_t n, T* dest) {
	        internal::copy_construct_n(src, n, dest, int_to_type<has_trivial_copy<T>::value>());
	}

	template<typename T> 
    void move_n(const T* from, size_t n, T* dest) {
        assert(from != dest || n == 0);

        if (dest + n >= from && dest < from + n) {
            internal::move_n(from, n, dest, int_to_type<has_trivial_copy<T>::value>());
        } else {
            internal::copy_n(from, n, dest, int_to_type<has_trivial_copy<T>::value>());
        }
	}

	template<typename T> 
    inline void move(const T* src, const T* last, T* dest) {
        assert(src != dest || src == last);
        const size_t n = reinterpret_cast<uintptr_t>(last) - reinterpret_cast<uintptr_t>(src);
        const unsigned char* destEnd = reinterpret_cast<const unsigned char*>(dest) + n;

        if (destEnd >= reinterpret_cast<const unsigned char*>(src) && dest < last) {
            internal::move(src, last, dest, int_to_type<has_trivial_copy<T>::value>());
        } else {
            internal::copy(src, last, dest, int_to_type<has_trivial_copy<T>::value>());
        }
	}

	template<class TIter, class TFn> 
    TFn foreach(TIter src, TIter last, TFn fn) {
        while (src!=last)  {
            fn (*src); ++src;
        }
        return (fn);      
	}

	template<typename T> 
    void construct_n(T* src, size_t n) {
	    internal::construct_n(src, n, int_to_type<has_trivial_constructor<T>::value>());
	}
	template<typename T> 
    void destruct_n(T* src, size_t n) {
	    internal::destruct_n(src, n, int_to_type<has_trivial_destructor<T>::value>());
	}
	template<typename T> 
    inline void fill_n(T* src, size_t n, const T& val) {
        T* last = src + n;
       
        while (src != last) {
            switch (n & 0x7) {
                case 0: *src = val; ++src;
                case 7: *src = val; ++src;
                case 6: *src = val; ++src;
                case 5: *src = val; ++src;
                case 4: *src = val; ++src;
                case 3: *src = val; ++src;
                case 2: *src = val; ++src;
                case 1: *src = val; ++src;
            }
        }
	}

	template<typename TIter, typename TDist> 
    inline void distance(TIter src, TIter last, TDist& dist) {
	        internal::distance(src, last, dist, typename iterator_traits<TIter>::iterator_category());
	}

	template<typename TIter, typename TDist> 
    inline void advance(TIter& iter, TDist off) {
	        internal::advance(iter, off, typename iterator_traits<TIter>::iterator_category());
	}

	template<class TIter, typename T, class TPred> 
    inline TIter lower_bound(TIter src, TIter last, const T& val, const TPred& pred) {
	        internal::test_ordering(src, last, pred);
	        int dist(0);
	        distance(src, last, dist);

	        while (dist > 0) {
                const int halfDist = dist >> 1;
                TIter mid = src;
                advance(mid, halfDist);
                if (internal::debug_pred(pred, *mid, val))
                        src = ++mid, dist -= halfDist + 1;
                else
                        dist = halfDist;
	        }
	        return src;
	}

	template<class TIter, typename T, class TPred> 
    inline TIter upper_bound(TIter src, TIter last, const T& val, const TPred& pred) {
	        internal::test_ordering(src, last, pred);
	        int dist(0);
	        distance(src, last, dist);

	        while (dist > 0) {
                const int halfDist = dist >> 1;
                TIter mid = src;
                advance(mid, halfDist);
                if (!internal::debug_pred(pred, val, *mid))
                    src = ++mid, dist -= halfDist + 1;
                else
                    dist = halfDist;
	        }
	        return src;
	}

	template<class TIter, typename T> 
    TIter find(TIter src, TIter last, const T& val) {
        while (src != last) {
            if ((*src) == val) return src;
            ++src;
        }
        return last;
	}

	template<class TIter, typename T, class TPred> 
    TIter find_if(TIter src, TIter last, const T& val, const TPred& pred) {
        while (src != last) {
            if (pred(*src, val))
                    return src;
            ++src;
        }
        return last;
	}

	template<class TIter, typename T> 
    void accumulate(TIter src, TIter last, T& dest) {
        while (src != last)
        {
            dest += *src;
            ++src;
        }
	}

	template<typename T> 
    T abs(const T& t) {
	    return t >= T(0) ? t : -t;
	}

	inline int abs(int x) {
        const int y = x >> 31;
        return (x ^ y) - y;
	}

	inline short abs(short x) {
        const short y = x >> 15;
        return (x ^ y) - y;
	}

	template<typename T> 
    inline T max(const T& x, const T& y) {
	    return x > y ? x : y;
	}

	template<typename T> 
    inline T min(const T& x, const T& y) {
	    return x < y ? x : y;
	}

	template<typename TAssignable> 
    void swap(TAssignable& a, TAssignable& b) {
        TAssignable tmp(a);
        a = b;
        b = tmp;
	}
}

#endif