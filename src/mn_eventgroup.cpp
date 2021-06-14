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

#include <stdio.h>

#include <esp_log.h>



#include "mn_eventgroup.hpp"
#include "mn_error.hpp"
#include "excp/mn_eventgroup_exception.hpp"


namespace mn {

    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group(const char* strName)
    	: m_pHandle(nullptr) {
		sprintf(m_strName, "evg_%s", strName);
    }



    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_event_group::basic_event_group(EventGroupHandle_t handle)
        : m_pHandle(handle) {
        	if( !is_init() ) ESP_LOGE(m_strName, "the given handle is NULL this group will not work!!");
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

		if( is_init() )
			return xEventGroupSync( m_pHandle, bitstoset, bitstowaitfor, timeout);
		else {
			ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
		}
    }

    //-----------------------------------
    //  is_bit
    //-----------------------------------
    bool basic_event_group::is_bit( const EventBits_t uxBitToWaitFor, uint32_t timeout) {

		EventBits_t _uxBits;

		_uxBits = wait(uxBitToWaitFor, false, true, timeout);
		return ( ( _uxBits & uxBitToWaitFor ) != 0 );
    }

    //-----------------------------------
    //  wait
    //-----------------------------------
    EventBits_t basic_event_group::wait( const EventBits_t uxBitsToWaitFor,
                                        bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout) {

		EventBits_t _uxBits = 0;

		if( is_init() )
			_uxBits = xEventGroupWaitBits( m_pHandle,
										uxBitsToWaitFor,
										xClearOnExit ? pdTRUE : pdFALSE,
										xWaitForAllBits ? pdTRUE : pdFALSE,
										timeout);
		else {
			ESP_LOGE(m_strName, "the event group handle is not created, call create first");
		}



		return _uxBits;
    }

    //-----------------------------------
    //  clear
    //-----------------------------------
    EventBits_t basic_event_group::clear(const EventBits_t uxBitsToClear) {
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

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
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

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
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

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
		#else
			if( m_pHandle == NULL ) ESP_LOGE(m_strName, "out of mem xEventGroupCreate - failed");
        #endif // MN_THREAD_CONFIG_USE_EXCEPTIONS
	}

	//-----------------------------------
    //  create
    //-----------------------------------
	int basic_event_group::create() {
		if( m_pHandle == nullptr )
			init_internal();

		return m_pHandle != nullptr ? NO_ERROR : ERR_MNTHREAD_NULL;
	}

	//-----------------------------------
    //  set_name
    //-----------------------------------
	void basic_event_group::set_name(const char* strName) {
		ESP_LOGI(m_strName, "rename the event group to %s", strName);
		sprintf(m_strName, "evg_%s", strName);
    }
}
