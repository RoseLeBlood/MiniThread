#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "queue/mn_deque.hpp"
#include "mn_error.hpp"

int basic_deque::enqueue_front(void* item, unsigned int timeout) {
    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    BaseType_t success;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xQueueSendToFrontFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueSendToFront(m_pHandle, item, timeout);
    }
    return success == pdTRUE ? ERR_QUEUE_OK : ERR_QUEUE_ADD;
}