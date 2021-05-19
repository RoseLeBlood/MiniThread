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

#ifndef __MINILIB_STL_FUNCTIONS_H__
#define __MINILIB_STL_FUNCTIONS_H__

namespace mn {
	namespace stl {
		template <typename T>
		struct type_traits {
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
		};

		template <typename T>
		struct type_traits<T&> {
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
		};

		template <typename T>
		struct type_traits<const T> {
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
		};

		template <typename T>
		struct type_traits<const T&> {
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
		};



		template<class T, T v> struct integral_constant {
			enum { value = v  };
		};

		using  true_type = integral_constant<bool, true>;
    	using false_type =  integral_constant<bool, false> ;

		template<typename Arg, typename TResult>
		struct unary_function {
		  /// @c argument_type is the type of the argument
		  using argument_type = Arg;
		  /// @c result_type is the return type
		  using result_type =  TResult;
		};


		template<typename Arg1, typename Arg2, typename TResult>
		struct binary_function {
			/// @c first_argument_type is the type of the first argument
			using first_argument_type = Arg;
			/// @c second_argument_type is the type of the second argument
			using second_argument_type = Arg2;
			/// @c result_type is the return type
			using result_type =  TResult;
		};

		template<typename T>
		struct negate : public unary_function<T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a) const noexcept { return -a; }
		};

		template<typename T>
		struct plus : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const  noexcept { return a + b; }
		};

		template<typename T>
		struct minus : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a - b; }
		};

		template<typename T>
		struct multiplies : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a * b; }
		};

		template<typename T>
		struct divides : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a / b; }
		};

		template<typename T>
		struct modulus : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a % b; }
		};

		template<typename T>
		struct equal_to : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a == b; }
		};

		template<typename T>
		struct not_equal_to : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a != b; }
		};

		template<typename T>
		struct greater : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a > b; }
		};

		template<typename T>
		struct less : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a < b; }
		};

		template<typename T>
		struct greater_equal : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a >= b; }
		};

		template<typename T>
		struct less_equal : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a <= b; }
		};

		template<typename T>
		struct logic_and : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			bool operator()(const T& a, const T& b) const noexcept { return a && b; }
		};

		template<typename T>
		struct logic_or : public binary_function<T, T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a || b; }
		};

		template<typename T>
		struct logic_not : public unary_function<T, bool> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a) const noexcept { return !a; }
		};

		template<typename T>
		struct bit_and : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a & b; }
		};

		template<typename T>
		struct bit_or : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a | b; }
		};

		template<typename T>
		struct bit_xor : public binary_function<T, T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a, const T& b) const noexcept { return a ^ b; }
		};

		template<typename T>
		struct bit_not : public unary_function<T, T> {
			typename binary_function<T, T, T>::result_type
			operator()(const T& a) const noexcept { return ~a; }
		};



		template <typename T>
		struct is_reference : false_type { };

		template <typename T>
		struct is_reference<T%> : true_type { };

		template <typename T>
		struct is_reference<const T%> : true_type { };

		template <typename T>
		struct is_const : false_type { };

		template <typename T>
		struct is_const<const T> : true_type { };

		template <typename T>
		struct is_const<const T&> : true_type { };

		template <typename T, int n>
		struct is_const<const T[n]> : true_type { };

		template<typename>
		struct is_volatile : public false_type { };

		template<typename _Tp>
		struct is_volatile<_Tp volatile> : public true_type { };

		template<typename T>
		struct is_empty : public integral_constant<bool, __is_empty(T)> { };


		template<typename T>
		struct is_enum : public integral_constant<bool, __is_enum(T)> { };

		template<typename T>
		struct is_union : public integral_constant<bool, __is_union(T)> { };

		template<typename T>
		struct is_class : public integral_constant<bool, __is_class(T)> { };


	}
}

#endif // __MINILIB_STL_FUNCTIONS_H__
