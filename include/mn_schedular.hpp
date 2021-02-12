/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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

#ifndef _MINLIB_VECTOR_SCHEDULAR_H_
#define _MINLIB_VECTOR_SCHEDULAR_H_

#include "mn_error.hpp"
#include "mn_config.hpp"

namespace mn {
    /**
     * FreeRTOS schedular wrapper util
     * \ingroup util
     */ 
    class basic_schedular {
    public:
        /**
         * Start the schedular
         * @note On ESP32 only return NO_ERROR. 
         * @return - NO_ERROR: No error - schedular started 
         *         - ERR_UNKN: Error in FreeRTOS - schedular can't start 
         */ 
        static int start();
        /**
         * Stop the schedular
         *  @note On ESP32 not use Text from IDF-SDK: It is unlikely that the Xtensa port will get stopped.  If required simply disable the tick interrupt here. 
         */ 
        static void stop();

        static void yieldOtherCore(int core) {
        vPortYieldOtherCore(core);
        }
        static uint32_t get_hz() {
            return (uint32_t)configTICK_RATE_HZ;
        }
    };

    using schedular = basic_schedular;
}
#endif