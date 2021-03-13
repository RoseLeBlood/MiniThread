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

#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {
        /**
         * @brief auto lock pointer. 
         * 
         * @tparam T pointer type of this lock pointer
         * @tparam TLOCK lock class @see defalt_lock 
         */
        template <typename T, class TLOCK = LockType_t>
        class basic_lock_ptr  {
        public:
            using value_type = T;
            using const_value_type = const T;

            using lock_type = TLOCK;
            using pointer =  T*;
            using const_pointer = T*;
            using reference = T&;
            using const_reference = const T&;

            using self_type = basic_lock_ptr<T, TLOCK>;
        
            /**
             * @brief constructor make the pointer and auto lock
             * @param v Reference to locked area/object
             * @param m The reference of the lock object
             */
            basic_lock_ptr(volatile reference& v, lock_type& m) 
                : m_ptr(const_cast<pointer>(&v)), m_lock(m) { 
                    m_lock.lock(); 
            }
            /**
             * @brief decunstructor and auto unlock the pointer
             */
            ~basic_lock_ptr() { 
                m_lock.unlock(); 
            }

            /** 
             * @brief Get the pointer value 
             * @return The pointer value 
             */
            const_pointer   get() const                 { return m_ptr;  }
            /** 
             * @brief Get the pointer value 
             * @return The pointer value 
             */
            pointer         get()                       { return m_ptr; }
    
            /**
             * @brief Get the reference of the pointer 
             * @return The reference of the pointer  
             */
            const_reference operator *() const         { return *m_ptr; }
            /**
             * @brief Get the reference of the pointer 
             * @return The reference of the pointer  
             */
            reference       operator *()               { return *m_ptr; }
            /**
             * @brief Helper to use this pointer as "native" pointer 
             */
            const_pointer   operator->() const         { return m_ptr; }
            /**
             * @brief Helper to use this pointer as "native" pointer 
             */
            pointer         operator->()               { return m_ptr; }

            pointer release() { 
                pointer tmp = m_ptr; 
                m_ptr = NULL; 
                return tmp; 
            }       
            void reset(pointer p = NULL) {
                m_ptr = p;
            }
            lock_type& get_lock() {
                return m_lock;
            }
            
            basic_lock_ptr(const basic_lock_ptr&) = delete;
            basic_lock_ptr& operator = (const basic_lock_ptr&) = delete;
        private:
            volatile pointer m_ptr;
            lock_type& m_lock;
        };
    }
}
#endif