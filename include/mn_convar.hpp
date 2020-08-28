#ifndef MINLIB_ESP32_CONVAR_
#define MINLIB_ESP32_CONVAR_

#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES

#include <list>
#include "mn_mutex.hpp"

class basic_thread;

class basic_condition_variable {
    friend class basic_thread;
public:
    basic_condition_variable();

    /**
     *  Signal a thread waiting on this condition_variable.
     */
    void signal(bool with_child_thread = true);

    /**
     *  Signal all threads waiting on this condition_variable.
     */
    void broadcast(bool with_child_thread = true);

private:
    void add_list(basic_thread *thread);
protected:
    mutex_t                     m_mutex;
    std::list<basic_thread*>    m_waitList;
};

using convar_t = basic_condition_variable;

#endif

#endif