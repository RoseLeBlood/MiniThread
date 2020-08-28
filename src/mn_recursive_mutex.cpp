#include "mn_config.hpp"

#if (configUSE_RECURSIVE_MUTEXES == MN_THREAD_CONFIG_YES)

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "esp_attr.h"


#include "mn_recursive_mutex.hpp"


recursive_mutex::recursive_mutex() : m_bisinitialized(false) {

}
recursive_mutex::~recursive_mutex() {
    
}

int recursive_mutex::create()  {
    if (m_bisinitialized)
        return ERR_MUTEX_ALREADYINIT;
  
    m_pmutex = xSemaphoreCreateRecursiveMutex();

    if (m_pmutex) {
        m_bisinitialized = true;
        unlock();
        return ERR_MUTEX_OK;
    } else {
        return ERR_MUTEX_CANTCREATEMUTEX;
    }
}
int recursive_mutex::lock() {
    if (!m_bisinitialized)
        return ERR_MUTEX_NOTINIT;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreTakeRecursiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        xSemaphoreTakeRecursive(m_pmutex, portMAX_DELAY);
    }

    return ERR_MUTEX_OK;
}
int recursive_mutex::unlock() {
    if (!m_bisinitialized)
    return ERR_MUTEX_NOTINIT;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveRecursiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
            xSemaphoreGiveRecursive(m_pmutex);
    }
    return ERR_MUTEX_OK;
}
bool recursive_mutex::try_lock() {
    if (!m_bisinitialized)
        return false;

    return (xSemaphoreTakeRecursive( m_pmutex, 0 ) == pdTRUE);
}


#endif