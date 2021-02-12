/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify
*it under the terms of the GNU Lesser General Public License as published by
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but
*WITHOUT ANY WARRANTY; without even the implied warranty of
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.
*/
#include "mn_task.hpp"
#include "mn_task_list.hpp"

namespace mn {
    basic_task_list* basic_task_list::m_pInstance = NULL;
    mutex_t  basic_task_list::m_staticInstanceMux = mutex_t();

    //-----------------------------------
    //  construtor
    //-----------------------------------
    basic_task_list::basic_task_list() 
        : m_pLock(), m_mapTaskOnCore() {  }

    //-----------------------------------
    //  add_task
    //-----------------------------------
    void basic_task_list::add_task(basic_task* task) {
        autolock_t lock(m_pLock);

        if(task == NULL) return;

        int core = task->get_on_core();
        m_mapTaskOnCore[core].push_front(task);
    }

    //-----------------------------------
    //  remove_task
    //-----------------------------------
    void basic_task_list::remove_task(basic_task* task) {
        autolock_t lock(m_pLock);

        if(task == NULL) return;

        int core = task->get_on_core();
        m_mapTaskOnCore[core].remove(task);
    } 

    //-----------------------------------
    //  get_task
    //-----------------------------------
    basic_task* basic_task_list::get_task(int id) {
        basic_task* _ret = get_task(0);
        if(_ret == NULL) _ret = get_task(1);

        return _ret;
    }

    //-----------------------------------
    //  get_task
    //-----------------------------------
    basic_task* basic_task_list::get_task(std::string name) {
        basic_task* _ret = get_task(name);
        if(_ret == NULL) _ret = get_task(name);

        return _ret;
    }

    //-----------------------------------
    //  get_task
    //-----------------------------------
    basic_task* basic_task_list::get_task(int core, int id) {
        autolock_t lock(m_pLock);

        basic_task* _retTask = NULL;

        for(std::list<basic_task*>::iterator i = m_mapTaskOnCore[core].begin();
            i != m_mapTaskOnCore[core].end(); i++) {

            if( (*i)->get_id() == id ) {
                _retTask = (*i);
                break;
            }
        }
        return _retTask;
    }

    //-----------------------------------
    //  get_task
    //-----------------------------------
    basic_task* basic_task_list::get_task(int core, std::string name) {
        autolock_t lock(m_pLock);
        
        basic_task* _retTask = NULL;

        for(std::list<basic_task*>::iterator i = m_mapTaskOnCore[core].begin();
            i != m_mapTaskOnCore[core].end(); i++) {

            if( (*i)->get_name() == name ) {
                _retTask = (*i);
                break;
            }
        }
        return _retTask;
    }
}