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
#include "mn_foreign_task.hpp"

#if MN_THREAD_CONFIG_FOREIGIN_TASK_SUPPORT == MN_THREAD_CONFIG_YES

namespace mn {
  namespace ext {
    //-----------------------------------
    //  construtor
    //-----------------------------------
    foreign_task::foreign_task() 
      : foreign_task(xTaskGetCurrentTaskHandle()) { 

      m_strName = "current_foreign_task";
    }

    //-----------------------------------
    //  construtor
    //-----------------------------------
    foreign_task::foreign_task(void* t)
      : basic_task() { 
      
      m_pHandle = t;

      m_strName = "foreign_task";
      m_iCore = xPortGetCoreID();
      m_iID = uxTaskGetTaskNumber(m_pHandle);
      m_usStackDepth = 0;

      if (xPortInIsrContext()) {
        m_uiPriority = (basic_task::priority)uxTaskPriorityGetFromISR(m_pHandle);
      } else {
        m_uiPriority = (basic_task::priority)uxTaskPriorityGet(m_pHandle);
      }
      m_pChild = NULL;
      m_pParent = NULL;
    }

    //-----------------------------------
    //  __internal_create_usings_types
    //-----------------------------------
    int foreign_task::__internal_create_usings_types() {
      m_continuemutex.lock();

      if (m_pHandle == 0) {
        m_continuemutex.unlock();
        return ERR_TASK_CANTSTARTTHREAD;
      }
      m_continuemutex.unlock();

      return ERR_TASK_OK;
    }

    //-----------------------------------
    //  get_idle_task
    //-----------------------------------
    foreign_task* foreign_task::get_idle_task() {
      void* rawHandle = xTaskGetIdleTaskHandle();

      return foreign_task::create_from(rawHandle);
    }

    //-----------------------------------
    //  get_idle_task
    //-----------------------------------
    foreign_task* foreign_task::get_idle_task(UBaseType_t cpuid) {
      void* rawHandle = xTaskGetIdleTaskHandleForCPU(cpuid);

      foreign_task* thread = foreign_task::create_from(rawHandle);

      if(thread) thread->m_iCore = cpuid;
      
      return thread;
    }

    //-----------------------------------
    //  create_from
    //-----------------------------------
    foreign_task* foreign_task::create_from(void* foreign_handle, int* ret_error ) {
        if(foreign_handle == NULL) return NULL;

        int _error_ret = 0;
        foreign_task* newThread = new foreign_task(foreign_handle);

        _error_ret = newThread->__internal_create_usings_types();
        if(ret_error) *ret_error = _error_ret;

        return newThread;
    }
  }
}
#endif