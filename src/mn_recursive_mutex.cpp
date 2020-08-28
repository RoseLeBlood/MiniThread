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
    vSemaphoreDelete(m_pmutex);
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
int recursive_mutex::lock(unsigned int timeout) {
    BaseType_t success;

    if (!m_bisinitialized)
        return ERR_MUTEX_NOTINIT;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        success = xSemaphoreTakeRecursiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xSemaphoreTakeRecursive(m_pmutex, timeout);
    }

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_LOCK;
}
int recursive_mutex::unlock() {
    BaseType_t success;

    if (!m_bisinitialized)
    return ERR_MUTEX_NOTINIT;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        success = xSemaphoreGiveRecursiveFromISR( m_pmutex, &xHigherPriorityTaskWoken );
        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xSemaphoreGiveRecursive(m_pmutex);
    }
    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_UNLOCK;
}
bool recursive_mutex::try_lock() {
    if (!m_bisinitialized)
        return false;

    return (lock( 0 ) == ERR_MUTEX_OK);
}


#endif