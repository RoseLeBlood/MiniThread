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
#ifndef MINLIB_ESP32_THREAD_LIST_
#define MINLIB_ESP32_THREAD_LIST_

class basic_task;

#include <list>
#include <map>

namespace mn {
    
    /**
     * A simple task list 
     * 
     * @note If MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST activated then automatic added new basic_tasks 
     * to this list. On default is MN_THREAD_CONFIG_ADD_TASK_TO_TASK_LIST deactivated  
     */ 
    class basic_task_list  {
        /**
         * Construtor
         * @note This is a signleton class, only one object 
         * plaese use basic_task_list::instance() 
         */ 
        basic_task_list();

        /**
         * The static object of this class
         */ 
        static basic_task_list* m_pInstance;
        /**
         * The static instance mutex
         */ 
        static mutex_t  m_staticInstanceMux;
    public:
        /**
         * Add a task to list
         * 
         * @param task The task to add
         */ 
        void add_task(basic_task* task);
        /**
         * Remove a task from the list
         * 
         * @param task The task to remove
         */ 
        void remove_task(basic_task* task);

        /**
         * Get a task from the list by id
         * 
         * @param id The id to search 
         * @return The finded task, by id. NULL when not finded a task
         */ 
        basic_task* get_task(int id);
        /**
         * Get a task from the list by name
         * 
         * @param name The name to search 
         * @return The finded task, by name. NULL when not finded a task
         */  
        basic_task* get_task(std::string name);
    private:
        /**
         * Get a task from the list by id on a specific core
         * 
         * @param id The id to search 
         * @param core The core
         * @return The finded task, by id. NULL when not finded a task
         */ 
        basic_task* get_task(int core, int id);
        /**
         * Get a task from the list by name on a specific core
         * 
         * @param name The name to search 
         * @param core The core
         * @return The finded task, by name. NULL when not finded a task
         */ 
        basic_task* get_task(int core, std::string name);
    public:
        /**
         * Get the singleton instance 
         * @return The singleton instance
         */ 
        static basic_task_list& instance() {
            automutx_t lock(m_staticInstanceMux);
            if(m_pInstance != NULL) 
                m_pInstance = new basic_task_list();
            return *m_pInstance;
        }
    private:
        /**
         * Lock Object for this 
         */ 
        LockType_t               m_pLock;
        /**
         * The map for this Task list holder
         */ 
        std::map<int, std::list<basic_task*> > m_mapTaskOnCore; 
    };

    using task_list_t = basic_task_list;
}

#endif