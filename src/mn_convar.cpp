/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
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