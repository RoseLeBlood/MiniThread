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
#ifndef MINLIB_e3c52c47_2600_40a8_bbff_29fa60eea6d2_H_
#define MINLIB_e3c52c47_2600_40a8_bbff_29fa60eea6d2_H_

#include "mn_algorithm.hpp"
#include "../mn_autolock.hpp"


namespace mn {
    namespace pointer {
        /**
         * @brief auto lock pointer. 
         * 
         * @tparam T pointer type of this lock pointer
         * @tparam TLOCK lock class @see defalt_lock 
         */
        template <typename T, class TLOCK = LockType_t>
        class basic_lock_ptr {
        public:
            using value_type = T;
            using lock_type = TLOCK;
            using pointer = volatile T*;
            using self_type = basic_lock_ptr<T, TLOCK>;
        
            /**
             * @brief constructor make the pointer and auto lock
             * 
             * @param v Reference to locked area/object
             * @param m The reference of the lock object
             */
            basic_lock_ptr(pointer v, lock_type& m) 
                : m_ptr(v), m_lock(m)                   { m_lock.lock(); }
            
            /**
             * @brief decunstructor and auto unlock the pointer
             */
            ~basic_lock_ptr()                           { m_lock.unlock(); }

            /** @brief get the pointer value const */
            value_type const *get() const               { return m_ptr;  }
            /** @brief get the pointer value */ 
            value_type *get()                           {  return m_ptr; }
            /** @brief return the reference of the pointer */
            value_type operator *()                    { return *m_ptr;  }
            /** @brief helper to use this pointer as "native" pointer */
            value_type *operator->()                    { return m_ptr;  }
            
            basic_lock_ptr(const basic_lock_ptr&) = delete;
            basic_lock_ptr& operator = (const basic_lock_ptr&) = delete;
        private:
            pointer m_ptr;
            lock_type& m_lock;
         };
    }
}
#endif