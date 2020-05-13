#include "mn-mutex.h"

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"

#include "esp_attr.h"

basic_mutex::basic_mutex() : m_bisinitialized(false) { }
basic_mutex::~basic_mutex() { }

int basic_mutex::create() {
  if (m_bisinitialized)
    return ERR_MUTEX_ALREADYINIT;
  m_pmutex = xSemaphoreCreateBinary();


  if (m_pmutex) {
    m_bisinitialized = true;
    unlock();
    return ERR_MUTEX_OK;
  } else {
    return ERR_MUTEX_CANTCREATEMUTEX;
  }
}
int basic_mutex::lock() {
  if (!m_bisinitialized)
    return ERR_MUTEX_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       xSemaphoreTakeFromISR( m_pmutex, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
    xSemaphoreTake(m_pmutex, portMAX_DELAY);
   }

  return ERR_MUTEX_OK;
}
int basic_mutex::unlock() {
  if (!m_bisinitialized)
    return ERR_MUTEX_NOTINIT;

  if (xPortInIsrContext()) {
       BaseType_t xHigherPriorityTaskWoken = pdFALSE;
       xSemaphoreGiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
       if(xHigherPriorityTaskWoken)
         _frxt_setup_switch();
   } else {
			xSemaphoreGive(m_pmutex);
  }
  return ERR_MUTEX_OK;
}
bool basic_mutex::try_lock() {
  if (!m_bisinitialized)
    return false;

  return (xSemaphoreTake( m_pmutex, 0 ) == pdTRUE);
}
