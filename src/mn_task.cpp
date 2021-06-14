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
#include "mn_config.hpp"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#include <stdio.h>
#include <esp_log.h>

#include "mn_task_utils.hpp"
#include "mn_task.hpp"
#include "mn_task_list.hpp"
#include "mn_atomic_counter.hpp"

#define EVENTGROUP_BIT_JOINABLE (1 << 0)
#define EVENTGROUP_BIT_STARTED	(1 << 2)

namespace mn {
  namespace internal {

    inline int32_t get_new_uniqid() {
      static mn::atomic_counter _counter;
      return _counter++;
    }
  }
  //-----------------------------------
  //  construtor
  //-----------------------------------
  basic_task::basic_task(std::string strName, basic_task::priority uiPriority,
      unsigned short  usStackDepth)
        : m_runningMutex(),
          m_contextMutext(),
          m_continuemutex(),
          m_strName(strName),
          m_uiPriority(uiPriority),
          m_usStackDepth(usStackDepth),
          m_retval(ERR_TASK_OK),
          m_bRunning(false),
          m_iID(0),
          m_iCore(-1),
          m_pHandle(NULL),
          m_eventGroup(strName.c_str())
          { }
  //-----------------------------------
  //  deconstrutor
  //-----------------------------------
  basic_task::~basic_task() {
    if(m_pHandle != NULL)
      vTaskDelete(m_pHandle);

  #if MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST == MN_THREAD_CONFIG_YES
    basic_task_list::instance().remove_task(this);
  #endif
  }


  //-----------------------------------
  //  start
  //-----------------------------------
  int basic_task::start(int iCore) {

    m_iCore = iCore;

    m_continuemutex.lock();
    m_runningMutex.lock();
    if (m_bRunning)
    {
      m_runningMutex.unlock();
      m_continuemutex.unlock();

      ESP_LOGI(m_strName.c_str(), "task allready running");
      return ERR_TASK_ALREADYRUNNING;
    }
    m_runningMutex.unlock();

    if(m_eventGroup.create() != 0) {
		m_continuemutex.unlock();
		ESP_LOGE(m_strName.c_str(), "can create the event group for this task");
		return ERR_TASK_CANTCREATEEVENTGROUP;
    }

    #if( configSUPPORT_STATIC_ALLOCATION == 1 )
      xTaskCreateStaticPinnedToCore(&runtaskstub, m_strName.c_str(),
                  m_usStackDepth,
                  this, (UBaseType_t)m_uiPriority, &m_pHandle, m_stackBuffer, m_TaskBuffer m_iCore);
    #else
      xTaskCreatePinnedToCore(&runtaskstub, m_strName.c_str(),
                  m_usStackDepth,
                  this, (UBaseType_t)m_uiPriority, &m_pHandle, m_iCore);
    #endif

    if (m_pHandle == 0) {
      m_continuemutex.unlock();
      ESP_LOGE(m_strName.c_str(), "the freertos task can not created");
      return ERR_TASK_CANTSTARTTHREAD;
    }

    m_runningMutex.lock();

    m_iID = internal::get_new_uniqid();
    on_start();
    m_continuemutex.unlock();
    m_runningMutex.unlock();
    m_iCore = xTaskGetAffinity(m_pHandle) ;

    //xTaskNotify(m_pHandle, 0, eNoAction);
    task_utils::notify(this, 0, task_utils::action::no_action);

  #if MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST == MN_THREAD_CONFIG_YES
    basic_task_list::instance().add_task(this);
  #endif


    return ERR_TASK_OK;
  }

  //-----------------------------------
  //  join
  //-----------------------------------
  int basic_task::join(timespan_t time) {
	timespan_t _time = time - timespan_t::now();
	return join(_time.to_ticks());

  }

  //-----------------------------------
  //  wait
  //-----------------------------------
  int basic_task::wait(timespan_t time) {
	timespan_t _time = time - timespan_t::now();
	return wait(_time.to_ticks());
  }

  //-----------------------------------
  //  join
  //-----------------------------------
  int basic_task::join(unsigned int xTimeOut) {
  	if(!m_bRunning) return ERR_TASK_NOTRUNNING;
  	if(m_pHandle == xTaskGetCurrentTaskHandle())  {
  		ESP_LOGW("WARNING BOT", "Don't do this!! Don't do this.... only you are a cake! ... Bob?");
		return ERR_TASK_CALLFROMSELFTASK;
  	}

  	while ( !m_eventGroup.is_bit(EVENTGROUP_BIT_JOINABLE, xTimeOut) ) { }

  	return NO_ERROR;
  }

  //-----------------------------------
  //  wait
  //-----------------------------------
  int basic_task::wait(unsigned int xTimeOut) {
  	if(!m_bRunning) return ERR_TASK_NOTRUNNING;
  	if(m_pHandle == xTaskGetCurrentTaskHandle())  {
  		ESP_LOGW("WARNING BOT", "Don't do this!! Don't do this.... only you are a cake! ... Bob?");
		return ERR_TASK_CALLFROMSELFTASK;
  	}

  	while ( !m_eventGroup.is_bit(EVENTGROUP_BIT_STARTED, xTimeOut) ) { }

  	return NO_ERROR;
  }

  //-----------------------------------
  //  kill
  //-----------------------------------
  int basic_task::kill() {
    m_continuemutex.lock();
    m_runningMutex.lock();

    if (!m_bRunning) {
      m_runningMutex.unlock();
      m_continuemutex.unlock();

      return ERR_TASK_NOTRUNNING;
    }
    vTaskDelete(m_pHandle); m_pHandle = 0;
    m_bRunning = false;
    on_kill();

    m_runningMutex.unlock();
    m_continuemutex.unlock();

    return ERR_TASK_OK;
  }

  //-----------------------------------
  //  is_running
  //-----------------------------------
  bool basic_task::is_running() {
    autolock_t autolock(m_runningMutex);
    return m_bRunning;
  }

  //-----------------------------------
  //  get_id
  //-----------------------------------
  int32_t basic_task::get_id() {
    autolock_t autolock(m_runningMutex);
    return m_iID;
  }

  //-----------------------------------
  //  get_on_core
  //-----------------------------------
  int32_t basic_task::get_on_core() {
    autolock_t autolock(m_runningMutex);
    return m_iCore;
  }

  //-----------------------------------
  //  get_name
  //-----------------------------------
  std::string basic_task::get_name() {
    autolock_t autolock(m_runningMutex);

    return m_strName;
  }

  //-----------------------------------
  //  get_priority
  //-----------------------------------
  basic_task::priority basic_task::get_priority() {
    autolock_t autolock(m_runningMutex);

    if(m_pHandle == NULL) return m_uiPriority;

    if (xPortInIsrContext()) {
      return (basic_task::priority)uxTaskPriorityGetFromISR(m_pHandle);
    } else {
      return (basic_task::priority)uxTaskPriorityGet(m_pHandle);
    }
  }

  //-----------------------------------
  //  get_stackdepth
  //-----------------------------------
  unsigned short basic_task::get_stackdepth() {
    autolock_t autolock(m_runningMutex);
    return m_usStackDepth;
  }

  //-----------------------------------
  //  get_handle
  //-----------------------------------
  xTaskHandle basic_task::get_handle() {
    autolock_t autolock(m_runningMutex);
    return m_pHandle;
  }

  //-----------------------------------
  //  get_tasks
  //-----------------------------------
  uint32_t basic_task::get_tasks() {
    return uxTaskGetNumberOfTasks();
  }

  //-----------------------------------
  //  get_state
  //-----------------------------------
  basic_task::state basic_task::get_state() {
    autolock_t autolock(m_runningMutex);
    return (basic_task::state) eTaskGetState(m_pHandle);
  }

  //-----------------------------------
  //  get_return_value
  //-----------------------------------
  int basic_task::get_return_value() {
    autolock_t autolock(m_runningMutex);
    return (m_bRunning) ? -999 : m_retval;
  }

  //-----------------------------------
  //  get_time_since_start
  //-----------------------------------
  timespan_t basic_task::get_time_since_start() const {
    autolock_t autolock(m_runningMutex);
    auto ms = (xTaskGetTickCount()*portTICK_PERIOD_MS);

    return timespan_t(ms);
  }

  //-----------------------------------
  //  get_self
  //-----------------------------------
  basic_task* basic_task::get_self() {
    auto _pHandle = xTaskGetCurrentTaskHandle();

    if (_pHandle == 0) return NULL;

    basic_task* _task = new basic_task();

    _task->m_runningMutex.lock();
    _task->m_iID = -1;
    _task->m_pHandle = _pHandle;
    _task->m_eventGroup.create();
    _task->m_runningMutex.unlock();

    return _task;
  }

  //-----------------------------------
  //  set_priority
  //-----------------------------------
  void  basic_task::set_priority(basic_task::priority uiPriority) {
    autolock_t autolock(m_runningMutex);
    m_uiPriority = uiPriority;
    if(m_pHandle != NULL)
      vTaskPrioritySet(m_pHandle, (UBaseType_t)uiPriority);
  }

  //-----------------------------------
  //  suspend
  //-----------------------------------
  void basic_task::suspend() {
    autolock_t autolock(m_runningMutex);
    vTaskSuspend( m_pHandle );
  }

  //-----------------------------------
  //  resume
  //-----------------------------------
  void basic_task::resume() {
    autolock_t autolock(m_runningMutex);
    vTaskResume(m_pHandle);
  }
  //-----------------------------------
  //  runtaskstub
  //-----------------------------------
  void basic_task::runtaskstub(void* parm) {
    basic_task *esp_task;
    int ret; // the return value of the user task functions

    // cast the user data to this object
    esp_task = (static_cast<basic_task*>(parm));

    // error?
    if(esp_task == nullptr) {
		// Log the error and delete the task
		ESP_LOGE("basic_task", "unknown error on minilib task stub, task will delete");
		vTaskDelete(xTaskGetCurrentTaskHandle());
    } else { // on no error run normal minilib task system
    	// set the started bit
		esp_task->m_eventGroup.set(EVENTGROUP_BIT_STARTED);

		// set running
		esp_task->m_runningMutex.lock();
		esp_task->m_continuemutex.lock();
		esp_task->m_bRunning = true;
		esp_task->m_runningMutex.unlock();

		// call the user task functions
		ret = esp_task->on_task();

		// clean up
		esp_task->on_cleanup();

		// set the return value and delete the task
		esp_task->m_runningMutex.lock();
		esp_task->m_bRunning = false;
		esp_task->m_retval = ret;
		vTaskDelete(esp_task->m_pHandle);
		esp_task->m_pHandle = 0;
		esp_task->m_runningMutex.unlock();
		esp_task->m_continuemutex.unlock();

		// set the join bit
		esp_task->m_eventGroup.set(EVENTGROUP_BIT_JOINABLE);
    }
  }
}
