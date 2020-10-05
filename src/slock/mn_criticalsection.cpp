/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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


#define _MINLIB_CRITICAL_H_
#include "slock/mn_criticalsection.hpp"

#include "mn_error.hpp"

//-----------------------------------
//  basic_critical_section::basic_critical_section()
//-----------------------------------
basic_critical_section::basic_critical_section() {
    m_pHandle = portMUX_INITIALIZER_UNLOCKED;
}
//-----------------------------------
//  basic_critical_section::basic_critical_section(portMUX_TYPE type)
//-----------------------------------
basic_critical_section::basic_critical_section(portMUX_TYPE type) {
    m_pHandle = type;
}
//-----------------------------------
//  basic_critical_section::lock(unsigned int timeout)
//-----------------------------------
int basic_critical_section::lock(unsigned int timeout) {
    ((void)timeout);

    portENTER_CRITICAL_SAFE(&m_pHandle);

    return NO_ERROR;
}
//-----------------------------------
//  basic_critical_section::unlock()
//-----------------------------------
int basic_critical_section::unlock() {
    portEXIT_CRITICAL_SAFE(&m_pHandle);

    return NO_ERROR;
}
//-----------------------------------
//  basic_critical_section_timedout::basic_critical_section_timedout()
//-----------------------------------
basic_critical_section_timedout::basic_critical_section_timedout() {
    vPortCPUInitializeMutex(&m_pHandle);
}
//-----------------------------------
//  basic_critical_section_timedout::lock(unsigned int timeout) 
//-----------------------------------
int basic_critical_section_timedout::lock(unsigned int timeout) {
    bool _bReturn = false;

    if(timeout == UINT_MAX)
        _bReturn = vPortCPUAcquireMutexTimeout(&m_pHandle, portMUX_NO_TIMEOUT);
    else
        _bReturn = vPortCPUAcquireMutexTimeout(&m_pHandle, (int)timeout);

    return (_bReturn) ? ERR_SYSTEM_OK : ERR_SYSTEM_LOCK;
}

//-----------------------------------
//  basic_critical_section_timedout::time_lock(const struct timespec *abs_time)
//-----------------------------------
int basic_critical_section_timedout::time_lock(const struct timeval *abs_time) {
  struct timeval now;
  gettimeofday(&now, NULL);

  auto _time = (*abs_time) - now;

  return lock(time_to_ticks(&_time));
}
//-----------------------------------
//  basic_critical_section_timedout::try_lock()
//-----------------------------------
bool basic_critical_section_timedout::try_lock() {
    return ( lock(portMUX_TRY_LOCK) == ERR_SYSTEM_OK);
}

//-----------------------------------
//  basic_critical_section_timedout::unlock()
//-----------------------------------
int basic_critical_section_timedout::unlock() {
    bool _bReturn = false;
    vPortCPUReleaseMutex(&m_pHandle);

    return ERR_SYSTEM_NO_RETURN;
}


//-----------------------------------
//  basic_critical_section_nested::lock(unsigned int t)
//-----------------------------------
int basic_critical_section_nested::lock(unsigned int t) {
    m_iState = portENTER_CRITICAL_NESTED();
    return ERR_SYSTEM_NO_RETURN;
}
//-----------------------------------
//  basic_critical_section_nested::unlock()
//-----------------------------------
int basic_critical_section_nested::unlock() {
    portEXIT_CRITICAL_NESTED(m_iState);
    return ERR_SYSTEM_NO_RETURN;
}