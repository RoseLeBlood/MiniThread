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

#ifndef __LIBMIN_THREAD_TRACE_H_
#define __LIBMIN_THREAD_TRACE_H_

#include <mn_micros.hpp>
#include <string>

/**
 * Basic app trace item. A item is a entry in trace task list 
 * and send to the trace output
 * 
 * To use this, you need to subclass it. All of your trace_item should
 * be derived from the basic_trace_item class. Then implement the virtual message
 * function.
 */ 
class basic_trace_item {
public:
    /** The item creating time */
    struct time {
        union {
            struct {
                /** time sec */
                uint8_t sec : 6;
                /** time minits */
                uint8_t min  : 6;
                /** time hour */
                uint8_t hour  : 4;
                /** reserved */
                uint16_t reserved;
                /** current os ticks, or debug */
                uint32_t ticks; 
            }
            /** the buffer of the time - this variable are send */
            uint64_t time;
        }
    };
    /**
     * Construtor
     * 
     * @param _Object The object 
     * @param _Name The human readebly debug name for the Object
     */ 
    basic_trace_item(void* _Object, std::string _Name)
        : m_pObject(_Object), m_strName(_Name), m_tTime(get_time()) {  }

    /** 
     * get the message as c-style string
     * 
     * @return The message as c-style string 
     */
    virtual const char* message() = 0;

    template <class TRET> 
        TRET get_as() { return static_cast<TRET>(m_pObject); }

    /**
     * Get the human readebly debug name of the object
     * @return The human readebly debug name of the object
     */ 
    std::string get_name() { return m_strName; }
private:   
    /**
     * Get the current creating time
     * @return The current creating time
     */ 
    static time get_time();
protected:
    void*  m_pObject;
    std::string m_strName;
private:
    time m_tTime;
};


/** The states of a semaphore_trace_item message */
enum class semaphore_trace_state {
    StateCreate,    /*!< The message state on create the semaphore */
    StateDelete,    /*!< The message state on delete the semaphore */
    StateEnterLock, /*!< The message state on lock the semaphore */
    StateExitLock,  /*!< The message state on unlock the semaphore */
    StateTryLock    /*!< The message state on try_lock the semaphore */
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
     * 
     * @return The message as c-style string 
     */
    const char* message();

    const semaphore_trace_state get_state() const { return m_State; }
private:
    semaphore_trace_state m_State;
};

#endif