// *** ADDED BY HEADER FIXUP ***
#include <ctime>
// *** END ***
/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef _MINILIB_NANOSLEEP_H_
#define _MINILIB_NANOSLEEP_H_

#include "mn_config.hpp"

#include <errno.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <time.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "mn_basic_timespan.hpp"

MN_EXTERNC_BEGINN
/**
 * @ingroup base
 */

#define MN_DELAY_SEC(sec)		mn::delay(timespan_t(0,0,0,sec,0))
#define MN_DELAY_MICROS(ms)		mn::delay(timespan_t(0,0,0,0,ms))

namespace mn {

    /**
     * Delay a task for a given timespan.
     * @param ts The amount of time, as timespan, that the calling task should block.
     */
    void delay(const timespan_t& ts);

    /**
     * @brief Delay a task until a specified time.
     * @param tsPreviousWakeTime The time at which the task was last unblocked.
     *
     * @param uiTimeIncrement The cycle time period.  The task will be unblocked at
	 * time tsPreviousWakeTime + uiTimeIncrement.  Calling vTaskDelayUntil with the
	 * same uiTimeIncrement parameter value will cause the task to execute with
	 * a fixed interface period.
     */
    unsigned int delay_until( timespan_t& tsPreviousWakeTime, const unsigned int& uiTimeIncrement);
    /**
     * Delay a task for a given timespan_t and return in to rem the elapsed mircosecs as timespan_t
     * @param req The amount of time, as timespan, that the calling task should block.
     * @param[out] rem The elapsed mircosecs as timespan_t
     *
     * @note mircosecs > 999999999
     */
    void ndelay(const timespan_t& req, timespan_t* rem);

}

MN_EXTERNC_END

#endif
