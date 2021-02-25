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

#ifndef _MINLIB_7ea02a66_6557_4465_8a08_05054e5ccea8_H_
#define _MINLIB_7ea02a66_6557_4465_8a08_05054e5ccea8_H_

#include "mn_error.hpp"
#include "mn_config.hpp"
#include "pointer/mn_lock_ptr.hpp"
#include "mn_autolock.hpp"

namespace mn {

    /**
     * @brief safe counter is a base of thread saftly counter 
     */ 
    template <typename T, class TLOCK>
    class basic_safe_counter {
    public:
        using value_type = T;
        using self_type = basic_safe_counter<T, TLOCK>;
        using lock_type = TLOCK;
        using lock_ptr_type = pointer::basic_lock_ptr<T, TLOCK>;

        /**
         * @brief Construct a new basic safe counter object
         */
        basic_safe_counter() 
            : m_iCount(0) { }

        /**
         * @brief Construct a new basic safe counter object
         * 
         * @param start Start value of this counter 
         */
        explicit basic_safe_counter(value_type start) 
            : m_iCount(start) {}

        ///@brief thread saftly increment operator
        self_type& operator ++() {
            lock_ptr_type lock = lock_ptr_type(m_iCount, m_lockObject);
            ++(*lock);

            return *this;
        }
        ///@brief thread saftly decrement operator
        self_type& operator --() {
            lock_ptr_type lock = lock_ptr_type(m_iCount, m_lockObject);
            --(*lock);

            return *this;
        }
        ///@brief get the current count
        value_type count() {
            lock_ptr_type lock = lock_ptr_type(m_iCount, m_lockObject);
            return lock->get();
        }
    protected:
        lock_type m_lockObject;
        volatile value_type m_iCount; 
    };

    /**
     * @brief sot the default safe_counter, value type are uint64_t and as lock type use LockType_t
     * @see LockType_t 
     * @see basic_safe_counter
     * 
     * @code 
     * 
     * safe_counter_t counter(1);
     * 
     * // this while parallel use
     * while(it != list.end()) { 
     *  ++counter;
     * }
     * std::cout << "In list are: " << counter.count() << " elements";
     * 
     * @endcode
     */
    using safe_counter_t = basic_safe_counter<uint64_t, LockType_t>;
}