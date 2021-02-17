/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_TICK_HOOK_ENTRY_
#define MINLIB_ESP32_TICK_HOOK_ENTRY_

#include "freertos/FreeRTOS.h"
#include "mn_config.hpp"
#include "mn_task.hpp"
#include "mn_error.hpp"

namespace mn {
    class base_tickhook;

    /**
     * This is an abstract base class for a entry of base_tickhook.
     *  
     * To use this, you need to subclass it. All of your coroutines should
     * be derived from the base_tickhook_entry class. Then implement the virtual on_hook
     * function. 
     * 
     * \ingroup hook
     */ 
    class base_tickhook_entry {
        friend class base_tickhook;
        public:
            /**
             * Constructor 
             * 
             * @param iTicksToCall The number of ticks left after this run
             * @param bOneShoted If the hook oneshoted ?
             * @param pArgs a array of arguments.
             * @param numArgs How many arguments are contains in the array pArgs.
             */ 
            base_tickhook_entry(unsigned int iTicksToCall, bool bOneShoted = false, 
                void* pUserData = 0);

            base_tickhook_entry(const base_tickhook_entry&) = delete;
            /**
             * Set the number of ticks after this run. When param uiTicks 0 is, then run
             * every tick.
             * 
             * @param uiTicks The number of ticks left after this run
             */ 
            bool set_ticks(unsigned int uiTicks);
            /**
             * Set the is this entry oneshot? 
             * @param bIsOneShot If true then oneshoted this entry and removed after run.
             */ 
            bool set_oneshot(bool bIsOneShot);
            /**
             * Marked the hook as ready, after this you can not modifitated the hook
             */ 
            void start();
            void stop();

            bool is_oneshoted();
            bool is_ready();

            unsigned int get_ticks();
        protected:
            virtual void onTick(const unsigned int ticks) = 0;
        protected:
            void*    m_pUserData;
            unsigned int m_iTicksToCall;
            bool m_bOneShoted;
            bool m_bReady;
            mutex_t m_mutexEntry;
    };
}

#endif