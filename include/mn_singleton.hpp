/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_SINGLETON_H_
#define MINLIB_ESP32_SINGLETON_H_

#include "mn_config.hpp"
#include "mn_autolock.hpp"
#include "mn_pointer.hpp"

namespace mn {

    template < typename T, class TLOCK = LockType_t> 
    class basic_singleton {
    public:
        using value_type = T;
        using reference = value_type&;
        using lock_t = TLOCK;
        using pointer = T*;
        
        static reference instance() {
            basic_autolock<TLOCK> lock(m_tLock);

            if( m_ptr == 0 ) {
                m_ptr = new T();
            }
            return *m_ptr;
        }
        basic_singleton(const basic_singleton&)       = delete;
        void operator=(const basic_singleton&)  = delete; 
    protected:
        basic_singleton() {}
    private:
        static pointer m_ptr;  
        static lock_t  m_tLock; 
    };
    template < typename T, class TLOCK > 
    T* basic_singleton<T, TLOCK>::m_ptr = 0;

    template < typename T, class TLOCK > 
    TLOCK basic_singleton<T, TLOCK>::m_tLock;

}

#endif