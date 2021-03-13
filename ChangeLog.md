# Changelog
## Versoin 2.21 März 2021 (stable)

## Version 2.20 März 2021
+ add allocator helpor macros: MNALLOC_OBJECT, MNALLOC_OBJECT_D ... @see MNALLOC_OBJECT
+ update mempool - added MNALLOC_OBJECT systems
+ add mn::list -- replace std::list
+ mn_allocator_{multiheap, caps, object, system, buffer, stack} to dirctory allocator
+ Add a any_ptr - basic_any_ptr<void> - can only hold const pointer
+ add uuid_t / guid_t - hash/mn_uuid.hpp
+ remove type usings from mn_container.hpp to the array, list, fixed_array, ... headers. 
  mn_container include all container types and all special container usings switch to mn_container_esp32.hpp 
+ add a red black tree to the cointainer namespace - find under container/mn_rb_tree.hpp
+ add any_ptr - a pointer to hold a const any-variant type
+ add at mn::cointainer a Template queue with iterator-support
+ add new algorythmen and mn::functinal support
+ make atomic ready
  
## Version 2.10 Februar 2021
+ add pointer suppert - weak, shared, clone_ptr, and other
+ add new ringbuffer and updated examples
+ add Makefile support only for use the source from git, not use in platformio library regestrie
  
## Version 2.0.4 Januar 2021
+ add staked, buffered allocator
+ add allocator, mempool and stack to mn::memory namespace
+ add allocator with mempool backend
+ start add aSTL template classes to this library, aSTL a other lightwidgt STL 
  In the future are replace all vector, list, map etc. with aSTL version 
  + add auto_ptr, weak_ptr, skoped_ptr, clone_ptr, save_ptr, shared_ptr to library
+ added namespaces:
  + slock -> mn::system
  + mempool, allocator and deleter -> mn::memory
  + foregin_task, convar system, msg task -> mn::ext
  + trace -> mn::trace
  + all other -> mn
+ rename:
  + func: mn_sleep, mn_usleep and mn_nsleep rename to sleep, nsleep and usleeep
  + func: mn_panic to panic
  + class: libmnversion to version

  
## Version 2.0.1 Januar 2021
+ rename tasklet function create to schedule - (DE: Mein fehler - misverständnis)
+ add schudaler util class 
+ add random handler, for future use
+ add new memory pool handler simple version and a complexere version for debugging 
+ add a stack template class: memory/mn_stack.hpp
+ add a wrapper for handling tickhooks
+ EXAMPLES in progress
+ Add new configs items
  + MN_THREAD_CONFIG_MEMPOOL_USETIMED Use for mempool the timed version and xTicksToWait (see config)
  + MN_THREAD_CONFIG_TICKHOOK_MAXENTRYS The max entrys are hold the tickhook queue
  + MN_THREAD_CONFIG_STACK_TYPE The basic type for stack_t ( basic_stack<MN_THREAD_CONFIG_STACK_TYPE>; )

  
## Version 1.95 Oktober 2020: (stable 2.0beta) 
+ update error codes
+ merge config_preview with config and error_preview with error
+ add shared, timed and the ringbuffer to main version
+ remove old system locks and add new lock system - old include file
+ add new in code example and update the doxygen bootstrap output style
+ add basic_tickhook system
+ add mempool support

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
+ add a eventgroup wrapper
+ rename basic_thread to basic_task and the other threads to task - with error codes and 
  rename on_thread to on_task
+ add in confg:
    + MN_THREAD_CONFIG_CORE_MAX   (portNUM_PROCESSORS - 1)
    + add prority enum to task
+ rename mn_base.hpp to miniThread.hpp
+ remove bugs and errors
+ Strip semaphore classes 
+ add task_utils class for notify
+ add MN_THREAD_CONFIG_PREVIEW_FUTURE config item for next major version future (not for product use), not activate
+ all config items can you now override in the sdkconfig file
+ update config with new items, please cheack 

## Version 1.71 September 2020: (unstable 2.0beta)
+ add single and multi threaded worcking queues (basic_work_queue_single & basic_work_queue_multi) 
    (TODO Optional add a Workqueues ) and add configand error defines, with doku
+ remove bugs and errors
+ This is the preview version of 2.0.0
  
## Version 1.60 September 2020: (unstable nightly)
+ add Work queue support (basic_work_queue) basic_work_queue pull work_queue_item off of a FIFO queue and 
  run them sequentially.  
+ add New Config Options to mn_conig.hpp
    + MN_THREAD_CONFIG_WORK_QUEUE_MAX_WORK_ITEMS: How many work items to queue in the work queue engine default is 8
    + MN_THREAD_CONFIG_WORK_QUEUE_STACK_SIZE: Default Stak size for the work queue thread. defaultis (configMINIMAL_STACK_SIZE + 2)
    + MN_THREAD_CONFIG_WORK_QUEUE_PRIORITY Default Priority for the work queue thread. default is (tskIDLE_PRIORITY + 1)
    + MN_THREAD_CONFIG_DEFAULT_CORE Default Core to run a new Task (default MN_THREAD_CONFIG_CORE_NO)
    + MN_THREAD_CONFIG_DEFAULT_WORKQUEUE_CORE On whith core run all Workques an default  cas be override in the create function (default MN_THREAD_CONFIG_CORE2_TWO)
    + MN_THREAD_CONFIG_LOCK_TYPE This Property change now the Autolock type and the basic LockType_t (default MN_THREAD_CONFIG_BINARY_SEMAPHORE)
+ remane LIB_CPU_0 -> MN_THREAD_CONFIG_CORE2_ONE and LIBMN_CPU_1 -> MN_THREAD_CONFIG_CORE2_TWO
+ rename libmn_panic to mn_panic
+ add default informations to the config file 
+ remove config option MN_THREAD_CONFIG_MUTEX_CLASS. MN_THREAD_CONFIG_MUTEX_CLASS 
+ rename libmn.hpp to mn_base.hpp and add missing headers 
+ rename mn_spinlock.hpp => mn_semaphore.hpp and mn_spinlock.cpp => mn_semaphore.cpp
+ basic_mutex extends basic_semaphore
+ add foreign_thread for mini Thread foregin thread and current Thread handling

## Version 1.54 September 2020: (unstable)
+ Documentation ready
+ add Licence text to source files
+ remove semaphore_t and spinlock_t
+ add counting_semaphore_t (autocsemp_t) and binary_semaphore_t (autobinsemp_t)  
please update code semaphore_t = counting_semaphore_t, spinlock_t = binary_semaphore_t
                          
## Version 1.53 September 2020: 
+ add eaxample
+ add Documentation add (https://roseleblood.github.io/mnthread-docs/)
+ rename Project from mnthread to Mini Thread

## Version 1.43 September 2020:  (stable)
+ move convar function from basic_thread to basic_convar_thread 
+ add basic_convar_thread when "MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT  MN_THREAD_CONFIG_YES"is and update basic_condition_variable
+ remove errors - (sorry, ... )
+ add a deque class that implements a double ended queue. (deque_t = basic_deque)
                  
## Version 1.42 August 2020: (non-stable)
+ add queue wrapper (queue/mn_queue.hpp) queue_t
+ add binary queue (queue/mn_binaryqueue.hpp) binaryqueue_t

## Version 1.4 August 2020: (non-stable)
+ add Conditional Variable an disable in config file (mn_config.hpp)
+ add new Cosfigurations items on mn_config.hpp
+ add recursive matux and autolack tps for interrupt, critical_lock and schedular
+ add Tasklets

## Version 1.2 August 2020: (stable)
+ rename the header files from mn-(xxx).h to mn_(xxx).hpp
+ Platformio regestration

## Version 1.0.62 Mai 2020:  (non-stable)
+ update examples - stripped
+ switch from c3pb.gitlab to my public github account

## Version 1.0.60 November 2018: (non-stable)
+ rename:
    + sleep -> mn_sleep
    + usleep -> mn_usleep
    + nsleep -> mn_nsleep
+ start thread mutext unlock funktion - see examples
+ update examples
+ update version
## Version 1.0.5 November 2018: (stable)
+ Add libmn.h
+ Add Config : mn-config.h
+ rename header
+ Add examples

## Version 0.9.3 November 2018: (non-stable)
+ add autolock_t to mn-Thread
+ add mn-autolock.h
+ add spinlock

## Version 0.8.31 November 2018: (non-stable)
+ add logical mutext handle to basic_thread
+ rename function "delay" to "sleep" in basic_thread
+ add function "nsleep" and "usleep" to basic_thread
+ add "child_thread" and "parent_thread" to basic_thread
+ add function "get_root();" "get_child();" "add_child_thread(basic_thread+ thread);"

## Version 0.8.3 November 2018: (non-stable)
+ add nsleep, sleep, usleep, micros, milis


## Version 0.8.2, November 2018: (non-stable)
+ Public release
