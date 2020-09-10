/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _MINLIB_CRITICAL_H_
#define _MINLIB_CRITICAL_H_

#include "mn_mutex.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/**
 *  Wrapper class around various critical section type
 *  synchronization mechanisms within FreeRTOS.
 */
class basic_critical {
public:
    /**
     *  Disable context switches as well as maskable interrupts.
     */
    static void enter(portMUX_TYPE handle);
    /**
     *  Re-enable context switches.
     */
    static void exit(portMUX_TYPE handle);

    /**
     *  Disable all maskable interrupts.
     */
    static void disable_interrupts();
    /**
     *  Enable all maskable interrupts.
     */
    static void enable_interrupts();

    /**
     *  Suspend the scheduler without disabling interrupts.
     */
    static void stop_scheduler();
    /**
     *  Re-enable the scheduler.
     */
    static void resume_scheduler();
};

/**
 * Wrapper class to use the critical section lock with autolock guard see auto_critical_t
 */ 
class basic_critical_lock {
public:
    basic_critical_lock();

    virtual void lock() { basic_critical::enter(m_pHandle);  }
    virtual void unlock() { basic_critical::exit(m_pHandle);  }
private:
    portMUX_TYPE m_pHandle;
};

/**
 * Wrapper class to use the interrupts lock with autolock guard see auto_interrupt_t
 */
class basic_interrupts_lock { 
public:
    virtual void lock() { basic_critical::disable_interrupts();  }
    virtual void unlock() { basic_critical::enable_interrupts();  }
};

/**
 * Wrapper class to use the scheduler lock with autolock guard see auto_schedular_t
 */
class basic_scheduler_lock { 
public:
    virtual void lock() { basic_critical::stop_scheduler();  }
    virtual void unlock() { basic_critical::resume_scheduler();  }
};

using critical_t = basic_critical;

using critical_lock_t = basic_critical_lock;
using interrupts_lock_t = basic_interrupts_lock;
using sheduler_lock_t = basic_scheduler_lock;


#endif