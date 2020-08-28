#include "mn_spinlock.hpp"
#include "mn_error.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

#include "esp_attr.h"

basic_spinlock::basic_spinlock(int count, int maxcount) 
  : m_uiCount(count), m_uiMaxCount(maxcount), m_bisinitialized(false) {
    
}
basic_spinlock::~basic_spinlock() {
    vSemaphoreDelete(m_pSpinlock);
}

int basic_spinlock::destroy() {
  vSemaphoreDelete(m_pSpinlock);
  return ERR_SPINLOCK_OK;
}

int basic_spinlock::create() {
  if (m_bisinitialized)
    return ERR_SPINLOCK_ALREADYINIT;

  if (m_uiMaxCount < m_uiCount)
    return ERR_SPINLOCK_BAD_INITIALCOUNT;

  if (m_uiMaxCount == 0) {
    m_pSpinlock = xSemaphoreCreateCounting(m_uiMaxCount, m_uiCount);



    if (m_pSpinlock) {
      printf("[MUTEX] xSemaphoreCreateCounting OK\n");
      m_bisinitialized = true;

      unlock();

      return ERR_SPINLOCK_OK;
    }
  }
  return ERR_SPINLOCK_CANTCREATESPINLOCK;
}

int basic_spinlock::lock(unsigned int timeout) {
  BaseType_t success;

  if (!m_bisinitialized)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreTakeFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
    success = xSemaphoreTake(m_pSpinlock, timeout);
   }
   return success == pdTRUE ? ERR_SPINLOCK_OK : ERR_SPINLOCK_LOCK;
}
int basic_spinlock::unlock() {
  BaseType_t success;

  if (!m_bisinitialized)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       success = xSemaphoreGiveFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
			success = xSemaphoreGive(m_pSpinlock);
  }
  return success == pdTRUE ? ERR_SPINLOCK_OK : ERR_SPINLOCK_UNLOCK;
}
bool basic_spinlock::try_lock() {
  if (!m_bisinitialized)
    return false;

  return (xSemaphoreTake( m_pSpinlock, 0 ) == pdTRUE);
}
int basic_spinlock::get_count() const {
   return uxQueueMessagesWaiting(m_pSpinlock);
}

binary_semaphore::binary_semaphore() : basic_spinlock() { }


int binary_semaphore::create() {
  if (m_bisinitialized)
    return ERR_SPINLOCK_ALREADYINIT;

  m_pSpinlock = xSemaphoreCreateBinary();


  if (m_pSpinlock) {
    printf("[MUTEX] xSemaphoreCreateBinary OK\n");
    m_bisinitialized = true;

    unlock();

    return ERR_SPINLOCK_OK;
  }
  return ERR_SPINLOCK_CANTCREATESPINLOCK;
}
