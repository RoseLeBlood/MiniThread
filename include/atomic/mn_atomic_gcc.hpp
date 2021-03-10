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

#define ATOMIC_FLAG_INIT { 0 }

namespace mn {
    namespace modifier {

        enum  basic_atomic_gcc_memory_order_modifier  {
            Mask          = 0x0ffff,
            ModifierMask  = 0xffff0000,
            HleAcquire    = 0x10000,
            HleRelease    = 0x20000
        };

        using memory_order = basic_atomic_gcc_memory_order_modifier;
    }

    enum class basic_atomic_gcc_memory_order : int {
        Relaxed = __ATOMIC_RELAXED,
        Consume = __ATOMIC_CONSUME,
        Acquire = __ATOMIC_ACQUIRE,
        Release = __ATOMIC_RELEASE,
        AcqRel = __ATOMIC_ACQ_REL,
        SeqCst = __ATOMIC_SEQ_CST
    };
    using memory_order = basic_atomic_gcc_memory_order;


    inline  memory_order operator | (memory_order mMemOrder, modifier::memory_order mod) {
        return memory_order(int(mMemOrder) | int(mod));
    }

    inline  memory_order operator & (memory_order mMemOrder, modifier::memory_order mod) {
        return memory_order(int(mMemOrder) & int(mod));
    }

    struct basic_atomic_gcc_flag  {
    #if __GCC_ATOMIC_TEST_AND_SET_TRUEVAL == 1
        using flag_type =  bool;
    #else
        using flag_type =  unsigned char;
    #endif

        basic_atomic_gcc_flag() noexcept = default;
        ~basic_atomic_gcc_flag() noexcept = default;

        constexpr basic_atomic_gcc_flag(flag_type flag) 
            : m_iFlag(flag ? __GCC_ATOMIC_TEST_AND_SET_TRUEVAL : 0) { }
    
        bool test_and_set(memory_order order = memory_order::SeqCst) {
            return __atomic_test_and_set (&m_iFlag, int(order) );
        }
        bool test_and_set(memory_order order = memory_order::SeqCst) volatile  {
            return __atomic_test_and_set (&m_iFlag, int(order) );
        }

        void clear(memory_order order = memory_order::SeqCst) {
            __atomic_clear (&m_iFlag, int(order));
        }

        void clear(memory_order order = memory_order::SeqCst) volatile {
            __atomic_clear (&m_iFlag, int(order));
        }

        bool test(memory_order order = memory_order::SeqCst) const {
            flag_type _value;
            __atomic_load(&m_iFlag, &_value, int(order));

            return _value == __GCC_ATOMIC_TEST_AND_SET_TRUEVAL;
        }
        bool test(memory_order order = memory_order::SeqCst) const volatile  {
            flag_type _value;
            __atomic_load(&m_iFlag, &_value, int(order));

            return _value == __GCC_ATOMIC_TEST_AND_SET_TRUEVAL;
        }

        basic_atomic_gcc_flag(const basic_atomic_gcc_flag&) = delete;
        basic_atomic_gcc_flag& operator=(const basic_atomic_gcc_flag&) = delete;
        basic_atomic_gcc_flag& operator=(const basic_atomic_gcc_flag&) volatile = delete;
    
        flag_type m_iFlag;
    };

    /**
     *  @brief Generic atomic type, primary class template.
     *  @tparam T  Type to be made atomic, must be trivally copyable.
     */
    template <typename T > 
    class basic_atomic_gcc {
      static_assert(__is_trivially_copyable(T),"mn::atomic requires a trivially copyable type");
      static_assert(sizeof(T) > 0, "Incomplete or zero-sized types are not supported");
    public:
        static constexpr bool is_always_lock_free  = __atomic_always_lock_free(sizeof(T), 0);
        
        using pointer = T;
        using self_type = basic_atomic_gcc<pointer>;
        
         basic_atomic_gcc()  = default;
        ~basic_atomic_gcc()  = default;

        basic_atomic_gcc(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) volatile = delete;
    
        constexpr basic_atomic_gcc(pointer value)  : m_tValue(value) { }

        pointer get() { return m_tValue; }
        
        void store (pointer v, memory_order order = memory_order::SeqCst)
            { __atomic_store_n (&m_tValue, v, order); }

        pointer load (memory_order order = memory_order::SeqCst) const
            { return __atomic_load_n (&m_tValue, order); }

        pointer exchange (pointer v, memory_order order = memory_order::SeqCst)
            { return __atomic_exchange_n (&m_tValue, v, order); }

        bool compare_exchange_n (pointer& expected, pointer desired, bool b, memory_order order = memory_order::SeqCst)
            { return __atomic_compare_exchange_n (&m_tValue, &expected, desired, b, order, order); }

        bool compare_exchange_t (pointer& expected, pointer desired, memory_order order = memory_order::SeqCst)
            { return compare_exchange_n (&expected, desired, true, order); }

        bool compare_exchange_f (pointer& expected, pointer desired, memory_order order = memory_order::SeqCst)
            { return compare_exchange_n (&expected, desired, false, order); }


        bool compare_exchange_strong(pointer& expected, pointer desired, memory_order order = memory_order::SeqCst) 
            { return _atomic_compare_exchange (&m_tValue, &expected, &desired, 0, order, order); }

        bool compare_exchange_weak(pointer& expected, pointer desired, memory_order order = memory_order::SeqCst) 
            { return _atomic_compare_exchange (&m_tValue, &expected, &desired, 1, order, order); }

        pointer fetch_add (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_add (&m_tValue, v, order); }

        pointer fetch_sub (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_sub (&m_tValue, v, order); }

        pointer fetch_and (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_and (&m_tValue, v, order); }

        pointer fetch_or (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_or (&m_tValue, v, order); }

        pointer fetch_xor (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_fetch_xor (&m_tValue, v, order); }

        pointer add_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_add_fetch (&m_tValue, v, order); }

        pointer sub_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_sub_fetch (&m_tValue, v, order); }

        pointer and_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_and_fetch (&m_tValue, v, order); }

        pointer or_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_or_fetch (&m_tValue, v, order); }

        pointer xor_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return __atomic_xor_fetch (&m_tValue, v, order); }
        
        bool is_lock_free() const 
            { return __atomic_is_lock_free (sizeof(pointer), &m_tValue); }

        bool is_lock_free() const volatile 
            { return __atomic_is_lock_free (sizeof(pointer), &m_tValue); }

        inline operator pointer() const	         { return load(); }
        inline operator pointer() const volatile  { return load(); }

        inline pointer operator ++ (int)          { return add_fetch (1); }
        inline pointer operator -- (int)          { return sub_fetch (1); }
        inline pointer operator ++ ()             { return add_fetch (1); }
        inline pointer operator -- ()             { return sub_fetch (1); }

        inline pointer operator ++ (int) volatile { return add_fetch (1); }
        inline pointer operator -- (int) volatile { return sub_fetch (1); }
        inline pointer operator ++ ()    volatile { return add_fetch (1); }
        inline pointer operator -- ()    volatile { return sub_fetch (1); }


        inline pointer operator += (pointer v) { return add_fetch (v); }
        inline pointer operator -= (pointer v) { return sub_fetch (v); }
        inline pointer operator &= (pointer v) { return and_fetch (v); }
        inline pointer operator |= (pointer v) { return  or_fetch (v); }
        inline pointer operator ^= (pointer v) { return xor_fetch (v); }

        inline pointer operator  = (pointer v) { store(v); return v; }
        inline pointer operator  = (pointer v) volatile { store(v); return v; }

    private:
        volatile pointer m_tValue;
    };


    /**
     * @brief Partial specialization for pointer types.
     */
    template <typename T> 
    class basic_atomic_gcc<T*> {
    public:
        using pointer =  T*;
        using base_type = basic_atomic_gcc<T>;
        using memory_order = basic_atomic_gcc_memory_order;
        using self_type = basic_atomic_gcc<T*>;

        //static constexpr bool is_always_lock_free = ATOMIC_POINTER_LOCK_FREE == 2;

        basic_atomic_gcc() = default;
        ~basic_atomic_gcc() = default;

        basic_atomic_gcc(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) = delete;
        basic_atomic_gcc& operator=(const self_type&) volatile = delete;
    
        constexpr basic_atomic_gcc(pointer value)  : m_base(value) { }

        void store (pointer v, memory_order order = memory_order::SeqCst)
            { m_base.store(v, order); }

        pointer load (memory_order order = memory_order::SeqCst) const
            { m_base.load(order); }

        pointer exchange (pointer v, memory_order order = memory_order::SeqCst)
            { m_base.exchange(v, order); }

        bool compare_exchange_n (pointer& expected, pointer desired, bool b, memory_order order = memory_order::SeqCst)
            { return m_base.compare_exchange_n(expected, desired, b, order); }

        bool compare_exchange_t (pointer& expected, pointer desired, memory_order order = memory_order::SeqCst)
            { return m_base.compare_exchange_t(expected, desired, order); }

        bool compare_exchange_f (pointer& expected, pointer desired, memory_order order = memory_order::SeqCst)
            { return m_base.compare_exchange_f(expected, desired, order); }


        bool compare_exchange_strong(pointer& expected, pointer desired, memory_order order = memory_order::SeqCst) 
            { return m_base.compare_exchange_strong(expected, desired, order); }

        bool compare_exchange_weak(pointer& expected, pointer desired, memory_order order = memory_order::SeqCst) 
            { return m_base.compare_exchange_weak(expected, desired, order); }

        pointer fetch_add (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.fetch_add(v, order); }

        pointer fetch_sub (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.fetch_sub(v, order); }

        pointer fetch_and (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.fetch_and(v, order); }

        pointer fetch_or (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.fetch_or(v, order); }

        pointer fetch_xor (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.fetch_xor(v, order); }

        pointer add_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.add_fetch(v, order); }

        pointer sub_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.sub_fetch(v, order); }

        pointer and_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.and_fetch(v, order); }

        pointer or_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.or_fetch(v, order); }

        pointer xor_fetch (pointer v, memory_order order = memory_order::SeqCst )
            { return m_base.xor_fetch(v, order); }
        
        bool is_lock_free() const 
            { return m_base.is_lock_free(); }

        bool is_lock_free() const volatile 
            { return m_base.is_lock_free(); }

        inline operator pointer() const	          { return pointer(m_base); }
        inline operator pointer() const volatile  { return pointer(m_base); }

        inline pointer operator ++ (int)          { return m_base++; }
        inline pointer operator -- (int)          { return m_base--; }
        inline pointer operator ++ ()             { return ++m_base; }
        inline pointer operator -- ()             { return --m_base; }

        inline pointer operator ++ (int) volatile { return m_base++; }
        inline pointer operator -- (int) volatile { return m_base--; }
        inline pointer operator ++ ()    volatile { return ++m_base; }
        inline pointer operator -- ()    volatile { return --m_base; }


        inline pointer operator += (ptrdiff_t v) { return m_base.operator+=(v); }
        inline pointer operator -= (ptrdiff_t v) { return m_base.operator-=(v); }
        inline pointer operator += (ptrdiff_t v) volatile  { return m_base.operator+=(v); }
        inline pointer operator -= (ptrdiff_t v) volatile  { return m_base.operator-=(v); }

        inline pointer operator  = (pointer v) { return m_base.operator=(v); }
        inline pointer operator  = (pointer v) volatile { return m_base.operator=(v); }
    private:
        base_type m_base;
    };
}

#endif