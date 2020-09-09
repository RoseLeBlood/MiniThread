#include <libmn.hpp>

#define NUMBER_OF_TEST_THREADS      2


class hello_world_task : public basic_thread {
public:
    hello_world_task() : basic_thread("HelloWorld", 1) { }

    virtual void*  on_thread() override { 
        basic_thread::on_thread(); 
        
        int id = get_id();
        int core = get_on_core();

        for(;;) {
            printf("[%d @ %d] Hello World!!\n", id, core );
        }

        return NULL; 
    }
};

hello_world_task tasks[NUMBER_OF_TEST_THREADS];

extern "C" void app_main() {
    for(int i = 0; i < NUMBER_OF_TEST_THREADS; i++) {
        tasks[i].create( i % 2 );
    }

    libmn_panic();
}