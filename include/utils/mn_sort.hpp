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

#ifndef MINLIB_STL_SORT_H_
#define MINLIB_STL_SORT_H_

#include "mn_utils.hpp"


namespace mn {
    namespace internal {

        MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TPredicate)
        void quick_sort(T* data, long low, long high, TPredicate pred) {
			while (true) {
				long i = low;
				long j = high;
				const T pivot = data[(low + high) >> 1];

				do {
					while (pred(data[i], pivot)) ++i;
					while (pred(pivot, data[j])) --j;

					// Anything to swap?
					if (j >= i) {
						if (i != j) {
					    	// Swap
					    	T tmp(data[i]);
					    	data[i] = data[j];
					    	data[j] = tmp;
						}
					    ++i;
					    --j;
					}
				} while (i <= j);

                if (low < j) quick_sort(data, low, j, pred);
                if (i < high) low = i;    
                else break;
			} 
		}

		MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TPredicate)
        void down_heap(T* data, size_t k, size_t n, TPredicate pred) {
			const T temp = data[k - 1];

			while (k <= n / 2) {
				size_t child = 2 * k;
				if (child < n && pred(data[child - 1], data[child]))
					++child;
				if (pred(temp, data[child - 1])) {
					data[k - 1] = data[child - 1];
					k = child;
				} else break;
			}
			data[k - 1] = temp;
		}
	} // internal

	MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TPredicate)
    void insertion_sort(T* begin, T* end, TPredicate pred) {
		const size_t num = end - begin; 

		for (size_t i = 0; i < num; ++i) {
			const T t = begin[i];
			size_t j = i;

			while (j > 0 && pred(t, begin[j - 1])) {
				begin[j] = begin[j - 1];
				--j;
			}
			begin[j] = t;
		}
	}
	MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    void insertion_sort(T* begin, T* end) {
		insertion_sort(begin, end, less<T>());
	}

	MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TPredicate)
    void quick_sort(T* begin, T* end, TPredicate pred) {
		if (end - begin > 1)
			internal::quick_sort(begin, 0, (long)(end - begin - 1), pred);
	}

	MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    void quick_sort(T* begin, T* end) {
		quick_sort(begin, end, less<T>());
	}

	MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TPredicate)
    void heap_sort(T* begin, T* end, TPredicate pred) {

		size_t n = end - begin;
		for (size_t k = n / 2; k != 0; --k)
			internal::down_heap(begin, k, n, pred);

		while (n >= 1) {
			const T temp = begin[0];
			begin[0] = begin[n - 1];
			begin[n - 1] = temp;

			--n;
			internal::down_heap(begin, 1, n, pred);
		}
	}

    MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    void heap_sort(T* begin, T* end) {
		heap_sort(begin, end, mn::less<T>());
	}

    MN_TEMPLATE_FULL_DECL_TWO(typename, TIter, typename, TPredicate)
    bool is_sorted(TIter begin, TIter end, TPredicate pred) {
		TIter it = begin;
		TIter it_prev = it;
		bool is_sorted = true;

		while (it != end) {
			if (it_prev != it) {
				if (pred(*it, *it_prev)) {
					is_sorted = false;
					break;
				}
			}
			it_prev = it;
			++it;
		}
		return is_sorted;
	}
}

#endif