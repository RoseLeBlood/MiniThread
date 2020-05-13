#include <stdio.h>
#include <libmn.h>

#define TASk_ON_CPU LIBMN_CPU_1

class basic_task : public mthread {
public:
  basic_task() : mthread("basic_task", 5, 2048) { }

  virtual void*         on_thread() {
    mthread::on_thread();

    int id = get_id();
    int core = get_on_core();

    for(int i=10;i!=0;i--) {
        printf("[%d:%d] End thread in %d seconds\n", id, core,i);
        sleep(1);
    }
    printf("[%d:%d] End thread\n", id, core);

    return 0;
  }
};

extern "C" void app_main() {
    printf("Thank you for using libmnthread version: %s\n",
            libmn_version.to_string().c_str());

    mn_sleep(3);
    basic_task basic_task;
    basic_task.create(TASk_ON_CPU);

    for(;;) ;
}
