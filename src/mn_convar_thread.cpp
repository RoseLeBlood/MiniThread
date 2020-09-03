#include "mn_convar_thread.hpp"

basic_convar_thread::basic_convar_thread()
     :  basic_thread() { 
        
}
basic_convar_thread::basic_convar_thread(char const* strName, unsigned int uiPriority, unsigned short  usStackDepth) 
       : basic_thread(strName, uiPriority, usStackDepth) { 
        
}

int basic_convar_thread::on_create() {
    m_waitSem = new semaphore_t();
      if(m_waitSem->create() != ERR_MUTEX_OK)
        return ERR_THREAD_CANTINITMUTEX;
    
    return ERR_THREAD_OK;
}
int basic_convar_thread::on_kill() {
    return ERR_THREAD_OK;
}

void basic_convar_thread::signal() {
    m_waitSem->unlock(); 
    on_signal();
}
void basic_convar_thread::signal_all() {
    signal();

    basic_convar_thread* __child = (basic_convar_thread*)(m_pChild);

    if(__child) 
        __child->signal_all();
}

int basic_convar_thread::wait(convar_t& cv, mutex_t& cvl, TickType_t timeOut)  {
   cv.add_list(this);
  
    cvl.unlock();

    int timed_out = m_waitSem->lock(timeOut);

    cvl.lock();

    return timed_out; 
}