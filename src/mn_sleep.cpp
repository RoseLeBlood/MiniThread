#include "mn_sleep.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "sdkconfig.h"

#include "esp_attr.h"
#include "esp_partition.h"
#include <sys/time.h>



unsigned mn_sleep(unsigned int secs) {
	vTaskDelay( (secs * 1000) / ((TickType_t) 1000 / configTICK_RATE_HZ));
	return 0;
}
int mn_usleep(useconds_t usec) {
	vTaskDelay(usec / ((TickType_t) 1000000 / configTICK_RATE_HZ));
	return 0; 
}

int IRAM_ATTR mn_nsleep(const struct timespec *req, struct timespec *rem) {
	struct timeval start, end;

	if ((req->tv_nsec < 0) || (req->tv_nsec > 999999999)) {
		errno = EINVAL;

		return -1;
	}

	// Get time in msecs
	uint32_t msecs;

	msecs  = req->tv_sec * 1000;
	msecs += (req->tv_nsec + 999999) / 1000000;

	if (rem != NULL) {
		gettimeofday(&start, NULL);
	}

	vTaskDelay(msecs / portTICK_PERIOD_MS);
	if (rem != NULL) {
		rem->tv_sec = 0;
		rem->tv_nsec = 0;

		gettimeofday(&end, NULL);

		uint32_t elapsed = (end.tv_sec - start.tv_sec) * 1000
						 + ((end.tv_usec - start.tv_usec) / 1000000);

		if (elapsed < msecs) {
			if (elapsed > 1000) {
				rem->tv_sec = elapsed / 1000;
				elapsed -= rem->tv_sec * 1000;
				rem->tv_nsec = elapsed * 1000000;
			}
		}
	}
	errno = EINTR;


	return 0;
}
