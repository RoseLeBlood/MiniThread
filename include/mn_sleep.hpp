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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/signal.h>

/**
 * @ingroup base
 */

namespace mn {
    /**
     * Delay a given task for a given number of seconds.  
     */ 
    unsigned sleep(unsigned int secs);
    /**
     * Delay a given task for a given number of micro seconds.
     */ 
    int usleep(useconds_t usec);
    /**
     * Delay a given task for a given timespec 
     */ 
    int nsleep(const struct timespec *req, struct timespec *rem);
}
#endif
