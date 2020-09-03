#ifndef _MINILIB_NANOSLEEP_H_
#define _MINILIB_NANOSLEEP_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <errno.h>
#include <time.h>

#include <sys/time.h>
#include <sys/signal.h>

/**
 * Delay a given task for a given number of seconds.  
 */ 
unsigned mn_sleep(unsigned int secs);
/**
 * Delay a given task for a given number of micro seconds.
 */ 
int mn_usleep(useconds_t usec);
/**
 * Delay a given task for a given timespec 
 */ 
int mn_nsleep(const struct timespec *req, struct timespec *rem);

#endif
