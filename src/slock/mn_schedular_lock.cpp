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

#include "slock/mn_schedular_lock.hpp"
#include "mn_error.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace mn {
    namespace system {
        int basic_schedular_lock::lock(unsigned int timeout) {
            vTaskSuspendAll();
            return ERR_SYSTEM_NO_RETURN;
        }
        int basic_schedular_lock::unlock() {
            return xTaskResumeAll() ? ERR_SYSTEM_OK : ERR_SYSTEM_UNLOCK;
        }
    }
}