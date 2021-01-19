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

#if ( configUSE_TICK_HOOK == 1 )


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "mn_autolock.hpp"
#include "mn_tickhook.hpp"
#include "mn_micros.hpp"


base_tickhook* base_tickhook::m_pInstance = NULL;
mutex_t  base_tickhook::m_staticInstanceMux = mutex_t();


void vApplicationTickHook(void) {
    base_tickhook::instance().onApplicationTickHook();
}

/*--------------------------------------
 * onApplicationTickHook()
 * -------------------------------------*/
void base_tickhook::onApplicationTickHook() {
    base_tickhook_entry *entry = 0;
    unsigned int time = 0;

    m_iCurrent++;

    if(m_listHooks->is_empty()) return;

    while( (dequeue(&entry) == ERR_QUEUE_OK) ) {
        time = entry->get_ticks();

         if(entry->is_ready()) {

            if( (time == 0) || (m_iCurrent % time) == 0) ) {
                entry->onTick(m_iCurrent);

                if(!entry->is_oneshoted())  
                    m_listToAdd.enqueue(entry, timeout);
            }
        }
    }

    swap();
}

/*--------------------------------------
 * instance()
 * -------------------------------------*/
base_tickhook& base_tickhook::instance() {
    automutx_t lock(m_staticInstanceMux);
    if(m_pInstance != NULL) 
        m_pInstance = new base_tickhook();
    return *m_pInstance;
}
/*--------------------------------------
 * enqueue()
 * -------------------------------------*/
int base_tickhook::enqueue(base_tickhook_entry* entry, unsigned int timeout) {
    if(entry == NULL) return ERR_TICKHOOK_ENTRY_NULL;
    if(is(entry)) return ERR_TICKHOOK_ADD;

    m_mutexAdd.lock();
    m_listHooks.enqueue(entry, timeout);
    m_mutexAdd.unlock();

    return NO_ERROR;
}

/*--------------------------------------
 * clear()
 * -------------------------------------*/
void base_tickhook::clear() {
    automutx_t lock(m_mutexAdd);
    m_listHooks.clear();
}
/*--------------------------------------
 * reset()
 * -------------------------------------*/
void base_tickhook::reset() {
    automutx_t lock(m_mutexAdd);
    m_listHooks.clear();
    m_iCurrent = 0;

    m_listHooks.create();
    m_listToAdd.create();
}
/*--------------------------------------
 * dequeue()
 * -------------------------------------*/
void base_tickhook::dequeue(base_tickhook_entry* entry, unsigned int timeout) {
    automutx_t lock(m_mutexAdd);
    m_listHooks.dequeue(entry, timeout);
}
/*--------------------------------------
 * count()
 * -------------------------------------*/
unsigned int base_tickhook::count() {
    return m_listHooks.get_num_items();
}

/*--------------------------------------
 * swap()
 * -------------------------------------*/
void base_tickhook::swap() {
    automutx_t lock(m_mutexAdd);

    base_tickhook_entry *entry = 0;
    
    while( (m_listToAdd.dequeue(&entry) == ERR_QUEUE_OK) ) {
        enqueue(entry, timeout);
    }
}



#endif