#ifndef MINLIB_ESP32_CON_THREAD_
#define MINLIB_ESP32_CON_THREAD_


#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES

#include "mn_thread.hpp"
#include "mn_convar.hpp"


class basic_convar_thread : public basic_thread {
    friend class basic_condition_variable;
public:
    basic_convar_thread();
    basic_convar_thread(char const* strName, unsigned int uiPriority,
       unsigned short  usStackDepth = configMINIMAL_STACK_SIZE);

    virtual int           on_create();
    virtual int           on_kill();

	virtual void          signal();
    virtual void          signal_all();

    virtual int           wait(convar_t& cv, mutex_t& cvl, TickType_t timeOut = portMAX_DELAY);
protected:
    virtual void          on_signal() { }
private:
    semaphore_t* m_waitSem;
};

using convar_thread_t = basic_convar_thread;
#endif


#endif