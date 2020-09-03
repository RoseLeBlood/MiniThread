mnthread
=======
##Introduction
------------

The name mnthread  means 'mini Thread'. This library was developed
by  [RoseLeBlood](http://padersophia.wordpress.com),
As the name suggests the goal of the library is to make it easier to create
threads on the esp32. For more detailed information about the library, please
refer to the library documentation.

License
-------

The license that applies to the library is the LGPL.The license texts of these
licenses can be found in the files `LICENSE` of the
source code archive.


Contact
-------

Questions, comments and bug reports can be sent to
[pba3h11aso@t-online.de](mailto:pba3h11aso@t-online.de)

ChangeLog
---------

Version 1.0.5 November 2018:
* Add libmn.h
* Add Config : mn-config.h
* rename header
* Add examples

Version 0.9.3 November 2018:
* add autolock_t to mn-Thread
* add mn-autolock.h
* add spinlock

Version 0.8.31 November 2018:
* add logical mutext handle to basic_thread
* rename function "delay" to "sleep" in basic_thread
* add function "nsleep" and "usleep" to basic_thread
* add "hild_thread" and "parent_thread" to basic_thread
* add function "get_root();" "get_child();" "add_child_thread(basic_thread* thread);"

Version 0.8.3 November 2018:
* add nsleep, sleep, usleep, micros, milis

Version 0.8.2, November 2018:
* Public release
