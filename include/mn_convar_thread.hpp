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
#ifndef MINLIB_ESP32_CON_THREAD_
#define MINLIB_ESP32_CON_THREAD_

/**
 *  Condition variables are an additon to the mini Thread 
 *  classes. If you want to include them, you need to define the 
 *  following in your config file with MN_THREAD_CONFIG_YES
 */
#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES

#include "mn_thread.hpp"
#include "mn_convar.hpp"

/**
 * Extension of the basic thread with condition variable support
 */ 
class basic_convar_thread : public basic_thread {
    /**
     *  The Thread class and the basic_condition_variable class are interdependent.
     *  If we allow the basic_condition_variable class to access the internals of 
     *  the basic_convar_thread class, we can reduce the public interface, which is a
     *  good thing.
     */
    friend class basic_condition_variable;
public:
    basic_convar_thread();
    /**
     *  Constructor to create a named thread.
     *
     *  @param strName Name of the thread. Only useful for debugging.
     *  @param uiPriority FreeRTOS priority of this Thread.
     *  @param usStackDepth Number of "words" allocated for the Thread stack. default configMINIMAL_STACK_SIZE
     */
    basic_convar_thread(char const* strName, unsigned int uiPriority,
       unsigned short  usStackDepth = configMINIMAL_STACK_SIZE);

    /**
     * Override and at creating the m_waitSem semaphore
     */ 
    virtual void           on_create();
    /**
     * Override and at destroy the m_waitSem semaphore
     */ 
    virtual void           on_kill();

    /**
     *  helper function to signal this thread.
     */
	virtual void          signal();
    /**
     *  helper function to signal this thread, with childs
     */
    virtual void          signal_all();

    /**
     *  Have this thread wait on a condition variable.
     *
     *  @note Threads wait, while condition_variables signal.
     *
     *  @param cv The condition variable associated with the Wait.
     *  @param cvl The required condition variable lock. The
     *  Lock must be held before calling Wait.
     *  @param timeOut Allows you to specify a timeout on the Wait,
     *  if desired.
     *
     *  @return 
     */
    virtual int           wait(convar_t& cv, mutex_t& cvl, TickType_t timeOut = portMAX_DELAY);
protected:
    /**
     * Call on signal functions
     */ 
    virtual void          on_signal() { }
private:
    /**
     *  How we wait and signal the thread when using condition variables.
     *  Because a semaphore maintains state, this solves the race
     *  condition between dropping the CvLock and waiting.
     */
    binary_semaphore_t* m_waitSem;
};

using convar_thread_t = basic_convar_thread;
#endif


#endif