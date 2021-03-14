/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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

#ifndef __LIBMIN_THREAD_H_
#define __LIBMIN_THREAD_H_

#include "mn_config.hpp"

#define configUSE_IDLE_HOOK 1
#define configUSE_DAEMON_TASK_STARTUP_HOOK 1

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/queue.h"


#include "mn_version.hpp"
#include "mn_autolock.hpp"
#include "mn_micros.hpp"
#include "mn_task.hpp"
#include "mn_tasklet.hpp"
#include "mn_eventgroup.hpp"

#include "mn_critical.hpp"
#include "mn_timer.hpp"

#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES
#include "mn_convar.hpp"
#include "mn_convar_task.hpp"
#endif

#include "queue/mn_queue.hpp"
#include "queue/mn_binaryqueue.hpp"
#include "queue/mn_deque.hpp"
#include "queue/mn_workqueue.hpp"

#include "mn_ringbuffer.hpp"
#include "memory/mn_mempool.hpp"
#include "mn_shared.hpp"


#include "mn_container.hpp"
#include "mn_atomic.hpp"
#include "mn_safecounter.hpp"
#include "mn_pointer.hpp"


#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES

#endif

namespace mn {
    void panic();
}

#endif
