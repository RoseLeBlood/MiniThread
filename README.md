
# Mini Thread 2.10.7 
Mini Thread is a library for creating safe tasks, queues and other useful things for the esp32, in c ++. All in the background of a more secure and consistent workflow. 
So that your application will be smaller and more effective.

Many of the standard STL containers are replaced with their own optimized versions. Recognizable by its own namespace mn.
All containers, such as vector, map, list, array, can be found in the namespace mn::container.

Various types of association classes are provided for simplicity. This is just the exchange of the used allocator from the system to the Spiram allocator, whereby certain regions can be swapped out to a different storage capacity without having to rewrite the entire program. 
Certain allocation in addition to a limit, the bytes used to be limited.
All allocator find you under the namespace mn::memory.

The containers and pointers use as default the mn::memory::default_allocator_t.

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

## Using 
Build from git from
1. ```sh ./configure or ./configure --prefix=<path> # without prefix then install to /opt```
2. ```sh make build ```
3. ```sh sudo or doas make install ```
4. add  "lib_deps = /opt/miniThread/miniThread-2.*.tar.gz"  to your platformio.ini
 
### Example
```ini
[env:esp-wrover-kit]
platform = espressif32
board = esp-wrover-kit
framework = espidf
lib_deps = /opt/miniThread/miniThread-2.*.tar.gz

```
## Using from platformio
```ini
# platformio.ini – project configuration file

[env:my_build_env]
platform = espressif32
framework = espidf
lib_deps =
  # RECOMMENDED
  # Accept new functionality in a backwards compatible manner and patches
  roseleblood/mini Thread @ ^2.10.7

  # Accept only backwards compatible bug fixes
  # (any version with the same major and minor versions, and an equal or greater patch version)
  roseleblood/mini Thread @ ~2.10.7

  # The exact version
  roseleblood/mini Thread @ 2.10.7

```

## ChangeLog
For more see [ChangeLog](ChangeLog.md)
## Version 2.10.7 Februar 2021 (stable)
+ remove build issus

## Version 2.0.4 Januar 2021 (beta 2.0)
+ add staked, buffered allocator
+ add allocator, mempool and stack to mn::memory namespace
+ add allocator with mempool backend
+ start add aSTL template classes to this library, aSTL a other lightwidgt STL 
  In the future are replace all vector, list, map etc. with aSTL version 
  + add auto_ptr, weak_ptr, skoped_ptr, clone_ptr, save_ptr, shared_ptr to library


## Version 2.0.1 Januar 2021 (beta 2.0)
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



