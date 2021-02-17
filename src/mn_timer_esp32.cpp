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


#include "mn_error.hpp"
#include "esp_timer.h"
#include "mn_timer_esp32.hpp"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

namespace mn {
    namespace esp32 {
        
        basic_esp32_timer::basic_esp32_timer(const char * strName, unsigned int uiPeriod, bool bIsOneShot) {
            m_bIsOneShot = bIsOneShot;
            m_uiPeriod = uiPeriod;
            m_strName = strName;
            m_bIsInit = false;
            m_bIsRunning = false;
        }
        int basic_esp32_timer::create() {
            if(m_bIsInit) return ERR_TIMER_ALREADYINIT;

            /*
             esp_timer_cb_t callback;        //!< Function to call when timer expires
    void* arg;                      //!< Argument to pass to the callback
    esp_timer_dispatch_t dispatch_method;   //!< Call the callback from task or from ISR
    const char* name;               //!< Timer name, used in esp_timer_dump function
    bool skip_unhandled_events;     //!< Skip unhandled events for periodic timers
            */
            esp_timer_create_args_t _timerArgs;
            _timerArgs.arg = (void*) this;
            _timerArgs.callback = &runtimerstub;
            _timerArgs.name = m_strName;
        

            if(esp_timer_create(&_timerArgs, &m_pHandle) != ESP_OK) return ERR_TIMER_CANTCREATE;

            m_bIsInit = true;

            return ERR_TIMER_OK;
        }  
        int basic_esp32_timer::destroy(unsigned int timeout) {
            if(!m_bIsInit) return ERR_TIMER_NOTCREATED;

            if(m_bIsRunning) {
                if(esp_timer_stop(m_pHandle) != ESP_OK) 
                    return ERR_TIMER_INAKTIVATE;
            }
            if(esp_timer_delete(m_pHandle) != ESP_OK) 
                return ERR_UNKN;

            return ERR_TIMER_OK; //
        } 
        int basic_esp32_timer::active(unsigned int timeout) {
            if(!m_bIsInit) return ERR_TIMER_NOTCREATED;
            if(!m_bIsRunning) return ERR_TIMER_OK;

            
            if(!m_bIsOneShot) {
                if(esp_timer_start_periodic(m_pHandle, m_uiPeriod) != ESP_OK) return ERR_TIMER_AKTIVATE;
            } else {
                if(esp_timer_start_once(m_pHandle, m_uiPeriod)  != ESP_OK)  return ERR_TIMER_AKTIVATE;
            }
            m_bIsRunning = true;
            

            return ERR_TIMER_OK;
        }
        int basic_esp32_timer::inactive(unsigned int timeout) {
            if(!m_bIsInit) return ERR_TIMER_NOTCREATED;

            if(!m_bIsRunning) {
                if(esp_timer_stop(m_pHandle) != ESP_OK) return ERR_TIMER_INAKTIVATE;
                m_bIsRunning = false;
            }
            return ERR_TIMER_OK;
        }
        int basic_esp32_timer::reset(unsigned int timeout) {
            if(!m_bIsInit) return ERR_TIMER_NOTCREATED;

            if(esp_timer_stop(m_pHandle) != ESP_OK) return ERR_TIMER_INAKTIVATE;
            if(esp_timer_start_periodic(m_pHandle, m_uiPeriod) != ESP_OK) return ERR_TIMER_RESET;

            return ERR_TIMER_OK;
        }
        bool basic_esp32_timer::set_period(unsigned int uiNewPeriod, unsigned int timeout) {
            if(!m_bIsInit) return ERR_TIMER_NOTCREATED;

            if(esp_timer_stop(m_pHandle) != ESP_OK) return false;
            if(esp_timer_start_periodic(m_pHandle, uiNewPeriod) != ESP_OK) return false;

            m_uiPeriod = uiNewPeriod;

            return true;
        }
        void*  basic_esp32_timer::get_handle() {
            return (void*)m_pHandle;
        }

        void basic_esp32_timer::runtimerstub(void* xTimer) {
            basic_esp32_timer* timer = (basic_esp32_timer*)xTimer;

            if(timer) {
                timer->on_enter();
                timer->on_timer();
                timer->on_exit();

                if(timer->m_bIsOneShot) 
                    timer->m_bIsRunning = false;
            }
        }
    }
}