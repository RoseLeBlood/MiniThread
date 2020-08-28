

#include "mn_critical.hpp"

void basic_critical::enter(portMUX_TYPE h) { 
    if (xPortInIsrContext()) 
        portENTER_CRITICAL_ISR(&h);
    else
        vTaskEnterCritical(&h);
}

void basic_critical::exit(portMUX_TYPE h) { 
    if (xPortInIsrContext()) 
        portEXIT_CRITICAL_ISR(&h);
    else
        vTaskExitCritical(&h);
}

void basic_critical::disable_interrupts() {
    taskDISABLE_INTERRUPTS();
}
void basic_critical::enable_interrupts() {
    taskENABLE_INTERRUPTS();
}

void basic_critical::stop_scheduler() {
    vTaskSuspendAll();
}
void basic_critical::resume_scheduler() {
    xTaskResumeAll();
}


basic_critical_lock::basic_critical_lock() {
    m_pHandle = portMUX_INITIALIZER_UNLOCKED;
}