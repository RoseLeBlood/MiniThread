#include "mn-spinlock.h"
#include "mn-error.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

#include "esp_attr.h"

basic_spinlock::basic_spinlock(int count) : m_uiCount(count), m_bisinitialized(false) {
    m_pSpinlock = xSemaphoreCreateCounting(0x7fffffff, count);
}
basic_spinlock::~basic_spinlock() {
}

int basic_spinlock::create() {
  if (m_bisinitialized)
    return ERR_SPINLOCK_ALREADYINIT;
  m_pSpinlock = xSemaphoreCreateCounting(0x7fffffff, m_uiCount);


  if (m_pSpinlock) {
    printf("[MUTEX] xSemaphoreCreateCounting OK\n");
    m_bisinitialized = true;

    unlock();

    return ERR_SPINLOCK_OK;
  }
  return ERR_SPINLOCK_CANTCREATEMUTEX;
}

int basic_spinlock::lock() {
  if (!m_bisinitialized)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       xSemaphoreTakeFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
    xSemaphoreTake(m_pSpinlock, portMAX_DELAY);
   }
  return ERR_SPINLOCK_OK;
}
int basic_spinlock::unlock() {
  if (!m_bisinitialized)
    return ERR_SPINLOCK_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       xSemaphoreGiveFromISR( m_pSpinlock, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
			xSemaphoreGive(m_pSpinlock);
  }
  return ERR_SPINLOCK_OK;
}
bool basic_spinlock::try_lock() {
  if (!m_bisinitialized)
    return false;

  return (xSemaphoreTake( m_pSpinlock, 0 ) == pdTRUE);
}
int basic_spinlock::get_count() const {
  //return static_cast<int>(uxQueueMessagesWaiting(m_pSpinlock));
  return 0;
}
