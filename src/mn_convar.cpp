#include "mn_config.hpp"

#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES
#include "mn_convar.hpp"
#include "mn_convar_thread.hpp"

basic_condition_variable::basic_condition_variable() 
    : m_waitList() {
    m_mutex.create();
}

void basic_condition_variable::add_list(basic_convar_thread *thread) {
    automutx_t autolock(m_mutex);

    m_waitList.push_back(thread);
}
void basic_condition_variable::signal(bool with_child_thread) {
    automutx_t autolock(m_mutex);

    if ( !m_waitList.empty() ) {
        basic_convar_thread *thr = m_waitList.front();
        m_waitList.pop_front();

        if(with_child_thread)
            thr->signal_all();
        else
            thr->signal();
    }
}
void basic_condition_variable::broadcast(bool with_child_thread) {
    automutx_t autolock(m_mutex);

    while ( !m_waitList.empty() ) {
        basic_convar_thread *thr = m_waitList.front();
        m_waitList.pop_front();
        
        if(with_child_thread)
            thr->signal_all();
        else
            thr->signal();
    }
}



#endif