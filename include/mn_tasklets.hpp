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
#ifndef MINLIB_ESP32_TASKLETS_
#define MINLIB_ESP32_TASKLETS_



#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"
#include "mn_convar.hpp"

/**
 *  A FreeRTOS wrapper for its concept of a Pended Function.
 *  In Linux, one permutation of this would be a Tasklet, or
 *  bottom half processing from an ISR.
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your basic_tasklet should
 *  be derived from the basic_tasklet class. Then implement the virtual on_tasklet
 *  function.
 */
class basic_tasklet {
public:

    /**
     *  Create and schedule this Tasklet to run.
     *
     *  @param parameter Value passed to your on_tasklet method.
     *  @param CmdTimeout How long to wait to send this command to the
     *         timer daemon.
     *  @returns ERR_TASKLET_OK This command will be sent to the timer daemon,
     *           ERR_TASKLET_CANTSTART It will not (i.e. timeout) and
     *           ERR_TASKLET_CANTINITLOCKT The Locking Object can't create - tasklet not run
     */
    virtual int create(uint32_t parameter, unsigned int timeout = MN_THREAD_CONFIG_TIMEOUT_TASKLET_DEFAULT);
    /**
     * Destroy the Tasklet
     * @returns ERR_TASKLET_OK Destroyed without any errors
     */
    virtual int destroy();
protected:
    /**
     *  Implementation of your actual tasklet code.
     *  You must override this function.
     *
     *  @param parameter Value passed to you from the create() methods.
     */
    virtual int on_tasklet(uint32_t arg);
protected:
    /**
     *  Adapter function that allows you to write a class
     *  specific on_tasklet() function that interfaces with FreeRTOS.
     */
  static void runtaskletstub(void* parm, uint32_t parameter);
protected:
    /**
     *  Protect against accidental deletion before we were executed.
     */
    counting_semaphore_t   m_ssLock;
};

using tasklet_t = basic_tasklet;

#endif