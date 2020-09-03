#include <stdio.h>
#include <libmn.h>
#include "sdkconfig.h"
#include "task.hpp"

helloWorld_task g_test[2];

extern "C" void app_main() {
    mnversion_t& version = mnversion_t::instance();

    printf("Thank you for using libmnthread version: %s\n",
            version.to_string().c_str());

    mn_sleep(3);
    
    g_test[0].create(0);
    g_test[1].create(1);

    libmn_panic();
}
