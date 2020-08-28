#ifndef _MINLIB_CRITICAL_H_
#define _MINLIB_CRITICAL_H_

#include "mn_mutex.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class basic_critical {
public:
    static void enter(portMUX_TYPE handle);
    static void exit(portMUX_TYPE handle);

    static void disable_interrupts();
    static void enable_interrupts();

    static void stop_scheduler();
    static void resume_scheduler();
};

class basic_critical_lock {
public:
    basic_critical_lock();

    virtual void lock() { basic_critical::enter(m_pHandle);  }
    virtual void unlock() { basic_critical::exit(m_pHandle);  }
private:
    portMUX_TYPE m_pHandle;
};

class basic_interrupts_lock { 
public:
    virtual void lock() { basic_critical::disable_interrupts();  }
    virtual void unlock() { basic_critical::enable_interrupts();  }
};

class basic_scheduler_lock { 
public:
    virtual void lock() { basic_critical::stop_scheduler();  }
    virtual void unlock() { basic_critical::resume_scheduler();  }
};

using critical_t = basic_critical;

using critical_lock_t = basic_critical_lock;
using interrupts_lock_t = basic_interrupts_lock;
using sheduler_lock_t = basic_scheduler_lock;


#endif