
# Mini Thread 2.0.1
Mini Thread is a library for creating secure threads for the esp32 microcontroller. 
With variable condition support if required. 
And wrappers for:  
   +  Mutex (recursive and standard)
   +  Semaphore (binary and counting)
   +  Queues (deque, binary-queue and the standard queue)
   +  workqueues (multithreaded and singlethreaded workqueues)
   +  Timers, Protothreads and event groups

The license that applies to the library is the LGPL.The license texts of these
licenses can be found in the files [LICENSE](LICENSE.md) of the
source code archive.

## Folder Strutur
- include and src: 
  - / : all basics: task, semaphores. timer ...
  - memory: mempool handling
  - queue: FreeRTOS queue's and workqueue-engines
  - slock: ystem interrupt, schedular and ...  autolock helper 
- doc: Files to create the docu with doxygen 
  - The online pre builded version: https://roseleblood.github.io/mnthread-docs/
- example; The basic's example, and for more see extra repository: [mnthread-examples](https://github.com/RoseLeBlood/mnthread-examples)


## Simple Example
```cpp
//simple_example.cpp

#include <miniThread.hpp>

#define NUMBER_OF_TEST_THREADS      2

class hello_world_task : public basic_task {
public:
    hello_world_task() : basic_task("HelloWorld", 1) { }

    virtual void*  on_task() override { 
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
        tasks[i].start( i % 2 );
    }
    mn_panic();
}
```

## TODO's for Version 2.0
- [X] Add event group class 
- [X] Add usefull FreeRTOS Wrapper
- [X] Completed the documentation
- [-] Write more examles (after 1.9 ready ) 
- [X] add a Workqueues 
- [X] add Memory Manager class
- [ ] start to add trace support UART and LwIP out (ready to Version 2.5)
- [X] task Local Storage in task_utils
  

## ChangeLog
For more see [ChangeLog](ChangeLog.md)

## Version 2.0.1 Januar 2021
+ rename tasklet function create to schedule - (DE: Mein fehler - misverständnis)
+ add schudaler util class 
+ add random handler, for future use
+ add new memory pool handler simple version and a complexere version for debugging 
+ Version jump to 2.0
+ EXAMPLES in progress 
+ Add namespaces

## Version 1.9.5 Oktober 2020: (unstable 2.0beta) 
+ update error codes
+ merge config_preview with config and error_preview with error
+ add shared, timed and the ringbuffer to main version
+ remove old system locks and add new lock system - old nclude file
+ add new in code example and update the doxygen bootstra output style (not online)
+ Add static support
  
## Version 1.9.1 Oktober 2020: (unstable 2.0beta) 
+ for the future version add prototypes: (Not for produtions use!!)
  + free_list_memory pool = basic_free_list_mempool
  + Shared Object = basic_shared_object
  + Ringbuffer = basic_circular_buffer
  + Add timed lock type = basic_timed_lock
+ Add class to extends the basic_convar_task with a message queue support - basic_message_task
  + Add new config item, to handle the maximal messages in the message queue (MN_THREAD_CONFIG_MSGTASK_MAX_MESSAGES)
  + For message queue support must be conditional variable support marked as enable
+ Add new defines: 
  + in mn_foreign_task.hpp:
    + FT_IDLE_TASK  (foreign_task::get_idle_task()) 
    + FT_IDLE_TASK_ON(CPUID foreign_task::get_idle_task(CPUID) 
  + in mn_config.hpp:
    + MN_THREAD_CONFIG_MINIMAL_STACK_SIZE set the default minimal stack size for a task 
+ add in basic_task get_state() to get the current state and static function get_tasks() to get the number of tasks
+ remove create and destroy from all mutex and semaphore objects and add exceptins handling 
+ rename create and on_create to start and on_start 
+ add to the basic_task: join(), wait() and get_self() functions - basic_task remove abstract 
+ remove the m_contextMutext2 LockObject 



