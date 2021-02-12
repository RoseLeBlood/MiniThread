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
#ifndef MINLIB_ESP32_COUNTING_LOCK_
#define MINLIB_ESP32_COUNTING_LOCK_

#include "mn_basic_semaphore.hpp"

namespace mn {
    /**
     * Template wrapper for basic_semaphores for counting lock
     * 
     * @ingroup semaphore
     * \ingroup lock
     */
    template <class TLockType>
    class basic_clock : public basic_semaphore {
    public:
        using LockType_t = TLockType;

        /**
         * Constructor 
         * 
         * @param m The reference to the basic_semaphore object for counting
         * @param bLocked when true then lock the reference of the basic_semaphore object
         * @param timeout How long to wait to get the lock until giving up.
         */ 
        basic_clock(LockType_t& m, bool bLocked, unsigned int timeout) 
            : m_refLockRefObject(m), m_iLocks(0) { 
                if(bLocked) lock(timeout);
                m_pSpinlock = m.get_handle();
        }
        /**
         * We are not copyable.
         */ 
        basic_clock(basic_clock const&) = delete;    

        /**
         * Destructor - unlock the reference object
         */ 
        ~basic_clock() {
            if(m_iLocks > 0) {
                m_refLockRefObject.unlock();
            }
            m_iLocks = 0;
        }

        /**
         * lock (take) a semaphore.
         *
         * @param timeout How long to wait to get the lock until giving up.
         * @return ERR_SPINLOCK_OK if the Semaphore was locked, ERR_SPINLOCK_LOCK if it timed out.
         */
        virtual int lock(unsigned int timeout) {
            int lock_ret = ERR_SPINLOCK_LOCK;

            if(m_iLocks > 0) { 
                m_iLocks++; lock_ret = ERR_SPINLOCK_OK; 
            } else if(m_refLockRefObject.lock(timeout) == ERR_SPINLOCK_OK) { 
                m_iLocks++; lock_ret = ERR_SPINLOCK_OK; }

            return lock_ret;
        }
        /**
         *  unlock (give) a semaphore.
         *
         *  @return ERR_SPINLOCK_OK if the Semaphore was unlocked, ERR_SPINLOCK_UNLOCK if it failed.
         */ 
        virtual int unlock() {
            int lock_ret = ERR_SPINLOCK_OK;

            if(m_iLocks > 0) {
                m_iLocks--;
                if(m_iLocks == 0) {
                    lock_ret = m_refLockRefObject.unlock();
                }
            }
            return lock_ret;
        }

        /**
         * If the object locked ?
         * @return if true then are locked and if false when not
         */ 
        bool is_locked() { return m_iLocks > 0; }
        /**
         * If the object unlocked ?
         * @return if true then are unlocked and if false when not
         */ 
        bool is_unlocked() { return m_iLocks == 0; }

        /**
         * If the object locked ?
         * @return if true then are locked and if false when not
         */ 
        operator bool () { return m_iLocks > 0; }

        /**
         * We are not assignable.
         */ 
        void operator = (basic_clock const&) = delete; 
    protected:
        LockType_t &m_refLockRefObject;
        unsigned int m_iLocks;
    };
}