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
#ifndef _MINLIB_CITCALLOCK_NEW_H_
#define _MINLIB_CITCALLOCK_NEW_H_

#include "mn_system_lock.hpp"

/**
 * Macro for critical sections
 * 
 * 
 * @example @code 
 * critical_section_t criticalSection;
 * CRITICAL_SECTION(criticalSection) {
 *  //locked
 * }
 * //unlocked
 * @endcode
 * 
 * @ingroup lock
 */ 
#define CRITICAL_SECTION(OBJECT) if( (bool)(auto_critical_t lock(OBJECT)) )

/**
 * FreeRTOS miniTask lockk-wrapper for critical sections
 * Can you use im ISR Context
 * 
 * @ingroup Interface
 * @ingroup lock
 */ 
class basic_critical_section : public ISystemLockObject {
public:
    basic_critical_section();
    basic_critical_section(portMUX_TYPE type);

    /**
     * enter the critical section
     * @param timeout Not used
     * 
     * @return Only NO_ERROR
     */
	virtual int lock(unsigned int timeout = 0);
    /**
     * leave the critical section
     * @return Only NO_ERROR
     */
	virtual int unlock();
protected:
    portMUX_TYPE m_pHandle;
};

using critical_section_t = basic_critical_section;





#endif