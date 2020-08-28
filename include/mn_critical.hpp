#ifndef _MINLIB_CRITICAL_H_
#define _MINLIB_CRITICAL_H_

#include "mn_mutex.hpp"

class basic_critical {
public:
    // NOT imp
    static void enter(mutex_t& handle);
    // NOT imp
    static void exit(mutex_t& handle);

    static void disable_interrupts();
    static void enable_interrupts();

    static void stop_scheduler();
    static void resume_scheduler();
};

class basic_critical_lock {
public:
    basic_critical_lock(mutex_t& m) 
        : m_pMutex(m) { }

    virtual void lock() { basic_critical::enter(m_pMutex);  }
    virtual void unlock() { basic_critical::exit(m_pMutex);  }
private:
    mutex_t&    m_pMutex;
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