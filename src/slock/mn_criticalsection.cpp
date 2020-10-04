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


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "slock/mn_criticalsection.hpp"

#include "mn_error.hpp"

basic_critical_section::basic_critical_section() {
    m_pHandle = portMUX_INITIALIZER_UNLOCKED;
}
basic_critical_section::basic_critical_section(portMUX_TYPE type) {
    m_pHandle = type;
}

int basic_critical_section::lock(unsigned int timeout) {
    ((void)timeout);

    portENTER_CRITICAL_SAFE(&m_pHandle);

    return NO_ERROR;
}
int basic_critical_section::unlock() {
    portEXIT_CRITICAL_SAFE(&m_pHandle);

    return NO_ERROR;
}