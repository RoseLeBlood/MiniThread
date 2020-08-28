#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "mn_critical.hpp"

void basic_critical::enter(mutex_t& h) { 
    vTaskEnterCritical( (portMUX_TYPE*)(h.get_handle()) );
}

void basic_critical::exit(mutex_t& h) { 
    vTaskExitCritical( (portMUX_TYPE*)(h.get_handle()) );
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
