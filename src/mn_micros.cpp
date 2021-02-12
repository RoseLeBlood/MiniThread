/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
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

namespace mn {
  portMUX_TYPE microsMux = portMUX_INITIALIZER_UNLOCKED;
  
  //-----------------------------------
  //  micros
  //-----------------------------------
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

  //-----------------------------------
  //  millis
  //-----------------------------------
  unsigned long millis() {
    if (xPortInIsrContext()) {
      return xTaskGetTickCountFromISR() * portTICK_PERIOD_MS;
    } else {
      return xTaskGetTickCount() * portTICK_PERIOD_MS;
    }
  }

  //-----------------------------------
  //  get_ticks
  //-----------------------------------
  unsigned int get_ticks() {
    if (xPortInIsrContext()) {
      return xTaskGetTickCountFromISR();
    } else {
      return xTaskGetTickCount();
    }
  }

  //-----------------------------------
  //  ticks_to_ms
  //-----------------------------------
  unsigned int ticks_to_ms(unsigned int ticks) {
    return ticks * portTICK_PERIOD_MS;
  }

  //-----------------------------------
  //  ms_to_ticks
  //-----------------------------------
  unsigned int ms_to_ticks(unsigned int ms) {
    return ms / portTICK_PERIOD_MS;
  }

  //-----------------------------------
  //  seconds_to_ticks
  //-----------------------------------
  unsigned int seconds_to_ticks(unsigned int sec) {
    return (sec * 1000) / portTICK_PERIOD_MS;
  }

  //-----------------------------------
  //  time_to_ms
  //-----------------------------------
  unsigned int time_to_ms(const struct timeval* time) {
    uint32_t msecs;

    msecs  = time->tv_sec * 1000;
    msecs += (time->tv_usec + 999999) / 1000000;

    return msecs;
  }

  //-----------------------------------
  //  time_to_ms
  //-----------------------------------
  unsigned int time_to_ticks(const struct timeval* time) {
    unsigned int msecs = time_to_ms(time);

    return ms_to_ticks(msecs);
  }
}