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
#ifndef MINLIB_INTERFACE_TIMER_
#define MINLIB_INTERFACE_TIMER_

namespace mn {
    /**
     * Basic Interface for all timers in the library
     */ 
    class ITimer {
    public:
        /**
         * Create the timer
         * 
         * @note Timers are not active after they are created, you need to
         * activate them via active, reset, etc.
         * 
         * @return ERR_TIMER_OK No error, ERR_TIMER_ALREADYINIT The timer are allready created and
         * ERR_TIMER_CANTCREATE on error on create the FreeRTOS Timer
         */ 
        virtual int create() = 0;

        /**
         * destroy the timer
         * 
         * @param timeout How long to wait
         * @return ERR_TIMER_OK No error, ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
         */ 
        virtual int destroy(unsigned int timeout = (unsigned int) 0xffffffffUL) = 0;

        /**
         * Start the timer.
         *
         * @param timeout How long to wait 
         * @returns ERR_TIMER_OK All okay, no error
         *          ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
         *          ERR_TIMER_AKTIVATE if it will not (i.e. timeout).
         */
        virtual int active(unsigned int timeout = (unsigned int) 0xffffffffUL) = 0;

        /**
         * Stop the timer
         *
         * @param timeout How long to wait 
         * @returns ERR_TIMER_OK All okay, no error
         *          ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
         *          ERR_TIMER_INAKTIVATE if it will not (i.e. timeout).
         */
        virtual int inactive(unsigned int timeout = (unsigned int) 0xffffffffUL) = 0;

        /**
         * Reset the timer
         *
         * @param timeout How long to wait 
         * @returns ERR_TIMER_OK All okay, no error
         *          ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
         *          ERR_TIMER_RESET if it will not (i.e. timeout).
         */
        virtual int reset(unsigned int timeout = (unsigned int) 0xffffffffUL) = 0;

        /**
         *  Change a timer's period.
         *
         *  @param uiNewPeriod The new period in ticks.
         *  @param timeout How long to wait 
         *  @returns true no error, false if it will not (i.e. timeout).
         */
        virtual bool set_period(unsigned int uiNewPeriod, unsigned int timeout = (unsigned int) 0xffffffffUL) = 0;
        /**
         * Get the timer's period
         * 
         * @return The timer's period
         */ 
        virtual unsigned int get_period() = 0;

        /**
         * Get the timer handle
         * @return The timer handle
         */ 
        virtual void*  get_handle() = 0;

        virtual void        set_id(int nId) = 0;

        /**
         * Queries a timer to see if it is active or dormant.
         * 
         * @return false will be returned if the timer is dormant.  
         * And true will be returned if the timer is active.
         */ 
        virtual bool        is_running() = 0;

        virtual operator bool() { return is_running(); }
    protected:
        /**
         * Implementation of your actual timer code.
         * You must override this function.
         */
        virtual void on_timer() = 0;

        /**
         * call befor on_timer
         */ 
        virtual void on_enter() =  0;
        /**
         * call after on_timer
         */ 
        virtual void on_exit() = 0;

    };
}

#endif
