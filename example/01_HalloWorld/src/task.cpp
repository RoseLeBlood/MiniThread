#include "task.hpp"

helloWorld_task::helloWorld_task()
    : basic_thread("hello_task", 5, 2048) { k = 0;} 

void* helloWorld_task::on_thread() {
    basic_thread::on_thread();

    int id = get_id();
    int core = get_on_core();

    while(true) {
        printf("[%d:%d] Hello World!\n", id, core);
        sleep(1);
    }
    return &k;
}
