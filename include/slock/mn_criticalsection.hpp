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
#ifndef _MINLIB_CITCALLOCK_NEW_H_
#define _MINLIB_CITCALLOCK_NEW_H_

#include "mn_system_lock.hpp"


namespace mn {
    namespace system {
        /**
         * Wrapper class around FreeRTOS's implementation of a critical sections.
         * Can you use im ISR Context
         * 
         * @note Use the follow FreeRTOS macros:  portENTER_CRITICAL_SAFE and portEXIT_CRITICAL_SAFE
         */ 
        class basic_critical_section : public ISystemLockObject {
        public:
            basic_critical_section();
            basic_critical_section(portMUX_TYPE type);

            /**
             * enter the critical section
             * @param timeout Not used
             * 
             * @return ERR_SYSTEM_NO_RETURN
             */
            virtual int lock(unsigned int timeout = 0);
            /**
             * leave the critical section
             * @return ERR_SYSTEM_NO_RETURN
             */
            virtual int unlock();
        protected:
            portMUX_TYPE m_pHandle;
        };
        /**
         * Wrapper class around FreeRTOS's implementation of a critical sections.
         * Acquire a portmux spinlock with a timeout
         * 
         * @note Use the follow FreeRTOS functions:  
         * vPortCPUAcquireMutexTimeout and vPortCPUReleaseMutex
         */ 
        class basic_critical_section_timedout : public ISystemLockObject { 
        public:
            /**
            * Construtor initialize the cpu mutex 
            * use the FreeRTOS : vPortCPUInitializeMutex function
            */ 
            basic_critical_section_timedout();
            /**
            * enter the critical section
            * @param timeout How long to wait (in ticks) to get the lock until giving up.
            * By a given UINT_MAX then then use no timeout
            * 
            * @return ERR_SYSTEM_OK if enter in the critical section and ERR_SYSTEM_LOCK if it timed out.
            */
            virtual int lock(unsigned int timeout = UINT_MAX);

            /**
            * enter the critical section
            * @param abs_time How long to wait (in timeval) to get the lock until giving up.
            * 
            * @return ERR_SYSTEM_OK if enter in the critical section and ERR_SYSTEM_LOCK if it timed out.
            */
            virtual int time_lock(const struct timespec *timeout);
            /**
            * leave the critical section
            * @return ERR_SYSTEM_NO_RETURN
            */
            virtual int unlock();
            /**
            * try to enter the critical section
            * true if the Lock was acquired, false when not
            */ 
            virtual bool try_lock();
        protected:
            portMUX_TYPE m_pHandle;
        }; 

        /**
        * Wrapper class around FreeRTOS's implementation of a critical sections nested.
        */ 
        class basic_critical_section_nested : public ISystemLockObject { 
        public:
            /**
            * enter the critical section nested 
            * @param timeout How long to wait (in ticks) to get the lock until giving up.
            * By a given UINT_MAX then then use no timeout
            * 
            * @return ERR_SYSTEM_OK if enter in the critical section and ERR_SYSTEM_LOCK if it timed out.
            */
            virtual int lock(unsigned int timeout = UINT_MAX);
            /**
            * leave the critical section nested
            * @return ERR_SYSTEM_NO_RETURN
            */
            virtual int unlock();
        protected:
            unsigned int m_iState;
        }; 
    }
}

#endif