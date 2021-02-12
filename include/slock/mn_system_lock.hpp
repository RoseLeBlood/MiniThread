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
#ifndef _MINLIB_INTERFACE_SYSTEMLOCK_NEW_H_
#define _MINLIB_INTERFACE_SYSTEMLOCK_NEW_H_

#include "../mn_lock.hpp"

namespace mn {
    namespace system {
        /**
         * Interface for all system lock types in this library
         * This is an abstract mixed base class.
         * To use this, you need to subclass it. All of your SystemLockObject should
         * be derived from the ISystemLockObject class. 
         * Then implement the virtual lock, unlock functions, 
         * from the base class ILockObject, the function virtual time_lock,
         * implemented this class
         * 
         * @ingroup Interface
         * @ingroup lock
         */ 
        class ISystemLockObject : public ::mn::ILockObject {
        public:
            /**
             * call lock() 
             * @see lock()
             */
            virtual int time_lock(const struct timespec *timeout) { 
                return lock();
            }
            /**
            * Is the SystemLockObject (initialized) ?
            * 
            * @return Always true
            */
            virtual bool is_initialized() { 
                return true; 
            }
        };
    }
}

#endif