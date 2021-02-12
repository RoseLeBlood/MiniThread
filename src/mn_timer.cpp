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
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#include "mn_error.hpp"
#include "mn_timer.hpp"

namespace mn {
    //-----------------------------------
    //  constructor
    //-----------------------------------
    basic_timer::basic_timer(const char * strName, unsigned int uiPeriod, bool bIsOneShot)
    : m_bIsOneShot(bIsOneShot), m_uiPeriod(uiPeriod), m_strName(strName), m_pHandle(NULL)  { }


    //-----------------------------------
    //  create
    //-----------------------------------
    int basic_timer::create() {
        if(m_pHandle != 0) return ERR_TIMER_ALREADYINIT;

        #if( configSUPPORT_STATIC_ALLOCATION == 1 )

        m_pHandle = xTimerCreateStatic(m_strName, m_uiPeriod, 
                                m_bIsOneShot ? pdFALSE : pdTRUE,
                                this, runtimerstub, 
                                &m_xTimerBuffer);

        #else
        
        m_pHandle = xTimerCreate(m_strName, m_uiPeriod, 
                                m_bIsOneShot ? pdFALSE : pdTRUE,
                                this, runtimerstub);
        #endif
        
        if (m_pHandle == NULL) 
            return ERR_TIMER_CANTCREATE;

        m_iTimerID = ( int32_t )pvTimerGetTimerID(m_pHandle);

        return ERR_TIMER_OK;
    }

    //-----------------------------------
    //  destroy
    //-----------------------------------
    int basic_timer::destroy(unsigned int timeout) {
        if (m_pHandle == NULL) return ERR_TIMER_NOTCREATED;

        xTimerDelete(m_pHandle, timeout);

        return ERR_TIMER_OK;
    }

    //-----------------------------------
    //  active
    //-----------------------------------
    int basic_timer::active(unsigned int timeout) {
        if (m_pHandle == NULL) return ERR_TIMER_NOTCREATED;
        
        BaseType_t success = 0;

        if (xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xTimerStartFromISR( m_pHandle, &xHigherPriorityTaskWoken );
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
    } else {
            success = xTimerStart(m_pHandle, timeout);
    }

    return success == pdTRUE ? ERR_TIMER_OK : ERR_TIMER_AKTIVATE;
    }

    //-----------------------------------
    //  inactive
    //-----------------------------------
    int basic_timer::inactive(unsigned int timeout) {
        if (m_pHandle == NULL) return ERR_TIMER_NOTCREATED;
        
        BaseType_t success = 0;

        if (xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xTimerStopFromISR( m_pHandle, &xHigherPriorityTaskWoken );
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
    } else {
            success = xTimerStop(m_pHandle, timeout);
    }

    return success == pdTRUE ? ERR_TIMER_OK : ERR_TIMER_INAKTIVATE;
    }

    //-----------------------------------
    //  reset
    //-----------------------------------
    int basic_timer::reset(unsigned int timeout) {
        if (m_pHandle == NULL) return ERR_TIMER_NOTCREATED;
        
        BaseType_t success = 0;

        if (xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xTimerResetFromISR( m_pHandle, &xHigherPriorityTaskWoken );
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
    } else {
            success = xTimerReset(m_pHandle, timeout);
    }

    return success == pdTRUE ? ERR_TIMER_OK : ERR_TIMER_RESET;
    }

    //-----------------------------------
    //  set_period
    //-----------------------------------
    bool basic_timer::set_period(unsigned int uiNewPeriod, unsigned int timeout) {
        if (m_pHandle == NULL) return false;

        BaseType_t success = 0;
        
        if (xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xTimerChangePeriodFromISR( m_pHandle, uiNewPeriod, &xHigherPriorityTaskWoken );
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
    } else {
            success = xTimerChangePeriod(m_pHandle, uiNewPeriod, timeout);
    }

    return success == pdTRUE ;
    }

    //-----------------------------------
    //  runtimerstub
    //-----------------------------------
    void basic_timer::runtimerstub(void* xTimer) {
        configASSERT( xTimer );

        basic_timer *timer = static_cast<basic_timer *>(pvTimerGetTimerID(xTimer));

        timer->on_enter();
        timer->on_timer();
        timer->on_exit();
    }

    //-----------------------------------
    //  set_id
    //-----------------------------------
    void basic_timer::set_id(int nId) {
        vTimerSetTimerID(m_pHandle, &nId);
        m_iTimerID = ( int32_t )pvTimerGetTimerID(m_pHandle);
    }

    //-----------------------------------
    //  is_running
    //-----------------------------------
    bool basic_timer::is_running() {
        return (xTimerIsTimerActive( m_pHandle ) == pdTRUE );
    }
}