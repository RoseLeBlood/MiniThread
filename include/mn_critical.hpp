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
#include "slock/mn_interrupts_lock.hpp"
#include "slock/mn_schedular_lock.hpp"

namespace mn {
    namespace system {
                
        /** 
         * @code{c}
         * void function() {
         *  critical_section_t critical_section;
         *  CRITICAL_SECTION(critical_section) {
         *      // locked section code goes hier
         *  }
         *  // unlocked section  code goes hier
         * } 
         * @endcode
         */ 
        #define CRITICAL_SECTION(OBJECT) if( (bool)(auto_critical_t lock(OBJECT)) )

        /**
         * @code{c}
         * void function() {
         *  critical_section_timedout_t critical_section;
         *  CRITICAL_SECTION_TIMEDOUT(critical_section) {
         *      // locked section code goes hier
         *  }
         *  // unlocked section  code goes hier
         * } 
         * @endcode
         */ 
        #define CRITICAL_SECTION_TIMEDOUT(OBJECT) if( (bool)(auto_critical_section_timedout_t lock(OBJECT)) )
        /**
         * @code{c}
         * void function() {
         *  critical_section_nested_t critical_section;
         *  CRITICAL_SECTION_NESTED(critical_section) {
         *      // locked section code goes hier
         *  }
         *  // unlocked section  code goes hier
         * } 
         * @endcode
         */
        #define CRITICAL_SECTION_NESTED(OBJECT) if( (bool)(auto_critical_section_nested_t lock(OBJECT)) )


        /** 
         * @code{c}
         * void function() {
         *  interrupts_lock_t interrupts_lock;
         *  INTERRUPTS_LOCK(interrupts_lock) {
         *      // locked section code goes hier
         *  }
         *  // unlocked section  code goes hier
         * } 
         * @endcode
         */ 
        #define INTERRUPTS_LOCK(OBJECT) if( (bool)(auto_interrupts_lock_t lock(OBJECT)) )

        /** 
         * @code{c}
         * void function() {
         *  schedular_lock_t schedlock;
         *  SCHEDULAR_LOCK(schedlock) {
         *      // locked section code goes hier
         *  }
         *  // unlocked section  code goes hier
         * } 
         * @endcode
         */ 
        #define SCHEDULAR_LOCK(OBJECT) if( (bool)(auto_schedular_lock_t lock(OBJECT)) )

        /** using for the basic_critical_section class, for miniTask type design */
        using critical_section_t = basic_critical_section;
        /** using for the basic_critical_section_timedout class, for miniTask type design */
        using critical_section_timedout_t = basic_critical_section_timedout;
        /** using for the basic_critical_section_nested class, for miniTask type design */
        using critical_section_nested_t = basic_critical_section_nested;

        /** using for the basic_interrupts_lock class, for miniTask type design */
        using interrupts_lock_t = basic_interrupts_lock;

        /** using for the basic_schedular_lock class, for miniTask type design */
        using schedular_lock_t = basic_schedular_lock;

        /** A autolock guard type for basic_critical_lock objects */
        using auto_critical_section_t = basic_autolock<basic_critical_section>;
        /** A autolock guard type for critical_section_timedout_t objects */
        using auto_critical_section_timedout_t = basic_autolock<critical_section_timedout_t>;
        /** A autolock guard type for critical_section_nested_t objects */
        using auto_critical_section_nested_t = basic_autolock<critical_section_nested_t>;

        /** A autolock guard type for interrupts_lock_t objects */
        using auto_interrupts_lock_t = basic_autolock<interrupts_lock_t>;
        /** A autolock guard type for schedular_lock_t objects */
        using auto_schedular_lock_t = basic_autolock<schedular_lock_t>;
    }
}
#endif