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
#include "mn_config.hpp"

#include <esp_log.h>

#include "mn_eventgroup.hpp"
#include "mn_error.hpp"
#include "excp/mn_eventgroup_exception.hpp"


namespace mn {

    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group() {
		init_internal();
    }

    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group(EventGroupHandle_t handle)
        : m_pHandle(handle) {
        	if( !is_init() ) ESP_LOGE("event_group", "the given handle is NULL this group will not work!!");
	}

    //-----------------------------------
    //  deconstrutor
    //-----------------------------------
    basic_event_group::~basic_event_group() {
        if( is_init() ) {
            vEventGroupDelete(m_pHandle);
        }
    }

    //-----------------------------------
    //  sync
    //-----------------------------------
    EventBits_t basic_event_group::sync( const EventBits_t bitstoset, const EventBits_t bitstowaitfor,
                                        TickType_t timeout) {
		if(! is_init() ) init_internal();

		if( is_init() )
			return xEventGroupSync( m_pHandle, bitstoset, bitstowaitfor, timeout);
		else
			return portMAX_DELAY;
    }

    //-----------------------------------
    //  wait
    //-----------------------------------
    EventBits_t basic_event_group::wait( const EventBits_t uxBitsToWaitFor,
                                        bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout) {
		if(! is_init() ) init_internal();

		if( is_init() )
			return xEventGroupWaitBits( m_pHandle,
										uxBitsToWaitFor,
										xClearOnExit ? pdTRUE : pdFALSE,
										xWaitForAllBits ? pdTRUE : pdFALSE,
										timeout);
		else
			return portMAX_DELAY;
    }

    //-----------------------------------
    //  clear
    //-----------------------------------
    EventBits_t basic_event_group::clear(const EventBits_t uxBitsToClear) {
    	if(! is_init() ) return portMAX_DELAY;

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
    	if(! is_init() ) return portMAX_DELAY;

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
    	if(! is_init() ) init_internal();

        EventBits_t success;

        if(xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xEventGroupSetBitsFromISR(m_pHandle, uxBitsToSet, &xHigherPriorityTaskWoken);
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
        } else {
            success = xEventGroupSetBits(m_pHandle, (EventBits_t)uxBitsToSet);
        }
        return success;
    }

    //-----------------------------------
    //  init_internal
    //-----------------------------------
    void basic_event_group::init_internal() {
		 #if( configSUPPORT_STATIC_ALLOCATION == 1 )
		 	// Create the event group without dynamically allocating any memory.
            m_pHandle = xEventGroupCreateStatic(&m_xEventGroupBuffer);
        #else
            m_pHandle = xEventGroupCreate();
        #endif

        #if MN_THREAD_CONFIG_USE_EXCEPTIONS == MN_THREAD_CONFIG_YES
        	throw new mn::error::eventgroup_exception();
        #endif // MN_THREAD_CONFIG_USE_EXCEPTIONS

        if( m_pHandle == NULL ) ESP_LOGE("event_group", "out of mem xEventGroupCreate - failed");
	}

	int basic_event_group::create() {
		if( m_pHandle == NULL ) init_internal();

		return m_pHandle != NULL ? NO_ERROR : ERR_MNTHREAD_NULL;
	}
}
