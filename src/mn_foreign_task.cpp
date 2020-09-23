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


foreign_task::foreign_task() 
  : foreign_task(xTaskGetCurrentTaskHandle()) { 

  m_strName = "current_foreign_task";
}

foreign_task::foreign_task(void* t)
  : basic_task() { 
  
  handle = t;

  m_strName = "foreign_task";
  m_iCore = xPortGetCoreID();
  m_iID = uxTaskGetTaskNumber(handle);
  m_usStackDepth = 0;
  m_bMutexInit = false;

  

  if (xPortInIsrContext()) {
    m_uiPriority = (basic_task::priority)uxTaskPriorityGetFromISR(handle);
  } else {
	  m_uiPriority = (basic_task::priority)uxTaskPriorityGet(handle);
  }
  m_pChild = NULL;
  m_pParent = NULL;
}

int foreign_task::__internal_create_usings_types() {
   if(!m_bMutexInit) {
    m_runningMutex = new LockType_t();
    m_contextMutext = new LockType_t();

    m_continuemutex = new LockType_t();
    m_continuemutex2 = new LockType_t();


    if(m_runningMutex->create() != ERR_MUTEX_OK)
      return ERR_TASK_CANTINITMUTEX;
    if(m_contextMutext->create() != ERR_MUTEX_OK)
      return ERR_TASK_CANTINITMUTEX;

    if(m_continuemutex->create() != ERR_MUTEX_OK)
      return ERR_TASK_CANTINITMUTEX;
    if(m_continuemutex2->create() != ERR_MUTEX_OK)
      return ERR_TASK_CANTINITMUTEX;

    m_bMutexInit = true;
    
  }
  m_continuemutex->lock();

	if (handle == 0) {
    m_continuemutex->unlock();
		return ERR_TASK_CANTSTARTTHREAD;
  }
  m_continuemutex->unlock();

	return ERR_TASK_OK;
}

foreign_task* foreign_task::get_idle_task() {
  void* rawHandle = xTaskGetIdleTaskHandle();

  return foreign_task::create_from(rawHandle);
}
foreign_task* foreign_task::get_idle_task(UBaseType_t cpuid) {
  void* rawHandle = xTaskGetIdleTaskHandleForCPU(cpuid);

  foreign_task* thread = foreign_task::create_from(rawHandle);

  if(thread) thread->m_iCore = cpuid;
  
  return thread;
}
foreign_task* foreign_task::create_from(void* foreign_handle, int* ret_error ) {
    if(foreign_handle == NULL) return NULL;

    int _error_ret = 0;
    foreign_task* newThread = new foreign_task(foreign_handle);

    _error_ret = newThread->__internal_create_usings_types();
    if(ret_error) *ret_error = _error_ret;

    return newThread;
}

#endif