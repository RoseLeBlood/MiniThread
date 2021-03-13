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
#ifndef _MINLIB_76a34886_76df_4d18_a155_85d3730cbe17_H_
#define _MINLIB_76a34886_76df_4d18_a155_85d3730cbe17_H_

#include "../mn_def.hpp"

#ifdef __GCC_ATOMIC_BOOL_LOCK_FREE
#define	ATOMIC_BOOL_LOCK_FREE		__GCC_ATOMIC_BOOL_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_CHAR_LOCK_FREE
#define	ATOMIC_CHAR_LOCK_FREE		__GCC_ATOMIC_CHAR_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_CHAR16_T_LOCK_FREE
#define	ATOMIC_CHAR16_T_LOCK_FREE	__GCC_ATOMIC_CHAR16_T_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_CHAR32_T_LOCK_FREE
#define	ATOMIC_CHAR32_T_LOCK_FREE	__GCC_ATOMIC_CHAR32_T_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_WCHAR_T_LOCK_FREE
#define	ATOMIC_WCHAR_T_LOCK_FREE	__GCC_ATOMIC_WCHAR_T_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_SHORT_LOCK_FREE
#define	ATOMIC_SHORT_LOCK_FREE		__GCC_ATOMIC_SHORT_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_INT_LOCK_FREE
#define	ATOMIC_INT_LOCK_FREE		__GCC_ATOMIC_INT_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_LONG_LOCK_FREE
#define	ATOMIC_LONG_LOCK_FREE		__GCC_ATOMIC_LONG_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_LLONG_LOCK_FREE
#define	ATOMIC_LLONG_LOCK_FREE		__GCC_ATOMIC_LLONG_LOCK_FREE
#endif
#ifdef __GCC_ATOMIC_POINTER_LOCK_FREE
#define	ATOMIC_POINTER_LOCK_FREE	__GCC_ATOMIC_POINTER_LOCK_FREE
#endif

namespace mn {
    /**
     *  @brief Generic atomic type, primary class template.
     *  @tparam T  Type to be made atomic, must be trivally copyable.
     */
    template <typename T >
    struct basic_atomic_gcc {
        static_assert(__is_trivially_copyable(T),"mn::atomic requires a trivially copyable type");
        static_assert(sizeof(T) > 0, "Incomplete or zero-sized types are not supported");

        static constexpr bool is_always_lock_free  = __atomic_always_lock_free(sizeof(T), 0);

        using value_type = T;
        using difference_type = T;
        using self_type = basic_atomic_gcc<value_type>;

         basic_atomic_gcc()  = default;
        ~basic_atomic_gcc()  = default;

        basic_atomic_gcc(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) volatile = delete;

        constexpr basic_atomic_gcc(value_type value)  : __tValue(value) { }

        value_type get() { return __tValue; }

        void store (value_type v, memory_order order = memory_order::SeqCst)
            { __atomic_store_n (&__tValue, v, order); }

        value_type load (memory_order order = memory_order::SeqCst) const
            { return __atomic_load_n (&__tValue, order); }

        value_type exchange (value_type v, memory_order order = memory_order::SeqCst)
            { return __atomic_exchange_n (&__tValue, v, order); }

        bool compare_exchange_n (value_type& expected, value_type& desired, bool b, memory_order order = memory_order::SeqCst)
            { return __atomic_compare_exchange_n (&__tValue, &expected, desired, b, order, order); }

        bool compare_exchange_t (value_type& expected, value_type& desired, memory_order order = memory_order::SeqCst)
            { return compare_exchange_n (&expected, desired, true, order); }

        bool compare_exchange_f (value_type& expected, value_type& desired, memory_order order = memory_order::SeqCst)
            { return compare_exchange_n (&expected, desired, false, order); }


        bool compare_exchange_strong(value_type& expected, value_type& desired, memory_order order = memory_order::SeqCst)
            { return _atomic_compare_exchange (&__tValue, &expected, &desired, 0, order, order); }

        bool compare_exchange_weak(value_type& expected, value_type& desired, memory_order order = memory_order::SeqCst)
            { return _atomic_compare_exchange (&__tValue, &expected, &desired, 1, order, order); }

        value_type fetch_add (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_add (&__tValue, v, order); }

        value_type fetch_sub (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_sub (&__tValue, v, order); }

        value_type fetch_and (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_and (&__tValue, v, order); }

        value_type fetch_or (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_or (&__tValue, v, order); }

        value_type fetch_xor (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_xor (&__tValue, v, order); }

        value_type add_fetch (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_add_fetch (&__tValue, v, order); }

        value_type sub_fetch (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_sub_fetch (&__tValue, v, order); }

        value_type and_fetch (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_and_fetch (&__tValue, v, order); }

        value_type or_fetch (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_or_fetch (&__tValue, v, order); }

        value_type xor_fetch (value_type v, memory_order order = memory_order::SeqCst )
            { return __atomic_xor_fetch (&__tValue, v, order); }

        bool is_lock_free() const
            { return __atomic_is_lock_free (sizeof(value_type), &__tValue); }

        bool is_lock_free() const volatile
            { return __atomic_is_lock_free (sizeof(value_type), &__tValue); }

        inline operator value_type() const	         { return load(); }
        inline operator value_type() const volatile  { return load(); }

        inline value_type operator ++ (int)          { return add_fetch (1); }
        inline value_type operator -- (int)          { return sub_fetch (1); }
        inline value_type operator ++ ()             { return add_fetch (1); }
        inline value_type operator -- ()             { return sub_fetch (1); }

        inline value_type operator ++ (int) volatile { return add_fetch (1); }
        inline value_type operator -- (int) volatile { return sub_fetch (1); }
        inline value_type operator ++ ()    volatile { return add_fetch (1); }
        inline value_type operator -- ()    volatile { return sub_fetch (1); }


        inline value_type operator += (value_type v) { return add_fetch (v); }
        inline value_type operator -= (value_type v) { return sub_fetch (v); }
        inline value_type operator &= (value_type v) { return and_fetch (v); }
        inline value_type operator |= (value_type v) { return  or_fetch (v); }
        inline value_type operator ^= (value_type v) { return xor_fetch (v); }

        inline value_type operator  = (value_type v) { store(v); return v; }
        inline value_type operator  = (value_type v) volatile { store(v); return v; }

        volatile value_type __tValue;
    };
}

#endif