#include "mn_config.hpp"

#if (configUSE_RECURSIVE_MUTEXES == 1)

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "esp_attr.h"


#include "mn_recursive_mutex.hpp"


recursive_mutex::recursive_mutex() : basic_mutex() {

}


int recursive_mutex::create()  {
    if (m_pmutex != NULL)
        return ERR_MUTEX_ALREADYINIT;
  
    m_pmutex = xSemaphoreCreateRecursiveMutex();

    if (m_pmutex) {

        unlock();
        return ERR_MUTEX_OK;
    } else {
        return ERR_MUTEX_CANTCREATEMUTEX;
    }
}
int recursive_mutex::lock(unsigned int timeout) {
    BaseType_t success;

    if (m_pmutex == NULL)
        return ERR_MUTEX_NOTINIT;

    success = xSemaphoreTakeRecursive(m_pmutex, timeout);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_LOCK;
}
int recursive_mutex::unlock() {
    BaseType_t success;

    if (m_pmutex == NULL)
        return ERR_MUTEX_NOTINIT;
    success = xSemaphoreGiveRecursive(m_pmutex);

    return success == pdTRUE ? ERR_MUTEX_OK : ERR_MUTEX_UNLOCK;
}


#endif