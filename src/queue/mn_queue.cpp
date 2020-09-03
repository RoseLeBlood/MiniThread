#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "queue/mn_queue.hpp"
#include "mn_error.hpp"

basic_queue::basic_queue(unsigned int maxItems, unsigned int itemSize) 
    : m_pHandle(NULL), m_imaxItems(maxItems), m_iitemSize(itemSize)  { 

}
basic_queue::~basic_queue() { 

}

int basic_queue::create() {
    if(m_pHandle != NULL) return ERR_QUEUE_ALREADYINIT;

    m_pHandle = xQueueCreate(m_imaxItems, m_iitemSize);

    return (m_pHandle != NULL) ? ERR_QUEUE_OK : ERR_QUEUE_CANTCREATE;
}
int basic_queue::destroy() {
    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    vQueueDelete(m_pHandle);
    m_pHandle = NULL;

    return ERR_QUEUE_OK;
}

int basic_queue::enqueue(void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xQueueSendToBackFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueSendToBack(m_pHandle, item, timeout);
    }

    return success == pdTRUE ? ERR_QUEUE_OK : ERR_QUEUE_ADD;
}
int basic_queue::dequeue(void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xQueueReceiveFromISR(m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueReceive(m_pHandle, item, timeout);
    }

    return success == pdTRUE ? ERR_QUEUE_OK : ERR_QUEUE_REMOVE;
}
int basic_queue::peek(void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return ERR_QUEUE_NOTCREATED;

    if (xPortInIsrContext()) {
        success = xQueuePeekFromISR(m_pHandle, item);
    } else {
        success = xQueuePeek(m_pHandle, item, timeout);
    }

    return success == pdTRUE ? ERR_QUEUE_OK : ERR_QUEUE_PEEK;
}

bool basic_queue::is_empty() {
    unsigned int cnt = get_num_items();

    return cnt == 0 ? true : false;
}
bool basic_queue::is_full() {
    unsigned int cnt = get_left();

    return cnt == 0 ? true : false;
}

int basic_queue::clear() {
    xQueueReset(m_pHandle);
    return ERR_QUEUE_OK;
}


unsigned int basic_queue::get_num_items() {
    return uxQueueMessagesWaiting(m_pHandle);
}


unsigned int basic_queue::get_left() {
    return uxQueueSpacesAvailable(m_pHandle);
}