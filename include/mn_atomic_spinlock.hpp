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
#ifndef _MINLIB_d82b672b_8681_4cea_bb66_a4aa2d9927e2_H_
#define _MINLIB_d82b672b_8681_4cea_bb66_a4aa2d9927e2_H_

#include "mn_atomic.hpp"
#include "mn_lock.hpp"

namespace mn {
    template<typename T>
    class atomic_spinlock : public ILockObject {
    public:
        using self_type = atomic_spinlock<T>;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;
        using atomic_type = typename atomic_t<bool>::value_type;

        atomic_spinlock() 
            : m_locked(false), m_value(0) { }
        
        explicit atomic_spinlock(value_type value, atomic_type avalue = false)
            : m_locked(avalue), m_value(value) { }
        
        /**
         *  lock (take) a atomic_spinlock
         *  @param timeout Not use
         */
        virtual int lock(unsigned int not_use = 0) {
            bool _unlcd = false;

            while(! m_locked.compare_exchange(_unlcd, true, 
                   basic_atomic_gcc_memory_order::Acquire) ) { }
            return 0;
        }

        virtual int time_lock(const struct timespec *timeout) {
            return lock();
        }
        /**
         *  unlock (give) a atomic_spinlock.
         */
        virtual int unlock() {
            m_locked.store(false, basic_atomic_gcc_memory_order::Release);
            return 0;
        }

        /**
         * Try to lock the atomic_spinlock
         * 
         * @note call lock with timeout from 0
         * 
         * @return true if the Lock was acquired, false when not
         */
        virtual bool try_lock() {
            bool _unlcd = false;
            if( m_locked.compare_exchange(_unlcd, true, basic_atomic_gcc_memory_order::Acquire) )
                return false;
            else
                lock();
            return true;
        }

        /**
         * Is the atomic_spinlock created (initialized) ?
         * 
         * @return true if the atomic_spinlock created (initialized) 
         */
        virtual bool is_initialized() const {
            return true;
        }

        self_type& operator ++() {
            lock(); m_value++; unlock();
            return *this;
        }
        self_type& operator --() {
            lock(); m_value--; unlock();
            return *this;
        }
        self_type& operator -() {
            lock(); -m_value; unlock();
            return *this;
        }
        bool operator ==(self_type& other) {
            lock(); 
            bool _ret = false;
            if(other.m_locked.get() != m_locked.get()) _ret = false;
            else _ret = m_value == other.m_value;
            unlock();
            return _ret;
        }

        bool operator !=(self_type& other) {
            return !(*this==other);
        }

        atomic_spinlock(const self_type&) = delete;
        self_type& operator=(const self_type&) = delete; 
    protected:
        atomic_t<bool> m_locked;
        value_type m_value;
    };
}

#endif