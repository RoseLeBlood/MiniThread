#ifndef __LIBMIN_THREAD_H_
#define __LIBMIN_THREAD_H_

#define LIBMN_CPU_1 0
#define LIBMN_CPU_2 1

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "mn_config.hpp"
#include "mn_version.hpp"
#include "mn_autolock.hpp"
#include "mn_micros.hpp"
#include "mn_thread.hpp"



void libmn_panic();

#endif
