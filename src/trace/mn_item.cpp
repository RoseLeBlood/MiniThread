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

#include <time.h>  
#include <freertos/FreeRTOS.h>    
#include <freertos/task.h>

#include "trace/mn_item.hpp"

basic_trace_item::time basic_trace_item::get_time() {
    time_t rawtime = ::time(NULL);
    struct tm* _time = localtime (&rawtime);

    basic_trace_item::time _retTime;

    _retTime.sec = _time->tm_sec;
    _retTime.min = _time->tm_min;
    _retTime.hour = _time->tm_hour;
    _retTime.ticks = xTaskGetTickCount();

    return _retTime;
}
#endif
