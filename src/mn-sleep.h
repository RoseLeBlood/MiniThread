#ifndef _MINILIB_NANOSLEEP_H_
#define _MINILIB_NANOSLEEP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/signal.h>


unsigned mn_sleep(unsigned int secs);
int mn_usleep(useconds_t usec);

int mn_nsleep(const struct timespec *req, struct timespec *rem);

#endif
