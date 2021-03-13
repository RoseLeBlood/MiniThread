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
#ifndef _MINLIB_5ac13f75_2f13_4bb7_acae_7b8f27f78e20_H_
#define _MINLIB_5ac13f75_2f13_4bb7_acae_7b8f27f78e20_H_

#if defined(__GNUC__) || defined(__clang__) || defined(__xlc__)
    #include "mn_atomic_gcc.hpp"
    #define BASE_ATOMIC_TYPE basic_atomic_gcc
    #define BASE_ATOMIC_SMART_POINTER basic_smart_pointer_atomic_gcc

    #define	ATOMIC_VAR_INIT(value)		{ .__tValue = (value) }
    #define	atomic_init(obj, value)		((void)((obj)->__tValue = (value)))
    #define ATOMIC_FLAG_INIT            { 0 }
#else
#error Unsupported compiler / system.
#endif

namespace mn {
     /**
     * @brief Only for declar special version for types
     */
    template <typename T>
    struct _atomic : BASE_ATOMIC_TYPE<T> { 
        using base_type = BASE_ATOMIC_TYPE<T>;
        using self_type = _atomic<T>;
        using value_type = typename base_type::value_type;
        using difference_type =  typename base_type::value_type;
    };


     /**
     * @brief Special version for bool
     */
    template<>
    struct _atomic<bool> : BASE_ATOMIC_TYPE<bool>  {
        static constexpr bool is_always_lock_free  = ATOMIC_BOOL_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<bool> ;
        using self_type = _atomic<bool>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for char
     */
    template<>
    struct _atomic<char> : BASE_ATOMIC_TYPE<char>  {
        static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<char> ;
        using self_type = _atomic<char>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

     /**
     * @brief Special version for signed char
     */
    template<>
    struct _atomic<signed char> : BASE_ATOMIC_TYPE<signed char>  {
        static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<signed char> ;
        using self_type = _atomic<signed char>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for short
     */
    template<>
    struct _atomic<short> : BASE_ATOMIC_TYPE<short>  {
        static constexpr bool is_always_lock_free  = ATOMIC_SHORT_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<short> ;
        using self_type = _atomic<short>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for int
     */
    template<>
    struct _atomic<int> : BASE_ATOMIC_TYPE<int>  {
        static constexpr bool is_always_lock_free  = ATOMIC_INT_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<int> ;
        using self_type = _atomic<int>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for long
     */
    template<>
    struct _atomic<long> : BASE_ATOMIC_TYPE<long>  {
        static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<long> ;
        using self_type = _atomic<long>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for long long
     */
    template<>
    struct _atomic<long long> : BASE_ATOMIC_TYPE<long long>  {
        static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<long long> ;
        using self_type = _atomic<long long>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    //----------------
    /**
     * @brief Special version for unsigned char
     */
    template<>
    struct _atomic<unsigned char> : BASE_ATOMIC_TYPE<unsigned char>  {
        static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<unsigned char> ;
        using self_type = _atomic<unsigned char>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value) : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for short
     */
    template<>
    struct _atomic<unsigned short> : BASE_ATOMIC_TYPE<unsigned short>  {
        static constexpr bool is_always_lock_free  = ATOMIC_SHORT_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<unsigned short> ;
        using self_type = _atomic<unsigned short>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for int
     */
    template<>
    struct _atomic<unsigned int> : BASE_ATOMIC_TYPE<unsigned int>  {
        static constexpr bool is_always_lock_free  = ATOMIC_INT_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<unsigned int> ;
        using self_type = _atomic<unsigned int>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for long
     */
    template<>
    struct _atomic<unsigned long> : BASE_ATOMIC_TYPE<unsigned long>  {
        static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<unsigned long> ;
        using self_type = _atomic<unsigned long>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for char16_t
     */
    template<>
    struct _atomic<char16_t> : BASE_ATOMIC_TYPE<char16_t>  {
        static constexpr bool is_always_lock_free  = ATOMIC_CHAR16_T_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<char16_t> ;
        using self_type = _atomic<char16_t>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for char32_t
     */
    template<>
    struct _atomic<char32_t> : BASE_ATOMIC_TYPE<char32_t>  {
        static constexpr bool is_always_lock_free  = ATOMIC_CHAR32_T_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<char32_t> ;
        using self_type = _atomic<char32_t>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for wchar_t
     */
    template<>
    struct _atomic<wchar_t> : BASE_ATOMIC_TYPE<wchar_t>  {
        static constexpr bool is_always_lock_free  = ATOMIC_WCHAR_T_LOCK_FREE == 2;

        using base_type = BASE_ATOMIC_TYPE<wchar_t> ;
        using self_type = _atomic<wchar_t>;
        using value_type = typename base_type::value_type;
        using difference_type = typename base_type::difference_type;

        _atomic(const self_type&) = delete;
        _atomic& operator=(const self_type&) = delete;
        _atomic& operator=(const self_type&) volatile = delete;

        constexpr _atomic(value_type value)  : base_type(value) { }

        using base_type::operator value_type;
        using base_type::operator=;
    };

    /**
     * @brief Special version for pointer
     */
    template<typename T>
    struct _atomic_ptr : BASE_ATOMIC_TYPE<T*>  {
        static constexpr bool is_always_lock_free  = ATOMIC_POINTER_LOCK_FREE == 2;

        using pointer = T*;
        using base_type = BASE_ATOMIC_TYPE<pointer> ;
        using self_type = _atomic<pointer>;
        using difference_type = mn::ptrdiff_t;

        _atomic_ptr() = default;
        ~_atomic_ptr() = default;

        constexpr _atomic_ptr(pointer value)  : base_type(value) { }

        _atomic_ptr(const self_type&) = delete;
        _atomic_ptr& operator=(const self_type&) = delete;
        _atomic_ptr& operator=(const self_type&) volatile = delete;

        using base_type::operator pointer;
        using base_type::operator=;

    };
}

#endif
