#include "mn_thread.hpp"
#include <stdio.h>


basic_thread::basic_thread(char const* strName, unsigned int uiPriority,
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

    m_runningMutex->destroy();
    m_contextMutext->destroy();
    m_continuemutex->destroy();
    m_continuemutex2->destroy();

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
  autolock_t autolock(*m_contextMutext);

  basic_thread* child = 0;
  child = m_pChild;
  return child;
}

bool basic_thread::add_child_thread(basic_thread* thread) {
  autolock_t autolock(*m_contextMutext);

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
                this, m_uiPriority, &handle);
  } else {
    xTaskCreatePinnedToCore(&runtaskstub, m_strName,
                m_usStackDepth,
                this, m_uiPriority, &handle, uiCore);

  }
  m_iCore = uiCore;
  m_iID = get_new_id();

	if (handle == 0) {
    m_continuemutex->unlock();
		return ERR_THREAD_CANTSTARTTHREAD;
  }

	m_continuemutex->unlock();

	return on_create();
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

  m_runningMutex->unlock();
  m_continuemutex->unlock();

	return 0;
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
unsigned int basic_thread::get_priority() {
  autolock_t autolock(*m_runningMutex);
	return uxTaskPriorityGet(handle);
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
void  basic_thread::setPriority(unsigned int uiPriority) {
  autolock_t autolock(*m_runningMutex);
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
uint32_t __internal_id_base__ = 0;

uint32_t basic_thread::get_new_id() {
  return __internal_id_base__++;
}