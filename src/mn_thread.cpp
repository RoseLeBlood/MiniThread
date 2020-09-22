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
#include "mn_thread.hpp"
#include <stdio.h>


basic_thread::basic_thread(char const* strName, basic_thread::priority uiPriority,
     unsigned short  usStackDepth) {
  m_strName = strName;
  m_uiPriority = uiPriority;
  m_usStackDepth = usStackDepth;
  m_bMutexInit = false;
  m_bRunning = false;

  m_pChild = NULL;
  m_pParent = NULL;
}
basic_thread::~basic_thread() {
  vTaskDelete(handle); 

  if(!m_bRunning) { 

    if(m_runningMutex != NULL) m_runningMutex->destroy();
    if(m_runningMutex != NULL) m_contextMutext->destroy();
    if(m_runningMutex != NULL) m_continuemutex->destroy();
    if(m_runningMutex != NULL) m_continuemutex2->destroy();

    m_bMutexInit =false; 
  }
}
basic_thread*  basic_thread::get_root() {
  autolock_t autolock(*m_contextMutext);

  basic_thread* root = 0;

  if(m_pParent != NULL) root = m_pParent->get_root();
  else root = m_pParent;

  return root;
}
basic_thread*  basic_thread::get_child() {
  autolock_t autolock(*m_runningMutex);

  basic_thread* child = 0;
  child = m_pChild;
  return child;
}

bool basic_thread::add_child_thread(basic_thread* thread) {
  autolock_t autolock(*m_runningMutex);

  bool ret = true;
   thread->m_pParent = this;
   if(m_pChild == NULL) m_pChild = thread;
   else ret = m_pChild->add_child_thread(thread);

  return ret;
}

int basic_thread::create(int uiCore) {
  if(!m_bMutexInit) {
    m_runningMutex = new LockType_t();
    m_contextMutext = new LockType_t();

    m_continuemutex = new LockType_t();
    m_continuemutex2 = new LockType_t();


    if(m_runningMutex->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;
    if(m_contextMutext->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;

    if(m_continuemutex->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;
    if(m_continuemutex2->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;

    m_bMutexInit = true;
  }
  m_continuemutex->lock();
	m_runningMutex->lock();
	if (m_bRunning)
	{
		m_runningMutex->unlock();
		m_continuemutex->unlock();
		return ERR_THREAD_ALREADYRUNNING;
	}
	m_runningMutex->unlock();

  if(uiCore == -1) {
    xTaskCreate(&runtaskstub, m_strName,
                 m_usStackDepth,
                this, (int)m_uiPriority, &handle);
  } else {
    xTaskCreatePinnedToCore(&runtaskstub, m_strName,
                m_usStackDepth,
                this, (int)m_uiPriority, &handle, uiCore);

  }

 

	if (handle == 0) {
    m_continuemutex->unlock();
		return ERR_THREAD_CANTSTARTTHREAD;
  }

  m_runningMutex->lock();
  m_iCore = uiCore;
  m_iID = uxTaskGetTaskNumber(handle); //get_new_id(); uxTaskGetTaskNumber
  on_create();
  m_continuemutex->unlock();
  m_runningMutex->unlock();

	return ERR_THREAD_OK;
}
int basic_thread::kill() {
  m_continuemutex->lock();
  m_runningMutex->lock();

	if (!m_bRunning) {
    m_runningMutex->unlock();
    m_continuemutex->unlock();

		return ERR_THREAD_NOTRUNNING;
	}
  vTaskDelete(handle); handle = 0;
  m_bRunning = false;
  on_kill();

  m_runningMutex->unlock();
  m_continuemutex->unlock();
  
	return ERR_THREAD_OK;
}
bool basic_thread::is_running() {
  autolock_t autolock(*m_runningMutex);
  return m_bRunning;
}
uint32_t basic_thread::get_id() {
  autolock_t autolock(*m_runningMutex);
	return m_iID;
}
uint32_t basic_thread::get_on_core() {
  autolock_t autolock(*m_runningMutex);
	return m_iCore;
}
const char* basic_thread::get_name() {
  autolock_t autolock(*m_runningMutex);

	return m_strName;
}
basic_thread::priority basic_thread::get_priority() {
  autolock_t autolock(*m_runningMutex);
  
  if(handle == NULL) return m_uiPriority;

  if (xPortInIsrContext()) {
    return (basic_thread::priority)uxTaskPriorityGetFromISR(handle);
  } else {
	  return (basic_thread::priority)uxTaskPriorityGet(handle);
  }
}
unsigned short basic_thread::get_stackdepth() {
  autolock_t autolock(*m_runningMutex);
	return m_usStackDepth;
}
xTaskHandle basic_thread::get_handle() {
  autolock_t autolock(*m_runningMutex);
	return handle;
}
void *basic_thread::get_return_value() {
	autolock_t autolock(*m_runningMutex);
	return (m_bRunning) ? NULL : m_retval;
}
uint32_t basic_thread::get_time_since_start() {
  autolock_t autolock(*m_runningMutex);
	return (uint32_t)(xTaskGetTickCount()*portTICK_PERIOD_MS);
}
void  basic_thread::set_priority(unsigned int uiPriority) {
  autolock_t autolock(*m_runningMutex);
  m_uiPriority = uiPriority;
  if(handle != NULL)
    vTaskPrioritySet(handle, uiPriority);
}
void basic_thread::suspend() {
  autolock_t autolock(*m_runningMutex);
  vTaskSuspend( handle );
}
void basic_thread::resume() {
  autolock_t autolock(*m_runningMutex);
  vTaskResume(handle);
}

void basic_thread::runtaskstub(void* parm) {
  basic_thread *esp_thread;
	void *ret;

	esp_thread = (static_cast<basic_thread*>(parm));

  esp_thread->m_continuemutex2->lock();
  esp_thread->m_runningMutex->lock();
	esp_thread->m_bRunning = true;
  esp_thread->m_runningMutex->unlock();

  esp_thread->m_continuemutex->lock();
	esp_thread->m_continuemutex->unlock();

  esp_thread->thread_started();
  ret = esp_thread->on_thread();
  esp_thread->on_cleanup();

  esp_thread->m_runningMutex->lock();
	esp_thread->m_bRunning = false;
	esp_thread->m_retval = ret;
  vTaskDelete(esp_thread->handle);
  esp_thread->handle = 0;

  esp_thread->m_runningMutex->unlock();
}
void basic_thread::thread_started() {
  m_continuemutex2->unlock();
}


foreign_thread::foreign_thread() 
  : foreign_thread(xTaskGetCurrentTaskHandle()) { 

  m_strName = "current_foreign_thread";
}

foreign_thread::foreign_thread(void* t)
  : basic_thread() { 
  
  handle = t;

  m_strName = "foreign_thread";
  m_iCore = xPortGetCoreID();
  m_iID = uxTaskGetTaskNumber(handle);
  m_usStackDepth = 0;
  m_bMutexInit = false;

  

  if (xPortInIsrContext()) {
    m_uiPriority = uxTaskPriorityGetFromISR(handle);
  } else {
	  m_uiPriority = uxTaskPriorityGet(handle);
  }
  m_pChild = NULL;
  m_pParent = NULL;
}

int foreign_thread::__internal_create_usings_types() {
   if(!m_bMutexInit) {
    m_runningMutex = new LockType_t();
    m_contextMutext = new LockType_t();

    m_continuemutex = new LockType_t();
    m_continuemutex2 = new LockType_t();


    if(m_runningMutex->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;
    if(m_contextMutext->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;

    if(m_continuemutex->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;
    if(m_continuemutex2->create() != ERR_MUTEX_OK)
      return ERR_THREAD_CANTINITMUTEX;

    m_bMutexInit = true;
    
  }
  m_continuemutex->lock();

	if (handle == 0) {
    m_continuemutex->unlock();
		return ERR_THREAD_CANTSTARTTHREAD;
  }
  m_continuemutex->unlock();

	return ERR_THREAD_OK;
}

foreign_thread* foreign_thread::get_idle_task() {
  void* rawHandle = xTaskGetIdleTaskHandle();

  return foreign_thread::create_from(rawHandle);
}
foreign_thread* foreign_thread::get_idle_task(UBaseType_t cpuid) {
  void* rawHandle = xTaskGetIdleTaskHandleForCPU(cpuid);

  foreign_thread* thread = foreign_thread::create_from(rawHandle);

  if(thread) thread->m_iCore = cpuid;
  
  return thread;
}
foreign_thread* foreign_thread::create_from(void* foreign_handle, int* ret_error = NULL) {
    if(foreign_handle == NULL) return NULL;

    int _error_ret = 0;
    foreign_thread* newThread = new foreign_thread(foreign_handle);

    _error_ret = newThread->__internal_create_usings_types();
    if(ret_error) *ret_error = _error_ret;

    return foreign_thread;
  }