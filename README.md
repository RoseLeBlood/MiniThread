# Mini Thread 1.91-beta2.0 (2.0.0 preview)
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
```

## TODO's for Version 2.0
- [X] Add event group class 
- [X] Add timer, events and other usefull FreeRTOS Wrapper
  - [X] timer
  - [ ] events
  - [X] ...
- [X] Completed the documentation
- [ ] Write more examles (after 1.9 ready ) 
- [X] add a Workqueues 
- [X] add Memory Manager class
- [ ] add trace support UART and LwIP out
  - [ ] uxTaskGetStackHighWaterMark add to task class
  - [ ] get stackpointer and other things
- [ ] task Local Storage
- [ ] Optional:
  - [ ] In Timers, add OnStop / OnStart / mutex sync with these methods?
  

## ChangeLog

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
  
## Version 1.60 September 2020: (unstable nightly)
* add Work queue support (basic_work_queue) basic_work_queue pull work_queue_item off of a FIFO queue and 
  run them sequentially.  
* add New Config Options to mn_conig.hpp
    * MN_THREAD_CONFIG_WORK_QUEUE_MAX_WORK_ITEMS: How many work items to queue in the work queue engine default is 8
    * MN_THREAD_CONFIG_WORK_QUEUE_STACK_SIZE: Default Stak size for the work queue thread. defaultis (configMINIMAL_STACK_SIZE * 2)
    * MN_THREAD_CONFIG_WORK_QUEUE_PRIORITY Default Priority for the work queue thread. default is (tskIDLE_PRIORITY + 1)
    * MN_THREAD_CONFIG_DEFAULT_CORE Default Core to run a new Task (default MN_THREAD_CONFIG_CORE_NO)
    * MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE On whith core run all Workques an default  cas be override in the create function (default MN_THREAD_CONFIG_CORE2_TWO)
    * MN_THREAD_CONFIG_LOCK_TYPE This Property change now the Autolock type and the basic LockType_t (default MN_THREAD_CONFIG_BINARY_SEMAPHORE)
* remane LIB_CPU_0 -> MN_THREAD_CONFIG_CORE2_ONE and LIBMN_CPU_1 -> MN_THREAD_CONFIG_CORE2_TWO
* rename libmn_panic to mn_panic
* add default informations to the config file 
* remove config option MN_THREAD_CONFIG_MUTEX_CLASS. MN_THREAD_CONFIG_MUTEX_CLASS 
* rename libmn.hpp to mn_base.hpp and add missing headers 
* rename mn_spinlock.hpp => mn_semaphore.hpp and mn_spinlock.cpp => mn_semaphore.cpp
* basic_mutex extends basic_semaphore
* add foreign_thread for mini Thread foregin thread and current Thread handling
                       

For more see [ChangeLog](ChangeLog.md)

