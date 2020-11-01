#include <miniThread.hpp>

#define NUMBER_OF_TEST_THREADS      2

class hello_world_task : public basic_task {
public:
    hello_world_task() : basic_task("HelloWorld", 1) { }

    virtual void*  on_task() override { 
        basic_thread::on_task(); 
        
        int id = get_id();
        int core = get_on_core();

        for(;;) {
            printf("[%d @ %d] Hello World!!\n", id, core );
        }

        return NULL; 
    }
};
extern "C" void app_main() {
    hello_world_task tasks[NUMBER_OF_TEST_THREADS];

    for(int i = 0; i < NUMBER_OF_TEST_THREADS; i++) {
        tasks[i].create( i % 2 );
    }

    mn_panic();
}