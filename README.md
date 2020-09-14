# Mini Thread 1.60
Mini Thread is a library for creating secure threads for the esp32 microcontroller. 
With variable condition support if required. 
And wrappers for:  
   +  Mutex (recursive and standard)
   +  Semaphore (binary and counting)
   +  Queues (deque, binary-queue and the standard queue)
   + workqueues

For more detailed information about the library, please refer to the library documentation (https://roseleblood.github.io/mnthread-docs/) and Examples you find in the extra repository: [mnthread-examples](https://github.com/RoseLeBlood/mnthread-examples)

The license that applies to the library is the LGPL.The license texts of these
licenses can be found in the files [LICENSE](LICENSE.md) of the
source code archive.

## Simple Example
```cpp
//simple_example.cpp

#include <mn_base.hpp>

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
extern "C" void app_main() {
    hello_world_task tasks[NUMBER_OF_TEST_THREADS];

    for(int i = 0; i < NUMBER_OF_TEST_THREADS; i++) {
        tasks[i].create( i % 2 );
    }

    mn_panic();
}
```

## TODO's for Version 2.0
- [ ] Add event group class 
- [ ] Add timer and other usefull FreeRTOS Wrapper
- [ ] Completed the documentation
- [ ] Write more examles 
- [ ] Optional add a Thread (i. e. Task) Pool class

## ChangeLog

Version 1.60 September 2020:
  * add Work queue support (basic_work_queue) Create one or more basic_work_queue
    to accept work_queue_item. basic_work_queue pull work_queue_item off of a FIFO queue and 
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

Version 1.54 September 2020: (unstable)
  * Documentation ready
  * add Licence text to source files
  * remove semaphore_t and spinlock_t
  * add counting_semaphore_t (autocsemp_t) and binary_semaphore_t (autobinsemp_t)  
  please update code semaphore_t = counting_semaphore_t, spinlock_t = binary_semaphore_t
                            
Version 1.53 September 2020: 
  * add eaxample
  * add Documentation add (https://roseleblood.github.io/mnthread-docs/)
  * rename Project from mnthread to Mini Thread

Version 1.43 September 2020:  (stable)
  * move convar function from basic_thread to basic_convar_thread 
  * add basic_convar_thread when "MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES"is and update basic_condition_variable
  * remove errors - (sorry, ... )
  * add a deque class that implements a double ended queue. (deque_t = basic_deque)
                    
Version 1.42 August 2020: (non-stable)
  * add queue wrapper (queue/mn_queue.hpp) queue_t
  * add binary queue (queue/mn_binaryqueue.hpp) binaryqueue_t

For more see [ChangeLog](ChangeLog.md)

