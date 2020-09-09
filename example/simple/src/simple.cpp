#include <stdio.h>
#include <libmn.h>
#include "sdkconfig.h"
#include "task.hpp"

class helloWorld_task : public basic_thread {
public:
  helloWorld_task()
    : basic_thread("hello_task", 5, 2048) { k = 0;} 

  virtual void* on_thread() {
    basic_thread::on_thread();

    int id = get_id();
    int core = get_on_core();

    while(true) {
        printf("[%d:%d] Hello World!\n", id, core);
        sleep(1);
    }
    return &k;
  }
private:
  int k;
};

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
