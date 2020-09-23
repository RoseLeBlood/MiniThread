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
#include "mn_task.hpp"
#include <stdio.h>


basic_task::basic_task(char const* strName, basic_task::priority uiPriority,
     unsigned short  usStackDepth) {
  m_strName = strName;
  m_uiPriority = uiPriority;
  m_usStackDepth = usStackDepth;
  m_bMutexInit = false;
  m_bRunning = false;

  m_pChild = NULL;
  m_pParent = NULL;
}
basic_task::~basic_task() {
  vTaskDelete(handle); 

  if(!m_bRunning) { 

    if(m_runningMutex != NULL) m_runningMutex->destroy();
    if(m_runningMutex != NULL) m_contextMutext->destroy();
    if(m_runningMutex != NULL) m_continuemutex->destroy();
    if(m_runningMutex != NULL) m_continuemutex2->destroy();

    m_bMutexInit =false; 
  }
}
basic_task*  basic_task::get_root() {
  autolock_t autolock(*m_contextMutext);

  basic_task* root = 0;

  if(m_pParent != NULL) root = m_pParent->get_root();
  else root = m_pParent;

  return root;
}
basic_task*  basic_task::get_child() {
  autolock_t autolock(*m_runningMutex);

  basic_task* child = 0;
  child = m_pChild;
  return child;
}

bool basic_task::add_child_task(basic_task* task) {
  autolock_t autolock(*m_runningMutex);

  bool ret = true;
   task->m_pParent = this;
   if(m_pChild == NULL) m_pChild = task;
   else ret = m_pChild->add_child_task(task);

  return ret;
}

int basic_task::create(int uiCore) {
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
	m_runningMutex->lock();
	if (m_bRunning)
	{
		m_runningMutex->unlock();
		m_continuemutex->unlock();
		return ERR_TASK_ALREADYRUNNING;
	}
	m_runningMutex->unlock();

  xTaskCreatePinnedToCore(&runtaskstub, m_strName,
              m_usStackDepth,
              this, (int)m_uiPriority, &handle, uiCore);

	if (handle == 0) {
    m_continuemutex->unlock();
		return ERR_TASK_CANTSTARTTHREAD;
  }

  m_runningMutex->lock();
  m_iCore = uiCore;
  m_iID = uxTaskGetTaskNumber(handle); //get_new_id(); uxTaskGetTaskNumber
  on_create();
  m_continuemutex->unlock();
  m_runningMutex->unlock();

	return ERR_TASK_OK;
}
int basic_task::kill() {
  m_continuemutex->lock();
  m_runningMutex->lock();

	if (!m_bRunning) {
    m_runningMutex->unlock();
    m_continuemutex->unlock();

		return ERR_TASK_NOTRUNNING;
	}
  vTaskDelete(handle); handle = 0;
  m_bRunning = false;
  on_kill();

  m_runningMutex->unlock();
  m_continuemutex->unlock();
  
	return ERR_TASK_OK;
}
bool basic_task::is_running() {
  autolock_t autolock(*m_runningMutex);
  return m_bRunning;
}
uint32_t basic_task::get_id() {
  autolock_t autolock(*m_runningMutex);
	return m_iID;
}
uint32_t basic_task::get_on_core() {
  autolock_t autolock(*m_runningMutex);
	return m_iCore;
}
const char* basic_task::get_name() {
  autolock_t autolock(*m_runningMutex);

	return m_strName;
}
basic_task::priority basic_task::get_priority() {
  autolock_t autolock(*m_runningMutex);
  
  if(handle == NULL) return m_uiPriority;

  if (xPortInIsrContext()) {
    return (basic_task::priority)uxTaskPriorityGetFromISR(handle);
  } else {
	  return (basic_task::priority)uxTaskPriorityGet(handle);
  }
}
unsigned short basic_task::get_stackdepth() {
  autolock_t autolock(*m_runningMutex);
	return m_usStackDepth;
}
xTaskHandle basic_task::get_handle() {
  autolock_t autolock(*m_runningMutex);
	return handle;
}
void *basic_task::get_return_value() {
	autolock_t autolock(*m_runningMutex);
	return (m_bRunning) ? NULL : m_retval;
}
uint32_t basic_task::get_time_since_start() {
  autolock_t autolock(*m_runningMutex);
	return (uint32_t)(xTaskGetTickCount()*portTICK_PERIOD_MS);
}
void  basic_task::set_priority(basic_task::priority uiPriority) {
  autolock_t autolock(*m_runningMutex);
  m_uiPriority = uiPriority;
  if(handle != NULL)
    vTaskPrioritySet(handle, uiPriority);
}
void basic_task::suspend() {
  autolock_t autolock(*m_runningMutex);
  vTaskSuspend( handle );
}
void basic_task::resume() {
  autolock_t autolock(*m_runningMutex);
  vTaskResume(handle);
}

void basic_task::runtaskstub(void* parm) {
  basic_task *esp_task;
	void *ret;

	esp_task = (static_cast<basic_task*>(parm));

  esp_task->m_continuemutex2->lock();
  esp_task->m_runningMutex->lock();
	esp_task->m_bRunning = true;
  esp_task->m_runningMutex->unlock();

  esp_task->m_continuemutex->lock();
	esp_task->m_continuemutex->unlock();

  esp_task->task_started();
  ret = esp_task->on_task();
  esp_task->on_cleanup();

  esp_task->m_runningMutex->lock();
	esp_task->m_bRunning = false;
	esp_task->m_retval = ret;
  vTaskDelete(esp_task->handle);
  esp_task->handle = 0;

  esp_task->m_runningMutex->unlock();
}
void basic_task::task_started() {
  m_continuemutex2->unlock();
}

