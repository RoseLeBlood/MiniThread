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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if ( configUSE_TICK_HOOK == 1 )

#include "mn_tickhook_entry.hpp"

namespace mn {
    /*--------------------------------------
    * base_tickhook_entry()
    * -------------------------------------*/
    base_tickhook_entry::base_tickhook_entry(unsigned int iTicksToCall, bool bOneShoted, 
        void* pUserData)

        : m_pUserData(pUserData), 
        m_iTicksToCall(iTicksToCall), 
        m_bOneShoted(bOneShoted), 
        m_bReady(false) { }

    /*--------------------------------------
    * set_ticks()
    * -------------------------------------*/
    bool base_tickhook_entry::set_ticks(unsigned int uiTicks) {
        automutx_t lock(m_mutexEntry);

        if(m_bReady) return false;
        m_iTicksToCall = uiTicks; return true;
    }
    /*--------------------------------------
    * set_oneshot()
    * -------------------------------------*/
    bool base_tickhook_entry::set_oneshot(bool bIsOneShot) {
        automutx_t lock(m_mutexEntry);

        if(m_bReady) return false;
        m_bOneShoted = bIsOneShot; return true;
    }
    /*--------------------------------------
    * start()
    * -------------------------------------*/
    void base_tickhook_entry::start() {
        automutx_t lock(m_mutexEntry);

        m_bReady = true;
    }
    /*--------------------------------------
    * stop()
    * -------------------------------------*/
    void base_tickhook_entry::stop() {
        automutx_t lock(m_mutexEntry);

        m_bReady = false;
    }
    /*--------------------------------------
    * is_oneshoted()
    * -------------------------------------*/
    bool base_tickhook_entry::is_oneshoted() { 
        automutx_t lock(m_mutexEntry);

        return m_bOneShoted; 
    }
    /*--------------------------------------
    * is_ready()
    * -------------------------------------*/
    bool base_tickhook_entry::is_ready() { 
        automutx_t lock(m_mutexEntry);

        return m_bReady; 
    }
    /*--------------------------------------
    * get_ticks()
    * -------------------------------------*/
    unsigned int base_tickhook_entry::get_ticks() { 
        automutx_t lock(m_mutexEntry);
        
        return m_iTicksToCall; 
    }
}


#endif