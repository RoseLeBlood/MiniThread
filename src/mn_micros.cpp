#include "mn_micros.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "esp_attr.h"
#include "esp_partition.h"
#include <sys/time.h>

portMUX_TYPE microsMux = portMUX_INITIALIZER_UNLOCKED;

unsigned long IRAM_ATTR micros() {
    static unsigned long lccount = 0;
    static unsigned long overflow = 0;
    unsigned long ccount;
    portENTER_CRITICAL_ISR(&microsMux);
    __asm__ __volatile__ ( "rsr %0, ccount" : "=a" (ccount) );
    if(ccount < lccount){
        overflow += UINT32_MAX / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ;
    }
    lccount = ccount;
    portEXIT_CRITICAL_ISR(&microsMux);
    return overflow + (ccount / CONFIG_ESP32_DEFAULT_CPU_FREQ_MHZ);
}
unsigned long millis() {
  if (xPortInIsrContext()) {
    return xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
  } else {
    return xTaskGetTickCount() * portTICK_PERIOD_MS;
  }
}

unsigned int get_ticks() {
  if (xPortInIsrContext()) {
    return xTaskGetTickCountFromISR();
  } else {
    return xTaskGetTickCount();
  }
}

unsigned int ticks_to_ms(unsigned int ticks) {
  return ticks * portTICK_PERIOD_MS;
}
unsigned int ms_to_ticks(unsigned int ms) {
  return ms / portTICK_PERIOD_MS;
}
unsigned int seconds_to_ticks(unsigned int sec) {
  return (sec * 1000) / portTICK_PERIOD_MS;
}
