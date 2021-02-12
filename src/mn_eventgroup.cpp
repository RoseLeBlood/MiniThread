/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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

#include "mn_eventgroup.hpp"


namespace mn {
    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group() {

        #if( configSUPPORT_STATIC_ALLOCATION == 1 )
            m_pHandle = xEventGroupCreateStatic(&m_xEventGroupBuffer);
        #else
            m_pHandle = xEventGroupCreate();
        #endif
    }

    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group(EventGroupHandle_t handle) 
        : m_pHandle(handle) { }

    //-----------------------------------
    //  deconstrutor
    //-----------------------------------
    basic_event_group::~basic_event_group() {
        if(m_pHandle != NULL) {
            vEventGroupDelete(m_pHandle);
        }
    }

    //-----------------------------------
    //  sync
    //-----------------------------------
    EventBits_t basic_event_group::sync( const EventBits_t bitstoset, const EventBits_t bitstowaitfor,
                                        TickType_t timeout) {
        
        return xEventGroupSync( m_pHandle, bitstoset, bitstowaitfor, timeout);
    }

    //-----------------------------------
    //  wait
    //-----------------------------------
    EventBits_t basic_event_group::wait( const EventBits_t uxBitsToWaitFor,
                                        bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout) {
        
        return xEventGroupWaitBits( m_pHandle,
                                    uxBitsToWaitFor,
                                    xClearOnExit ? pdTRUE : pdFALSE,
                                    xWaitForAllBits ? pdTRUE : pdFALSE,
                                    timeout);
    }

    //-----------------------------------
    //  clear
    //-----------------------------------
    EventBits_t basic_event_group::clear(const EventBits_t uxBitsToClear) {
        if(xPortInIsrContext()) {
            return xEventGroupClearBitsFromISR(m_pHandle, uxBitsToClear);
        } else {
            return xEventGroupClearBits(m_pHandle, uxBitsToClear);
        }
    }

    //-----------------------------------
    //  get
    //-----------------------------------
    EventBits_t basic_event_group::get() {
        if(xPortInIsrContext()) {
            return xEventGroupGetBitsFromISR(m_pHandle);
        } else {
            return xEventGroupGetBits(m_pHandle);
        }
    }

    //-----------------------------------
    //  set
    //-----------------------------------
    EventBits_t basic_event_group::set(const EventBits_t uxBitsToSet) {
        EventBits_t success;

        if(xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xEventGroupSetBitsFromISR(m_pHandle, uxBitsToSet, &xHigherPriorityTaskWoken);
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
        } else {
            success = xEventGroupSetBits(m_pHandle, uxBitsToSet);
        }
        return success;
    }
}