/** This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
**/
#include "mn_convar_thread.hpp"

basic_convar_thread::basic_convar_thread()
     :  basic_thread() { 
        
}
basic_convar_thread::basic_convar_thread(char const* strName, unsigned int uiPriority, unsigned short  usStackDepth) 
       : basic_thread(strName, uiPriority, usStackDepth) { 
        
}

void basic_convar_thread::on_create() {
    m_waitSem = new basic_binary_semaphore();
    m_waitSem->create();
    m_waitSem->lock();
}
void basic_convar_thread::on_kill() {
    
}

void basic_convar_thread::signal() {
    autolock_t autolock(*m_runningMutex);
    m_waitSem->unlock(); 
    on_signal();
}
void basic_convar_thread::signal_all() {
    autolock_t autolock(*m_runningMutex);

    m_waitSem->unlock(); 
    on_signal();

    basic_convar_thread* __child = (basic_convar_thread*)(m_pChild);

    if(__child) 
        __child->signal_all();
}

int basic_convar_thread::wait(convar_t& cv, mutex_t& cvl, TickType_t timeOut)  {
    autolock_t autolock(*m_runningMutex);
    
    cv.add_list(this);
    
    cvl.unlock();
    int ret = m_waitSem->lock(timeOut);
    cvl.lock();

    return ret; 
}