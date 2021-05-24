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

#ifndef _MINLIB_423a6056_5421_4f39_85b0_41c5bada4730_H_
#define _MINLIB_423a6056_5421_4f39_85b0_41c5bada4730_H_

#include "mn_config.hpp"
#include "mn_defines.hpp"
#include "mn_def.hpp"
#include "mn_typetraits.hpp"


using nullptr_t = decltype(nullptr);

namespace mn {
    template <typename T> struct tag { using type = T; };
    template <typename Tag> using type_t = typename Tag::type;

    template< bool B, typename T, typename F > struct conditional              { using type = T; };
    template<         typename T, typename F > struct conditional<false, T, F> { using type = F; };



    template<typename...>
    struct bit_or;

	template<>
	struct bit_or<> : public false_type { };

	template<typename A>
	struct bit_or<A>
		: public A { };

	template<typename A, typename B>
	struct bit_or<A, B>
		: public conditional<A::value, A, B>::type { };

	template<typename A, typename B, typename C, typename... N>
	struct bit_or<A, B, C, N...>
		: public conditional<A::value, B, bit_or<B, C, N...>>::type { };


    template <typename T> T &&move(T &t) {
        return static_cast<T &&>(t); }

    template <typename T>
    struct remove_reference
        : tag<T> { };

    template <typename T>
    struct remove_reference<T &>
        : tag<T> {  };

    template <typename T>
    using remove_reference_t = type_t<remove_reference<T>>;

    template <typename T>
    T &&forward(remove_reference_t<T> &t) {
        return static_cast<T &&>(t);
    }
    template <typename T>
    T &&forward(remove_reference_t<T> &&t) {
        return static_cast<T &&>(t);
    }

    template <typename T>
    struct remove_const
        : tag<T> { };

    template <typename T>
    struct remove_const<T const>
        : tag<T> {};

    template <typename T>
    struct remove_volatile
        : tag<T> {};

    template <typename T>
    struct remove_volatile<T volatile>
        : tag<T> {};

    template <typename T>
    struct remove_cv
        : remove_const<type_t<remove_volatile<T>>> { };

	template< typename T >
	struct remove_cvref
		: remove_cv<type_t<remove_reference<T>>> { };

	template< class T >
	struct add_const
		: tag<const T> {};

	template< class T >
	struct add_volatile
		: tag<volatile T> {};

	template< class T >
	struct add_cv {
	 	using type = const volatile typename remove_reference<T>::type;
	};

	template <typename T>
    struct add_reference
        { using type = typename remove_reference<T>::type&;  };

	template< class T >
	struct add_pointer {
	 	using type = typename remove_reference<T>::type*;
	};

	template<class T>
	struct add_lvalue_reference
		: tag<T&> { };

	template<class T>
	struct add_rvalue_reference
		: tag<T&&> { };

    template <typename T>
    struct decay3 : remove_cv<T> {};

    template <typename R, typename... Args>
    struct decay3<R(Args...)> : tag<R (*)(Args...)> {};

    template <typename T>
    struct decay2 : decay3<T> {};

    template <typename T, size_t N>
    struct decay2<T[N]> : tag<T *> {};

    template <typename T>
    struct decay : decay2<remove_reference_t<T>> {};

    template <typename T>
    using decay_t = type_t<decay<T>>;


    template <typename T> T declval();


    namespace internal {
        template <template <class...> typename Z, typename, typename... Ts>
        struct can_apply : false_type {};

        template <template <class...> typename Z, typename... Ts>
        struct can_apply<Z, void_t<Z<Ts...>>, Ts...> : true_type {};

        template <typename From, typename To>
        using try_convert = decltype(To{declval<From>()});

        template <class G, class... Args>
        using invoke_t = decltype(declval<G>()(declval<Args>()...));

        template <class Sig, class = void>
        struct res_of {};
        template <class G, class... Args>
        struct res_of<G(Args...), void_t<invoke_t<G, Args...>>> : tag<invoke_t<G, Args...>> {};

    }

    template <template <typename...> typename Z, typename... Ts>
    using can_apply = internal::can_apply<Z, void, Ts...>;


    template <typename From, typename To>
    struct is_convertible : internal::can_apply<internal::try_convert, From, To> {};

    template <> struct is_convertible<void, void> : true_type {};

    template <bool, class = void>
    struct enable_if {};

    template <class T>
    struct enable_if<true, T> : tag<T> {};

    template <bool b, class T = void>
    using enable_if_t = type_t<enable_if<b, T>>;


    template <class Sig>
    using res_of = internal::res_of<Sig>;

    template <class Sig>
    using res_of_t = type_t<res_of<Sig>>;


    // is_same
    template <class A, class B>
    struct is_same : false_type {};

    template <class A>
    struct is_same<A, A> : true_type {};

    template<typename TBase, typename TDerived>
    struct is_base_of : public integral_constant<bool, __is_base_of(TBase, TDerived)> { };


    template<typename T>
	struct logic_and {
		bool operator()(const T& a, const T& b) const noexcept { return a && b; }
	};

	template<typename T>
	struct logic_or {
		bool operator()(const T& a, const T& b) const noexcept { return a || b; }
	};

	template<typename T>
	struct logic_not {
		bool operator()(const T& a) const noexcept { return !a; }
	};

	template <size_t VAL, size_t X = 1>
	struct sqrt{
		using type = typename mn::conditional<
			((X * X) > VAL), mn::integral_constant<intmax_t, X - 1>, mn::sqrt<VAL, X + 1> >::type;
		static constexpr size_t value = type::value;
	};

	/**
	 * @brief Calculates the Nth factorial value.
	 * @tparam N The number to find the factorial value of.
	 */
	template <size_t N>
	struct factorial {
		static constexpr size_t value = N * factorial<N - 1>::value;
	};
	/**
	 * @brief Calculates the 0 factorial value.
	 */
	template <>
	struct factorial<0> {
		static constexpr size_t value = 1;
	};

	/**
	 * @brief Calculates the Nth Fibonacci value.
	 * @tparam N The number to find the Fibbonacci value of.
	 */
	template <size_t N>
	struct fibonacci {
		static constexpr size_t value = fibonacci<N - 1>::value + fibonacci<N - 2>::value;
	};
	/**
	 * @brief Calculates the 0 Fibonacci value.
	 */
	template <>
	struct fibonacci<0> {
		static constexpr size_t value = 0;
	};
	/**
	 * @brief Calculates the 1 Fibonacci value.
	 */
	template <>
	struct fibonacci<1> {
		static constexpr size_t value = 1;
	};
}

#endif
