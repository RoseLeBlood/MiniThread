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

#include <stdalign.h>


#include "mn_nlz.hpp"
#include "../mn_typetraits.hpp"
#include "../mn_def.hpp"

namespace mn {
    namespace internal {

        template<typename T>
	    struct alignof_helper {
	        char    x;
	        T 		y;
	    };
	    struct __attribute__ ((aligned (0)))  aligned0 	{  uint8_t member : 4; };
	    struct __attribute__ ((aligned (1)))  aligned1 	{  uint8_t member; };
		struct __attribute__ ((aligned (2)))  aligned2 	{ uint16_t member; };
		struct __attribute__ ((aligned (4)))  aligned4 	{ uint32_t member; };
		struct __attribute__ ((aligned (8)))  aligned8 	{ uint64_t member; } ;
		struct __attribute__ ((aligned (16))) aligned16 { uint64_t member[2]; } ;
        struct __attribute__ ((aligned (32))) aligned32 { uint64_t member[4]; } ;
		struct __attribute__ ((aligned (64))) aligned64 { uint64_t member[8]; } ;



        template<size_t N> struct type_with_alignment {
	        typedef char err_invalid_alignment[N > 0 ? -1 : 1];
	    };
        template<> struct type_with_alignment<0> { aligned0 type; };
        template<> struct type_with_alignment<1> { aligned1 type; };
        template<> struct type_with_alignment<2> { aligned2 type; };
        template<> struct type_with_alignment<4> { aligned4 type; };
        template<> struct type_with_alignment<8> { aligned8 type; };
        template<> struct type_with_alignment<16> { aligned16 type; };
        template<> struct type_with_alignment<32> { aligned32 type; };
        template<> struct type_with_alignment<64> { aligned64 type; };

        union max_align {
			char c;
			short s;
			int i;
			long l;
			long long ll;
			float f;
			double d;
        };
    }

    template<> struct is_pod<internal::aligned0> : public true_type{};
    template<> struct is_pod<internal::aligned1> : public true_type{};
	template<> struct is_pod<internal::aligned2> : public true_type{};
	template<> struct is_pod<internal::aligned4> : public true_type{};
	template<> struct is_pod<internal::aligned8> : public true_type{};
	template<> struct is_pod<internal::aligned16> : public true_type{};
	template<> struct is_pod<internal::aligned32> : public true_type{};
	template<> struct is_pod<internal::aligned64> : public true_type{};


    template<typename T>
	struct alignment_of {
	    enum { res = offsetof(internal::alignof_helper<T>, y) };
	};
	template<> struct alignment_of<void> {
		enum { res = 0  };
	};

	template<> struct alignment_of<void const> {
		enum { res = 0  };
	};

	template<> struct alignment_of<void const volatile> {
		enum { res = 0  };
	};

	template<> struct alignment_of<void volatile> {
		enum { res = 0  };
	};

	template<size_t Len, size_t Align>
	struct aligned_storage {
		struct type { alignas(Align) unsigned char data[Len]; };
	};

	template <size_t Target, bool check>  struct long_double_alignment{ using type = long double; };
	template <size_t Target> struct long_double_alignment<Target, false> {
		using type = internal::max_align; };

	template <size_t Target, bool check> struct double_alignment{ using type = double; };
    template <size_t Target> struct double_alignment<Target, false> {
    	using type = typename long_double_alignment<Target, alignment_of<long double>::res >= Target>::type; };

	template <size_t Target, bool check> struct long_long_alignment{  using type = long long; };
	template <size_t Target> struct long_long_alignment<Target, false> {
    	using type = typename  double_alignment<Target, alignment_of<double>::res >= Target>::type; };

	template <size_t Target, bool check> struct long_alignment{ using type = long; };
	template <size_t Target> struct long_alignment<Target, false> {
		using type = typename long_long_alignment<Target, alignment_of<long long>::res >= Target>::type; };

	template <size_t Target, bool check> struct int_alignment{ using type = int; };
	template <size_t Target> struct int_alignment<Target, false>{
		using type = typename long_alignment<Target, alignment_of<long>::res >= Target>::type; };

	template <size_t Target, bool check> struct short_alignment{ using type = short; };
	template <size_t Target> struct short_alignment<Target, false>{
		using type = typename int_alignment<Target, alignment_of<int>::res >= Target>::type; };

	template <size_t Target, bool check> struct char_alignment{ using type = char; };
	template <size_t Target> struct char_alignment<Target, false>{
		using type = typename short_alignment<Target, alignment_of<short>::res >= Target>::type; };

    template<typename T>
	struct aligned_as {
	    using res = typename internal::type_with_alignment<alignment_of<T>::res>;
	};

	template <class integral>
	constexpr bool is_aligned(integral x, size_t a) noexcept {
		return (x & (integral(a) - 1)) == 0u;
	}
	inline bool is_aligned(const volatile void* p, size_t a) {
	  return is_aligned(reinterpret_cast<uintptr_t>(p), a);
	}

	template <class integral>
	constexpr integral align_up(integral x, size_t a) noexcept {
		return integral((x + (integral(a) - 1)) & ~integral(a-1));
	}
	template <class pointer>
	pointer align_up_ptr(pointer p, size_t a) {
	  	return reinterpret_cast<pointer>(align_up(reinterpret_cast<uintptr_t>(p), a));
	}

	template <class integral>
	constexpr integral align_down(integral x, size_t a) noexcept {
		return integral(x & ~integral(a-1));
	}
	template <class pointer>
	pointer align_down_ptr(pointer p, size_t a) {
	  	return reinterpret_cast<pointer>(align_down(reinterpret_cast<uintptr_t>(p), a));
	}


	/**
	 * @brief is a valid alignment, i.e. a power of two not zero
	 */
	constexpr bool is_aligvalid(size_t alignment) noexcept {
		return alignment && (alignment & (alignment - 1)) == 0u;
	}

	// returns the offset needed to align ptr for given alignment
	inline size_t alig_offset(void* address, size_t alignment) noexcept {
		if(!is_aligvalid(alignment)) return 0;

		auto _isaligned = reinterpret_cast<uintptr_t>(address) & (alignment - 1);
		return _isaligned != 0 ? (alignment - _isaligned) : 0;
	}



	inline size_t alignment_for(const size_t size) noexcept {
		if(size >= max_alignment) return max_alignment;

		return (size_t(1) << nlz(size) );
	}
}


#endif
