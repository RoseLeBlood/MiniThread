/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2020 Amber-Sophia Schroeck
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
* <https://www.gnu.org/licenses/>. -
*/
#ifndef _MINLIB_TYPE_TRAITS_H_
#define _MINLIB_TYPE_TRAITS_H_

#include "mn_config.hpp"
#include "mn_def.hpp"

/**
 * @author Amber-Sophia Schröck
 * @date 2021.02.14
 */
namespace mn {
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

	template <typename T>
	struct type_traits<volatile T&> {
		using value_type = T;
		using const_type = const T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
	};

	template <typename T>
	struct type_traits<const volatile T&> {
		using value_type = T;
		using const_type = const T;
		using reference = T&;
		using const_reference = const T&;
		using pointer = T*;
		using const_pointer = const T*;
	};

	template <typename T>
	struct type_traits<T*> {
		using value_type = T*;
		using const_type = const T*;
		using reference = T*&;
		using const_reference = const T*&;
		using pointer = T*;
		using const_pointer = const T*;
	};

	template <typename T, size_t N>
	struct type_traits<T [N]> {
	private:
		typedef T array_type[N];
	public:
	   	using value_type = T*;
	  	using const_type = const T*;
		using reference = array_type&;
		using const_reference = const array_type&;
		using pointer = T*;
		using const_pointer = const T*;
	};

	template <typename T, size_t N>
	struct type_traits<const T [N]> {
	private:
		typedef const T array_type[N];
	public:
	   	using value_type = const T*;
	  	using const_type = const T*;
		using reference = array_type&;
		using const_reference = const array_type&;
		using pointer = T*;
		using const_pointer = const T*;
	};

    template<class T, T v> struct integral_constant {
        enum { value = v  };
    };

    using  true_type = integral_constant<bool, true>;
    using false_type =  integral_constant<bool, false> ;

    template<typename>
    struct is_const : public false_type { };

    template<typename T>
    struct is_const<T const>  : public true_type { };

	/// is_volatile
	template<typename>
	struct is_volatile : public false_type { };

	template<typename T>
	struct is_volatile<T volatile> : public true_type { };



	/// is_empty
	template<typename T>
	struct is_empty : public integral_constant<bool, __is_empty(T)> { };


    template<typename T>
    struct is_enum : public integral_constant<bool, __is_enum(T)> { };

    template<typename T>
    struct is_union : public integral_constant<bool, __is_union(T)> { };

    template<typename T>
    struct is_class : public integral_constant<bool, __is_class(T)> { };

    template<typename>
    struct is_function;

    template<typename>
    struct is_function : public false_type { };

    template<typename T, typename... Args>
    struct is_function<T(Args...)> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......)> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) volatile> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) volatile &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) volatile &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) volatile> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) volatile &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) volatile &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const volatile> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const volatile &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args...) const volatile &&> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const volatile> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const volatile &> : public true_type { };

	template<typename T, typename... Args>
	struct is_function<T(Args......) const volatile &&> : public true_type { };

    template<typename T>
    struct is_abstract : public integral_constant<bool, __is_abstract(T)> { };

    template<typename T>
    struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)> { };

    template<typename T>
    struct is_literal_type : public integral_constant<bool, __is_literal_type(T)> { };

    template<typename T>
    struct is_pod : public integral_constant<bool, __is_pod(T)> { };

    /// is_trivial
	template<typename T>
	struct is_trivial : public integral_constant<bool, __is_trivial(T)> { };

	// is_trivially_copyable
  	template<typename T>
    struct is_trivially_copyable : public integral_constant<bool, __is_trivially_copyable(T)>  { };

	/// is_standard_layout
	template<typename T>
	struct is_standard_layout : public integral_constant<bool, __is_standard_layout(T)> { };

	template<typename T>
    struct is_bind_expression  : public false_type { };

	template<typename T>
    struct is_placeholder  : public integral_constant<int, 0> { };





	template<typename T>
    struct is_integral : public integral_constant<bool, false> { };

	template<class T>
    struct is_integral<const T> : public is_integral<T> { };

    template<class T>
    struct is_integral<volatile T> : public is_integral<T> { };

    template<class T>
    struct is_integral<const volatile T> : public is_integral<T> { };



    template<typename T>
    struct is_rational : public integral_constant<bool, false> { };

	template<class T>
    struct is_rational<const T> : public is_rational<T> { };

    template<class T>
    struct is_rational<volatile T> : public is_rational<T> { };

    template<class T>
    struct is_rational<const volatile T> : public is_rational<T> { };

    template<typename T>
    struct is_void : public integral_constant<bool, false> { };

    template<class T>
    struct is_void<const T> : public is_void<T> { };

    template<class T>
    struct is_void<volatile T> : public is_void<T> { };

    template<class T>
    struct is_void<const volatile T> : public is_void<T> { };

    template<typename T>
    struct is_floating_point : public integral_constant<bool, false> { };

    template<>
    struct is_floating_point<float> : public integral_constant<bool, true> { };

    template<>
    struct is_floating_point<double> : public integral_constant<bool, true> { };

	template <>
	struct is_floating_point<long double> : public integral_constant<bool, true> { };

	template <typename T>
	struct is_floating_point<const T> : is_floating_point<T> {};

	template <typename T>
	struct is_floating_point<volatile T> : is_floating_point<T> {};

	template <typename T>
	struct is_floating_point<const volatile T> : is_floating_point<T> {};

	 /// is_lvalue_reference
  	template<typename>
    struct is_lvalue_reference : public false_type { };

  	template<typename T>
    struct is_lvalue_reference<T&> : public true_type { };


    /// is_rvalue_reference
  	template<typename>
    struct is_rvalue_reference : public false_type { };

  	template<typename T>
    struct is_rvalue_reference<T&&> : public true_type { };

    template<typename T>
    struct is_pointer
    	: public integral_constant<bool, false> { };

    template<typename T>
    struct is_pointer<T*>
    	: public integral_constant<bool, true> { };

    template<typename T>
    struct is_fundamental
    	: public integral_constant<bool, is_integral<T>::value || is_rational<T>::value> { };

    template<typename T>
    struct has_trivial_constructor
    	: public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T>
    struct has_trivial_copy
    	: public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T>
    struct has_trivial_assign
    	: public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T>
    struct has_trivial_destructor
    	: public integral_constant<bool, is_fundamental<T>::value || is_pointer<T>::value || is_pod<T>::value> { };

	template<typename T>
	struct is_reference
		: public integral_constant<bool, is_lvalue_reference<T>::value | is_rvalue_reference<T>::value> { };

	/// is_arithmetic
	template<typename T>
	struct is_arithmetic
		: public integral_constant< bool, !(is_integral<T>::value | is_floating_point<T>::value) > { };

	/// is_object
	template<typename T>
	struct is_object
		: public integral_constant< bool, ! (is_function<T>::value |  is_reference<T>::value |
											is_void<T>::value) > { };

	/// is_signed
	template <typename T>
	struct is_signed : false_type {};

	template <>
	struct is_signed<char> : integral_constant<bool, (char(255) < 0)> {};

	template <>
	struct is_signed<signed char> : true_type {};

	template <>
	struct is_signed<short> : true_type {};

	template <>
	struct is_signed<int> : true_type {};

	template <>
	struct is_signed<long> : true_type {};

	template <>
	struct is_signed<long long> : true_type {};

	template <>
	struct is_signed<float> : true_type {};

	template <>
	struct is_signed<double> : true_type {};

	template <>
	struct is_signed<long double> : true_type {};

	template <typename T>
	struct is_signed<const T> : is_signed<T> {};

	template <typename T>
	struct is_signed<volatile T> : is_signed<T> {};

	template <typename T>
	struct is_signed<const volatile T> : is_signed<T> {};


	/// is_unsigned
	template <typename T>
	struct is_unsigned : false_type {};

	template <>
	struct is_unsigned<bool> : true_type {};

	template <>
	struct is_unsigned<char> : integral_constant<bool, (char(255) > 0)> {};

	template <>
	struct is_unsigned<unsigned char> : true_type {};

	template <>
	struct is_unsigned<unsigned short> : true_type {};

	template <>
	struct is_unsigned<unsigned int> : true_type {};

	template <>
	struct is_unsigned<unsigned long> : true_type {};

	template <>
	struct is_unsigned<unsigned long long> : true_type {};

	template <typename T>
	struct is_unsigned<const T> : is_unsigned<T> {};

	template <typename T>
	struct is_unsigned<volatile T> : is_unsigned<T> {};

	template <typename T>
	struct is_unsigned<const volatile T> : is_unsigned<T> {};

	template<typename>
	struct is_member_pointer;



	/// is_compound
	template<typename T>
	struct is_compound
	: public integral_constant<bool, !is_fundamental<T>::value> { };


    template<typename T>
    struct has_cheap_compare : public integral_constant<bool, has_trivial_copy<T>::value && sizeof(T) <= 4 > { };


    template<size_t size, size_t align = alignof(void *)>
	struct aligned_storage {
		struct type { alignas(align) unsigned char data[size]; };
	};

    template<size_t size, size_t align = alignof(void *)>
    using aligned_storage_t = typename aligned_storage<size, align>::type;



    #define MN_INTEGRAL(T) template<> \
    struct is_integral<T> : public integral_constant<bool, true> { };

    #define MN_RATIONAL(T) template<> \
    struct is_rational<T> : public integral_constant<bool, true> { };

    #define MN_VOIDTYPE(T) template<> \
    struct is_void<T> : public integral_constant<bool, true> { };

	MN_INTEGRAL(char);
	MN_INTEGRAL(unsigned char);
	MN_INTEGRAL(short);
	MN_INTEGRAL(unsigned short);
	MN_INTEGRAL(int);
	MN_INTEGRAL(unsigned int);
	MN_INTEGRAL(long);
	MN_INTEGRAL(unsigned long);
	MN_INTEGRAL(wchar_t);
    MN_INTEGRAL(bool);

    MN_VOIDTYPE(void);
    MN_VOIDTYPE(const void);
    MN_VOIDTYPE(volatile void );
    MN_VOIDTYPE(const volatile void );

    MN_RATIONAL(float);
    MN_RATIONAL(double);
    MN_RATIONAL(long double);
}


#endif
