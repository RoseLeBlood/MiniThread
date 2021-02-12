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
#include <mn_config.hpp>
#if MN_THREAD_CONFIG_USE_TRACE == 1

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <mn_basic_semaphore.hpp>
#include <trace/mn_semaphore_item.hpp>

#include <string>

namespace mn {
    namespace trace {
        const char* semaphore_trace_item::message() {
            std::string _retString;

            _retString += std::to_string( (uint32_t)m_pObject) + ":" + m_strName + ":" 
                + std::to_string( (uint32_t)m_State) + ";";

            return _retString.c_str();
        }
    }
}

#endif