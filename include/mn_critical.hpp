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
#ifndef _MINLIB_CRITICAL_H_ 
#define _MINLIB_CRITICAL_H_

#include "mn_error.hpp"
#include "mn_micros.hpp"
#include "mn_autolock.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "slock/mn_criticalsection.hpp"

/** Helper macro's for critical sections */ 
#define CRITICAL_SECTION(OBJECT) if( (bool)(auto_critical_t lock(OBJECT)) )
#define CRITICAL_SECTION_TIMEDOUT(OBJECT) if( (bool)(auto_critical_section_timedout_t lock(OBJECT)) )
#define CRITICAL_SECTION_NESTED(OBJECT) if( (bool)(auto_critical_section_nested_t lock(OBJECT)) )

/** using for the basic_critical_section class, for miniTask type design */
using critical_section_t = basic_critical_section;
/** using for the basic_critical_section_timedout class, for miniTask type design */
using critical_section_timedout_t = basic_critical_section_timedout;
/** using for the basic_critical_section_nested class, for miniTask type design */
using critical_section_nested_t = basic_critical_section_nested;

/** A autolock guard type for basic_critical_lock objects */
using auto_critical_section_t = basic_autolock<basic_critical_section>;
/** A autolock guard type for critical_section_timedout_t objects */
using auto_critical_section_timedout_t = basic_autolock<critical_section_timedout_t>;
/** A autolock guard type for critical_section_nested_t objects */
using auto_critical_section_nested_t = basic_autolock<critical_section_nested_t>;

#endif