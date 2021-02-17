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
#ifndef MINLIB_ESP32_TIMER_
#define MINLIB_ESP32_TIMER_

#include "mn_itimer.hpp"
#include "esp_timer.h"

namespace mn {
    namespace esp32 {
            
        /**
         * Wrapper class around esp-idf of a timer.
         * This is a base abstract class, all esp32_timers must be override 
         * the function on_timer()
         * 
         * @ingroup base
         */
        class basic_esp32_timer : public ITimer {
        public:
            /**
             * Construct a timer.
             *
             * @param strName Name of the timer.
             * @param uiPeriod When does the timer expire and run your Run()
             *        method.
             * @param bIsOneShot true if this is a one shot timer.
             *  false if the timer expires every PeriodInTicks.
             *         
             */
            basic_esp32_timer(const char * strName, unsigned int uiPeriod, bool bIsOneShot = true);

            virtual ~basic_esp32_timer() { destroy(); }

            /**
             * Create the timer
             * 
             * @note Timers are not active after they are created, you need to
             * activate them via active, reset, etc.
             * 
             * @return - ERR_TIMER_OK No error, 
             *         - ERR_TIMER_ALREADYINIT The timer are allready created 
             *         - ERR_TIMER_CANTCREATE on error on create the ESP Timer
             */ 
            virtual int create();

            /**
             * destroy the timer
             * 
             * @param timeout How long to wait - not use
             * @return  - ERR_TIMER_OK No error, 
             *          - ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
             */ 
            virtual int destroy(unsigned int timeout = (unsigned int) 0xffffffffUL);

            /**
             * Start the timer.
             *
             * @param timeout How long to wait - not use
             * @returns ERR_TIMER_OK All okay, no error
             *          ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
             *          ERR_TIMER_AKTIVATE if it will not (i.e. timeout).
             */
            virtual int active(unsigned int timeout = (unsigned int) 0xffffffffUL);

            /**
             * Stop the timer
             *
             * @param timeout How long to wait - not use
             * @returns ERR_TIMER_OK All okay, no error
             *          ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
             *          ERR_TIMER_INAKTIVATE Can not stop the timer
             */
            virtual int inactive(unsigned int timeout = (unsigned int) 0xffffffffUL);

            /**
             * Reset the timer (first inactive and then active)
             *
             * @param timeout How long to wait - not use
             * @returns - ERR_TIMER_OK All okay, no error
             *          - ERR_TIMER_NOTCREATED the timer are not created, plaese call create() first
             *          - ERR_TIMER_RESET Can not reset
             *          - ERR_TIMER_INAKTIVATE Can not stop the timer
             */
            virtual int reset(unsigned int timeout = (unsigned int) 0xffffffffUL);

            /**
             *  Change a timer's period.
             *
             *  @param uiNewPeriod The new period in ticks.
             *  @param timeout How long to wait 
             *  @returns true no error, false if it will not (i.e. timeout).
             */
            virtual bool set_period(unsigned int uiNewPeriod, unsigned int timeout = (unsigned int) 0xffffffffUL);
            /**
             * Get the timer's period
             * 
             * @return The timer's period
             */ 
            virtual unsigned int get_period() { return m_uiPeriod; }

            /**
             * Get the timer handle
             * @return The timer handle
             */ 
            virtual void*  get_handle();

            /**
             * Get the timer ID - not implantated
             */ 
            virtual void set_id(int nId) { /* NOT IMPL */ }

            /**
             * Queries a timer to see if it is active or dormant.
             * 
             * @return false will be returned if the timer is dormant.  
             * And true will be returned if the timer is active.
             */ 
            virtual bool        is_running() { return m_bIsRunning; }

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
            virtual void on_enter() { }
            /**
             * call after on_timer
             */ 
            virtual void on_exit() { }
        private:
            /**
             * Adapter function that allows you to write a class
             * specific on_timer() function that interfaces with FreeRTOS.
             */
            static void runtimerstub(void* xTimer);
        private:
            /**
             *  Reference to the underlying timer handle.
             */
            esp_timer_handle_t m_pHandle;
            
            /**
             * A saved / cached copy of what the timers's mode is.
             */
            bool m_bIsOneShot;
            /**
             * A saved / cached copy of what the timers's period is.
             */
            unsigned int m_uiPeriod;
            /**
             *  A saved / cached copy of what the timers's name is.
             */ 
            const char* m_strName;

            /**
             * A saved / cached copy is the timer running
             */ 
            bool m_bIsRunning;

            /**
             * A saved / cached copy is the timer created
             */ 
            bool m_bIsInit;
        };

        using timer32_t = basic_esp32_timer;
    }
}
#endif