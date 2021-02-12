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
#ifndef _MINLIB_SCHEDULAR_LOCK_NEW_H_
#define _MINLIB_SCHEDULAR_LOCK_NEW_H_

#include "mn_system_lock.hpp"

namespace mn {
    namespace system {
        /**
         * This class is a lock helper for disable and enable the schedular 
         * with auto_schedular_lock_t guard
         * Example usage:
         * @code{c}
         * 
         * void* foo_task::on_task() {
         *      basic_schedular_lock scheL;
         * 
         *      for(;;) {
         *          // Prevent the real time kernel swapping out the task.
         *          scheL.lock();
         * 
         *          // Perform the operation here.  There is no need to use critical
         *          // sections as we have all the microcontroller processing time.
         *          // During this time interrupts will still operate and the real
         *          // time kernel tick count will be maintained.
         * 
         *          // ...
         * 
         *          // The operation is complete.  Restart the kernel.  We want to force
         *          // a context switch - but there is no point if resuming the scheduler
         *          // caused a context switch already.
         *          if( scheL.unlock() () ) {
         *            basic_task::yield ();
         *          }
         * 
         *      }
         * }
         * @endcode
         */ 
        class basic_schedular_lock : public ISystemLockObject {
        public:
            /**
            * Disable the schedular
            * @return - ERR_SYSTEM_NO_RETURN no errors
            */
            int lock(unsigned int timeout = 0);
            /**
            * Enable the schedular
            * @return If resuming the scheduler caused a context switch then ERR_SYSTEM_OK is
            *		   returned, otherwise ERR_SYSTEM_UNLOCK is returned.
            */
            int unlock();
        };
    }
}

#endif