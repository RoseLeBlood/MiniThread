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
#ifndef __LIBMIN_THREAD_TRACE_ITEM_H_
#define __LIBMIN_THREAD_TRACE_ITEM_H_


#include <mn_config.hpp>
#if MN_THREAD_CONFIG_USE_TRACE == 1


#include "mn_item.hpp"
#include "mn_semaphore_item.hpp"

#include <string>
#include <list>

class mntrace {
    static mntrace* m_pInstance;
    mntrace() { }
public:
    using addr_t = uint32_t;

    struct named {
        addr_t addr;
        std::string name;
    };
    mntrace& instance() {
        if(m_pInstance == NULL) m_pInstance = new mntrace(); 
        return m_pInstance;
    }
    inline basic_trace_item* get_next() {
        basic_trace_item* _retItem = m_lstItems.front();
        m_lstItems.pop_front();
        return _retItem;
    }

    void add_mutex_item(void* object, int state);

    inline void set_queue_name(void* object, std::string name) {
       set_name((uint32_t)object, std::string("queu_") +name); }
    inline void set_semaphore_name(void* object, std::string name) {
       set_name((uint32_t)object, std::string("bism_") +name); }
    inline void set_mutex_name(void* object, std::string name) {
       set_name((uint32_t)object, std::string("mutx_") +name); }
    inline void set_eventgroup_name(void* object, std::string name) {
       set_name((uint32_t)object, std::string("evgr_") + name); }
    inline void set_task_name(void* object, std::string name) {
       set_name((uint32_t)object, std::string("task_") + name); }
protected:
    void set_name(addr_t object, std::string name);
    std::string get_name(addr_t object, std::string alternative = "(no)") {
        return alternative; }
private:
    std::list<named>                m_lstNamedTrace;
    std::list<basic_trace_item*>    m_lstItems;
};

#undef traceCREATE_MUTEX
#define traceCREATE_MUTEX( pxMutex ) \
    mntrace::instance().add_mutex_item(pxMutex, semaphore_trace_state::StateCreate)

#undef traceGIVE_MUTEX_RECURSIVE
#define traceGIVE_MUTEX_RECURSIVE( pxMutex ) \
    mntrace::instance().add_mutex_item(pxMutex, semaphore_trace_state::StateExitLock)

#undef traceGIVE_MUTEX_RECURSIVE_FAILED
#define traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex ) \
    mntrace::instance().add_mutex_item(pxMutex, semaphore_trace_state::StateFailedExitLock)

#undef traceTAKE_MUTEX_RECURSIVE
#define traceTAKE_MUTEX_RECURSIVE( pxMutex ) \
    mntrace::instance().add_mutex_item(pxMutex, semaphore_trace_state::StateEnterLock)

#undef traceTAKE_MUTEX_RECURSIVE_FAILED
#define traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex ) \
    mntrace::instance().add_mutex_item(pxMutex, semaphore_trace_state::StateFailedLock)


/*
traceQUEUE_CREATE

traceBLOCKING_ON_QUEUE_RECEIVE(xQueue)Indicates that the currently executing task is about to block following an attempt to read from an empty queue, or an attempt to ‘take’ an empty semaphore or mutex.
traceBLOCKING_ON_QUEUE_SEND(xQueue) Indicates that the currently executing task is about to block following an attempt to write to a full queue.

traceGIVE_MUTEX_RECURSIVE(xMutex) 	Called from within xSemaphoreGiveRecursive().
traceGIVE_MUTEX_RECURSIVE_FAILED(xMutex)	Called from within xSemaphoreGiveRecursive().

traceTAKE_MUTEX_RECURSIVE(xMutex)

traceQUEUE_CREATE(pxNewQueue)   
traceQUEUE_CREATE_FAILED()

traceCREATE_MUTEX(pxNewMutex)
traceCREATE_MUTEX_FAILED()

traceGIVE_MUTEX_RECURSIVE(xMutex)
traceGIVE_MUTEX_RECURSIVE_FAILED(xMutex)

traceCREATE_COUNTING_SEMAPHORE()
traceCREATE_COUNTING_SEMAPHORE_FAILED()

traceQUEUE_SEND(xQueue)
traceQUEUE_SEND_FAILED(xQueue)

traceQUEUE_RECEIVE(xQueue)
traceQUEUE_RECEIVE_FAILED(xQueue)
traceQUEUE_PEEK(xQueue)
traceQUEUE_SEND_FROM_ISR(xQueue)
traceQUEUE_SEND_FROM_ISR_FAILED(xQueue)
traceQUEUE_RECEIVE_FROM_ISR(xQueue)
traceQUEUE_RECEIVE_FROM_ISR_FAILED(xQueue)
traceQUEUE_DELETE(xQueue)

traceTASK_CREATE(xTask)	Called from within xTaskCreate() (or xTaskCreateStatic()) when the task is successfully created.
traceTASK_CREATE_FAILED(pxNewTCB)	Called from within xTaskCreate() (or xTaskCreateStatic()) when the task was not successfully created due to there being insufficient heap space available.
traceTASK_DELETE(xTask)	Called from within vTaskDelete().
traceTASK_DELAY_UNTIL()	Called from within vTaskDelayUntil().
traceTASK_DELAY()	Called from within vTaskDelay().
traceTASK_PRIORITY_SET(xTask,uxNewPriority)	Called from within vTaskPrioritySet().
traceTASK_SUSPEND(xTask)	Called from within vTaskSuspend().
traceTASK_RESUME(xTask)	Called from within vTaskResume().
traceTASK_RESUME_FROM_ISR(xTask)

traceTIMER_COMMAND_RECEIVED(pxTimer, xCommandID, xCommandValue)	Called within the timer service task each time it receives a command, before the command is actually processed.
traceTIMER_COMMAND_SEND(pxTimer, xCommandID, xOptionalValue, xStatus)	Called from within any API function that sends a command to the timer service task, for example, xTimerReset(), xTimerStop(), etc. xStatus will be pdFAIL if the command was not successfully sent to the timer command queue.
traceTIMER_CREATE(pxNewTimer)	Called from within xTimerCreate() if the timer was successfully created.
traceTIMER_CREATE_FAILED()	Called from within xTimerCreate() if the timer was not successfully created due to there being insufficient heap memory available.
traceTIMER_EXPIRED(pxTimer)	Called when a software timer expires, before the timer callback is executed.



*/


#endif


#endif