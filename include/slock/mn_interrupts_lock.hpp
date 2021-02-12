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
#ifndef _MINLIB_INTERRUPTS_LOCK_NEW_H_
#define _MINLIB_INTERRUPTS_LOCK_NEW_H_

#include "mn_system_lock.hpp"

namespace mn {
    namespace system {
        /**
         * This class is a lock helper for disable and enable the interrupts 
         * with auto_interrupts_lock_t guard
         */ 
        class basic_interrupts_lock : public ISystemLockObject {
        public:
            /**
            * Disable the interrupts
            * @return - NO_ERROR no errors
            */
            int lock(unsigned int timeout = 0);
            /**
            * Enable the interrupts
            * @return - NO_ERROR no errors
            */
            int unlock();
        };
    }
}

#endif