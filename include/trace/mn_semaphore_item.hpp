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

#ifndef __LIBMIN_THREAD_TRACE_SEMAPHORE_H_
#define __LIBMIN_THREAD_TRACE_SEMAPHORE_H_

#include "mn_item.hpp"
#include "mn_allocator.hpp"

namespace mn {
    namespace trace {
        /** The states of a semaphore_trace_item message */
        enum class semaphore_trace_state {
            StateCreate = 0x010,    /*!< The message state on create the semaphore */
            StateDelete = 0x000,    /*!< The message state on delete the semaphore */
            StateEnterLock = 0x101, /*!< The message state on lock the semaphore */
            StateExitLock = 0x100,  /*!< The message state on unlock the semaphore */
        };

        /**
        * Trace item for a semaphore object
        */ 
        class semaphore_trace_item : public basic_trace_item {
        public:
            /**
            * Construtor
            * 
            * @param _Object The mutex object of this message
            * @param _Name The human readebly debug name for this mutex
            */
            semaphore_trace_item(semaphore_trace_state _State, basic_semaphore* _Object, std::string _Name)
                : basic_trace_item(_Object, _Name), m_State(_State) {  }

            /** 
            * get the message as c-style string
            * @return The message as c-style string (object:name:state)
            */
            const char* message();

            semaphore_trace_state get_state() const { return m_State; }
        private:
            semaphore_trace_state m_State;
        };
    }
}

#endif