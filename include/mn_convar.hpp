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
#ifndef MINLIB_ESP32_CONVAR_
#define MINLIB_ESP32_CONVAR_

/**
 *  Condition variables are an additon to the mini Thread 
 *  classes. If you want to include them, you need to define the 
 *  following in your config file with MN_THREAD_CONFIG_YES
 */
#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES


#include <list>
#include "mn_mutex.hpp"

namespace mn {
    namespace ext {
        /**
         * Forward declaration. We need to prevent a circular dependency
         * between the basic_convar_task class and the basic_condition_variable class.
         * 
         * @ingroup condition-varible
         */
        class basic_convar_task;

        /**
         *  Class implementation of condition variable 
         *  
         *  A condition variable isn't really a variable. It's a list
         *  of threads.
         *
         *  The design here is that a basic_convar_task "waits", and a condition_variable
         *  "signals". This affects where the public interfaces reside.
         */
        class basic_condition_variable {
            /**
             *  The basic_convar_task class and the basic_condition_variable class are interdependent.
             *  If we allow the basic_convar_task class to access the internals of the
             *  basic_condition_variable, we can reduce the public interface which is a
             *  good thing.
             */
            friend class basic_convar_task;
        public:
            /**
             *  Constructor to create a condition variable.
             */
            basic_condition_variable();

            /**
             *  Signal a thread waiting on this condition_variable (FIFO list).
             *  
             *  @param with_child_thread If true then signal the thread (i.e. task) 
             *  with all childs threads (i.e. tasks), if false then signal the thread (i.e. task)
             *  without all childs threads (i.e. tasks)
             */
            void signal(bool with_child_thread = true);

            /**
             * Signal all threads waiting on this condition_variable.
             * 
             * @param with_child_thread If true then signal the threads (i.e. tasks) 
             *  with all childs threads (i.e. tasks), if false then signal the threads (i.e. tasks)
             *  without all childs threads (i.e. tasks)
             */
            void broadcast(bool with_child_thread = true);

        private:
            /**
             *  Internal helper function to queue a basic_convar_task to 
             *  this condition_variable's wait list.
             * 
             * @param thread The basic_convar_task to add to the waiting list
             */
            void add_list(basic_convar_task *thread);
        protected:
            /**
             *  Protect the internal condition_variable state.
             */
            mutex_t                            m_mutex;
            /**
             *  Implementation of a wait list of Threads.
             */
            std::list<basic_convar_task*>    m_waitList;
        };

        using convar_t = basic_condition_variable;
    }
}
#endif

#endif