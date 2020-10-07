# Mini Thread 1.95-beta.2.0 (2.0.0 preview)
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
- [ ] Write more examles (after 1.9 ready ) 
- [X] add a Workqueues 
- [~] add Memory Manager class
- [ ] start to add trace support UART and LwIP out (ready to Version 2.5)
- [X] task Local Storage in task_utils
  

## ChangeLog

## Version 1.9.5 Oktober 2020: (unstable 2.0beta) 
+ update error codes
+ merge config_preview with config and error_preview with error
+ add shared, timed and the ringbuffer to main version
+ remove old system locks and add new lock system - old nclude file
+ add new in code example and update the doxygen bootstra output style (not online)
  
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
  
## Version 1.9.0 September 2020: (stable 2.0beta)
* add a eventgroup wrapper
* rename basic_thread to basic_task and the other threads to task - with error codes and 
  rename on_thread to on_task
* add in confg:
    * MN_THREAD_CONFIG_CORE_MAX   (portNUM_PROCESSORS - 1)
    * add prority enum to task
* rename mn_base.hpp to miniThread.hpp
* remove bugs and errors
* Strip semaphore classes 
* add task_utils class for notify

## Version 1.71 September 2020: (unstable 2.0beta)
* add single and multi threaded worcking queues (basic_work_queue_single & basic_work_queue_multi) 
    (TODO Optional add a Workqueues ) and add configand error defines, with doku
* remove bugs and errors
* This is the preview version of 2.0.0

For more see [ChangeLog](ChangeLog.md)

