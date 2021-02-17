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
        using pointer = auto_ptr<value_type>;
        
        static reference instance() {
            basic_autolock<TLOCK> lock(m_tLock);

            if( m_ptr.get() == 0 ) {
                m_ptr.reset( new value_type() );
            }
            return *m_ptr;
        }
        basic_singleton(singleton const&)       = delete;
        void operator=(basic_singleton const&)  = delete; 
    protected:
        basic_singleton() {}
    private:
        static pointer m_ptr;  
        static lock_t  m_tLock; 
    };
    template < typename T, class TLOCK > 
    mn::auto_ptr<T> basic_singleton<T, TLOCK>::m_ptr = mn::auto_ptr<T>(0);

    template < typename T, class TLOCK > 
    basic_singleton<T, TLOCK>::lock_t basic_singleton<T, TLOCK>::m_tLock;

}

#endif