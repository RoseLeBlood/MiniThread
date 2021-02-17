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

namespace mn {

    enum class basic_atomic_gcc_memory_order {
        Relaxed = __ATOMIC_RELAXED,
        Consume = __ATOMIC_CONSUME,
        Acquire = __ATOMIC_ACQUIRE,
        Release = __ATOMIC_RELEASE,
        AcqRel = __ATOMIC_ACQ_REL,
        basic_atomic_gcc_memory_order::SeqCst = __ATOMIC_SEQ_CST
    };

    template <typename T > 
    class basic_atomic_gcc {
    public:
        using value_type = T;
        using self_type = basic_atomic_gcc<value_type>;
        
        basic_atomic_gcc() : _m_value(0) { }
        explicit basic_atomic_gcc(value_type value) : _m_value(value) { }
    
        basic_atomic_gcc(const self_type& self) : _m_value( value.get() ) { }
    
        value_type get() { return _m_value; }
        
        bool is_free (void) const
            { return __atomic_is_lock_free (sizeof(value_type), &_m_value); }

        void store (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst)
            { __atomic_store_n (&_m_value, v, order); }

        value_type load (basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst) const
            { return __atomic_load_n (&_m_value, order); }

        value_type exchange (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst)
            { return __atomic_exchange_n (&_m_value, v, order); }

        bool compare_exchange (value_type& expected, T desired, bool b, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst)
            { return __atomic_compare_exchange_n (&_m_value, &expected, desired, b, order, order); }

        value_type fetch_add (T v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_fetch_add (&_m_value, v, order); }

        value_type fetch_sub (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_fetch_sub (&_m_value, v, order); }

        value_type fetch_and (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_fetch_and (&_m_value, v, order); }

        value_type fetch_or (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_fetch_or (&_m_value, v, order); }

        value_type fetch_xor (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_fetch_xor (&_m_value, v, order); }

        value_type add_fetch (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
                { return __atomic_add_fetch (&_m_value, v, order); }

        value_type sub_fetch (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
                { return __atomic_sub_fetch (&_m_value, v, order); }

        value_type and_fetch (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_and_fetch (&_m_value, v, order); }

        value_type or_fetch (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_or_fetch (&_m_value, v, order); }

        value_type xor_fetch (value_type v, basic_atomic_gcc_memory_order order = basic_atomic_gcc_memory_order::SeqCst )
            { return __atomic_xor_fetch (&_m_value, v, order); }
        
        void swap(self_type& other) {
            mn::swap<value_type>(_m_value, other._m_value);
        }
        
        inline                  operator value_type (void) const	
            { return load(); }

        inline value_type		operator= (value_type v)		
            { store(v); return v; }

        inline value_type		operator++ (int)	
            { return fetch_add (1); }

        inline value_type		operator-- (int)	
            { return fetch_sub (1); }

        inline value_type		operator++ (void)	
            { return add_fetch (1); }

        inline value_type		operator-- (void)	
            { return sub_fetch (1); }
        inline value_type		operator+= (value_type v)	
            { return add_fetch (v); }

        inline value_type		operator-= (value_type v)	
            { return sub_fetch (v); }

        inline value_type		operator&= (value_type v)	
            { return and_fetch (v); }

        inline value_type		operator|= (value_type v)	
            { return or_fetch (v); }

        inline value_type		operator^= (value_type v)	
            { return xor_fetch (v); }

        basic_atomic_gcc& operator = (const basic_atomic_gcc&) = delete;
    private:
        T _m_value;
    };
}

#endif