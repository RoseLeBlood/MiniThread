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

#if ( configUSE_TICK_HOOK == 1 )

#include "mn_config.hpp"
#include "mn_task.hpp"
#include "mn_error.hpp"

#include "queue/mn_queue.hpp"

#include "mn_tickhook_entry.hpp"


/**
 * FreeRTOS expects this function to exist and requires it to be 
 * named as such with the following signature.
 * 
 */
extern "C" void vApplicationTickHook(void);

namespace mn {
    /**
     * Wrapper class for Tick hooks, functions you want to run within the tick ISR. 
     * 
     * You can register multiple hooks (base_tickhook_entry) with this class.
     * 
     * \ingroup hook
     */ 
    class base_tickhook {
    private:
        /**
         * @brief Creates a tick hook %list with default constructed elements.
         * 
         * @note This is a signleton class, only one object 
         * plaese use base_tickhook::instance() 
         */ 
        base_tickhook();

        /**
         * The static object of this class
         */ 
        static base_tickhook* m_pInstance;
        /**
         * The static instance mutex
         */ 
        static mutex_t  m_staticInstanceMux;
    public:
        /**
         * Get the singleton instance 
         * @return The singleton instance
         */ 
        static base_tickhook& instance();

        /**
         * Add a new tickhook to the list
         * @param entry The new tick hook entry
         * 
         * @return 
         *  - ERR_TICKHOOK_OK The entry was added
         *  - ERR_TICKHOOK_ADD The entry already added
         *  - ERR_TICKHOOK_ENTRY_NULL The entry is null
         *  - ERR_TIMEOUT TimeOut
         */ 
        int enqueue(base_tickhook_entry* entry, 
            unsigned int timeout = (unsigned int) 0xffffffffUL);
        /**
         *  Remove an item from the front of the queue.
         *
         *  @param item Where the item you are removing will be returned to.
         *  @param timeout How long to wait to remove an item to the queue.
         *  @return  - 'ERR_QUEUE_OK' the item was removed 
         *           - 'ERR_QUEUE_REMOVE' on an error
         *           - 'ERR_QUEUE_NOTCREATED' when the queue not created
         *           - 'ERR_TIMEOUT' TimeOut
         */
        int dequeue(base_tickhook_entry* entry,
            unsigned int timeout = (unsigned int) 0xffffffffUL);
        /**
         * Clear the list
         */ 
        void clear();

        /**
         * Reset
         */ 
        void reset();
        /**
         * How many entrys are in the list
         * @return The number of entrys in the list
         */ 
        unsigned int count();
        /**
         * The tick hook logic - call from vApplicationTickHook. 
         * All not oneshotted entrys enqueue after run to m_listToAdd.
         * 
         */ 
        void onApplicationTickHook();
    private:
        
        /**
         * Swapped all entrys in m_listToAdd to m_listHooks back
         * 
         */ 
        void swap();
    private:
        mutex_t m_mutexAdd;
        queue::queue_t m_listHooks, m_listToAdd;
        unsigned int m_iCurrent;
    };

    using tickhook_t = base_tickhook;
}

#endif // #if ( configUSE_TICK_HOOK == 1 )
#endif //  MINLIB_ESP32_TICK_HOOK_