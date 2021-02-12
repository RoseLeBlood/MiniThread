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

#include "mn_msg_task.hpp"

#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES

namespace mn {
    namespace ext {
        //-----------------------------------
        //  basic_message_task
        //-----------------------------------
        basic_message_task::basic_message_task(std::string strName, basic_task::priority uiPriority,
            unsigned short  usStackDepth)
            : basic_convar_task(strName, uiPriority, usStackDepth), 
            m_ltMessageQueueLock(), 
            m_qeMessageQueue(MN_THREAD_CONFIG_MSGTASK_MAX_MESSAGES, sizeof(task_message*)),
            m_cvMessage() { 

            m_qeMessageQueue.create(); 
        }

        //-----------------------------------
        //  post_msg
        //-----------------------------------
        void basic_message_task::post_msg(task_message* msg, unsigned int timeout) {
            automutx_t lock(m_ltMessageQueueLock);
            m_qeMessageQueue.enqueue(msg, timeout);
            m_cvMessage.signal(false);
        }

        //-----------------------------------
        //  on_task
        //-----------------------------------
        void* basic_message_task::on_task() {
            bool m_bRunning = true;
            task_message *msg;

            while(m_bRunning) {
                msg = NULL;

                {
                    m_ltMessageQueueLock.lock();

                    while (m_qeMessageQueue.is_empty())
                        wait(m_cvMessage, m_ltMessageQueueLock);

                    if(m_qeMessageQueue.is_empty())
                        continue;

                    m_qeMessageQueue.dequeue(msg);

                    m_ltMessageQueueLock.unlock();
                }

                if(msg) {
                    switch (msg->id) {
                    case Message_Exit:
                        m_bRunning = false;
                        break;
                    case Message_Child_Exit:
                        if(m_pChild) m_pChild->kill();
                        break;
                    case Message_Child_Resume:
                        if(m_pChild) m_pChild->resume();
                        break;
                    case Message_Child_Suspend:
                        if(m_pChild) m_pChild->suspend();
                        break;
                    default:
                        on_message(msg->id, msg->message);
                        break;
                    }; //switch (msg->id)
                } // if(msg)
            } //while(m_bRunning)

            return NULL;
        }
    }
}

#endif