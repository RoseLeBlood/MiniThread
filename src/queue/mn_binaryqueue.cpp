#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "queue/mn_binaryqueue.hpp"
#include "mn_error.hpp"

int basic_binaryqueue::enqueue(void *item, unsigned int timeout) {
    (void)timeout;

    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        (void)xQueueOverwriteFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        (void)xQueueOverwrite(m_pHandle, item);
    }
    return ERR_QUEUE_OK;
}