# Changelog

## Version 1.9.1 Oktober 2020: (stable 2.0beta) 
* for the future version add prototypes: (Not for produtions use!!)
  * free_list_memory pool and mempool
  * Shared Object 
  * Ringbuffer
  
  
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
* add MN_THREAD_CONFIG_PREVIEW_FUTURE config item for next major version future (not for product use), not activate
* all config items can you now override in the sdkconfig file
* update config with new items, please cheack 

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

## Version 1.54 September 2020: (unstable)
* Documentation ready
* add Licence text to source files
* remove semaphore_t and spinlock_t
* add counting_semaphore_t (autocsemp_t) and binary_semaphore_t (autobinsemp_t)  
please update code semaphore_t = counting_semaphore_t, spinlock_t = binary_semaphore_t
                          
## Version 1.53 September 2020: 
* add eaxample
* add Documentation add (https://roseleblood.github.io/mnthread-docs/)
* rename Project from mnthread to Mini Thread

## Version 1.43 September 2020:  (stable)
* move convar function from basic_thread to basic_convar_thread 
* add basic_convar_thread when "MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES"is and update basic_condition_variable
* remove errors - (sorry, ... )
* add a deque class that implements a double ended queue. (deque_t = basic_deque)
                  
## Version 1.42 August 2020: (non-stable)
* add queue wrapper (queue/mn_queue.hpp) queue_t
* add binary queue (queue/mn_binaryqueue.hpp) binaryqueue_t

## Version 1.4 August 2020: (non-stable)
* add Conditional Variable an disable in config file (mn_config.hpp)
* add new Cosfigurations items on mn_config.hpp
* add recursive matux and autolack tps for interrupt, critical_lock and schedular
* add Tasklets

## Version 1.2 August 2020: (stable)
* rename the header files from mn-(xxx).h to mn_(xxx).hpp
* Platformio regestration

## Version 1.0.62 Mai 2020:  (non-stable)
* update examples - stripped
* switch from c3pb.gitlab to my public github account

## Version 1.0.60 November 2018: (non-stable)
* rename:
    * sleep -> mn_sleep
    * usleep -> mn_usleep
    * nsleep -> mn_nsleep
* start thread mutext unlock funktion - see examples
* update examples
* update version
## Version 1.0.5 November 2018: (stable)
* Add libmn.h
* Add Config : mn-config.h
* rename header
* Add examples

## Version 0.9.3 November 2018: (non-stable)
* add autolock_t to mn-Thread
* add mn-autolock.h
* add spinlock

## Version 0.8.31 November 2018: (non-stable)
* add logical mutext handle to basic_thread
* rename function "delay" to "sleep" in basic_thread
* add function "nsleep" and "usleep" to basic_thread
* add "child_thread" and "parent_thread" to basic_thread
* add function "get_root();" "get_child();" "add_child_thread(basic_thread* thread);"

## Version 0.8.3 November 2018: (non-stable)
* add nsleep, sleep, usleep, micros, milis


## Version 0.8.2, November 2018: (non-stable)
* Public release
