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
#ifndef MINLIB_ESP32_TICK_HOOK_
#define MINLIB_ESP32_TICK_HOOK_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mn_config.hpp"
#include "mn_task.hpp"
#include "mn_error.hpp"

#include <list>

#if ( configUSE_TICK_HOOK == 1 )

/**
 * FreeRTOS expects this function to exist and requires it to be 
 * named as such with the following signature.
 * 
 */
extern "C" void vApplicationTickHook(void);

/**
 * This is an abstract base class for a entry of base_tickhook.
 *  
 * To use this, you need to subclass it. All of your coroutines should
 * be derived from the base_tickhook_entry class. Then implement the virtual on_hook
 * function. 
 */ 
class base_tickhook_entry {
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
            void* pArgs = 0, int numArgs = 0)

            : m_uiNumArgs(numArgs), m_pArgs(pArgs), 
              m_iTicksToCall(iTicksToCall), 
              m_bOneShoted(bOneShoted), 
              m_bReady(false) { }

        /**
         * Set the number of ticks after this run. When param uiTicks 0 is, then run
         * every tick.
         * 
         * @param uiTicks The number of ticks left after this run
         */ 
        bool set_ticks(unsigned int uiTicks) {
            if(m_bReady) return false;
            m_iTicksToCall = uiTicks; return true;
        }
        /**
         * Set the is this entry oneshot? 
         * @param bIsOneShot If true then oneshoted this entry and removed after run.
         */ 
        bool set_oneshot(bool bIsOneShot) {
            if(m_bReady) return false;
            m_bOneShoted = bIsOneShot; return true;
        }
        /**
         * Marked the hook as ready, after this you can not modifitated the hook
         */ 
        void start() {
            m_bReady = true;
        }
        void stop() {
            m_bReady = false;
        }

        bool is_oneshoted() { return m_bOneShoted; }
        bool is_ready() { return m_bReady; }

        unsigned int get_ticks() { return m_iTicksToCall; }

    protected:
        virtual void on_tick() = 0;
    protected:
        uint32_t m_uiNumArgs;
        void*    m_pArgs;
        unsigned int m_iTicksToCall;
        bool m_bOneShoted;
        bool m_bReady;
};


/**
 * Wrapper class for Tick hooks, functions you want to run within the tick ISR. 
 * 
 * You can register multiple hooks (base_tickhook_entry) with this class.
 */ 
class base_tickhook {
public:
    /**
     * Add a new tickhook to the list
     * @param entry The new tick hook entry
     * 
     * @return 
     *  - ERR_TICKHOOK_OK The entry was added
     *  - ERR_TICKHOOK_ADD The entry already added
     */ 
    static int register(base_tickhook_entry* entry);
private:
    /**
     *  Allow the global vApplicationTickHook() function access
     *  to the internals of this class. This simplifies the overall
     *  design.
     */
    friend void ::vApplicationTickHook();

    static std::list<base_tickhook_entry*> m_listHooks;
};


#endif