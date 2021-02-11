/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_MUTEX_
#define MINLIB_ESP32_MUTEX_

#include "mn_lock.hpp"
#include "mn_error.hpp"

namespace nm {
    class basic_null_lock : public ILockObject {
    public:
        basic_null_lock() : m_iLocked(0) { }
        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) { 
            if(m_iLocked == 1) return ERR_SPINLOCK_LOCK; 
            m_iLocked = 1;
            return NO_ERROR;
        }
        /**
         *  unlock (give) a semaphore.
         */
        virtual int unlock() {
            if(m_iLocked == 0) return ERR_SPINLOCK_UNLOCK; 
            m_iLocked = 0;
            return NO_ERROR;
        }
        /**
         * Is the ILockObject created (initialized) ?
         * 
         * @return true if the ILockObject created (initialized) and false when not
         */
        virtual bool is_initialized() const  { return true; }

        operator bool () { return m_iLocked == 1; }
    private:
        int m_iLocked : 2;
    };

};

#endif