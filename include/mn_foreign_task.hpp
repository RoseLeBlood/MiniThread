/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_FOREIGIN_THREAD_
#define MINLIB_ESP32_FOREIGIN_THREAD_

#include "mn_task.hpp"

#if MN_THREAD_CONFIG_FOREIGIN_TASK_SUPPORT == MN_THREAD_CONFIG_YES

namespace mn {
    namespace ext {
      /**
       * Get the idle task for the current CPU.
       */ 
      #define FT_IDLE_TASK            foreign_task::get_idle_task()
      /**
       * Get the idle task for the given CPU.
       */ 
      #define FT_IDLE_TASK_ON(CPUID)  foreign_task::get_idle_task(CPUID)

      /**
       * Wrapper class around FreeRTOS's implementation of a task, 
       * for foreign miniThread Task 
       * 
       * @note using foreign_task::create_from to create all using LockObjects 
       * objects
       * 
       * @ingroup task
       */
      class foreign_task : public basic_task {
      private: 
        /**
         * Constructor - the current thread
         */ 
        foreign_task();
        /**
         * Constructor - from a FreeRTOS handle
         */ 
        foreign_task(void* t);
      public:
        /**
         *  Override - do nothings
         */ 
        int                   start(int uiCore = -1) { return 0; }
      public:
        /**
         * Get the foreign_task of idle task for the current CPU.
         *
         * @return The foreign_task of the idle task.
         */
        static foreign_task* get_idle_task();
        /**
         * Get the foreign_task of idle task for the given CPU.
         * 
         * @param cpuid The CPU to get the foreign_task for
         *
         * @return Idle foreign_task of a given cpu.
         */
        static foreign_task* get_idle_task(UBaseType_t cpuid);

        /**
         * Helper function to create a foreign_task from a FreeRTOS handle
         * 
         * @param foreign_handle The FreeRTOS handle
         * @param ret_error optional pointer for creating error code
         */ 
        static foreign_task* create_from(void* foreign_handle, int* ret_error = NULL);
      protected:
        /**
         * Override - Do nothings and never call
         * @return NULL
         */
        void*         on_task() { return NULL; }
      private:
        /**
         * Internal functions to create all usings locktypes
         * its call from create_from
         */ 
        int           __internal_create_usings_types();
      };

      using foreign_task_t = foreign_task;
    }
}
#endif

#endif