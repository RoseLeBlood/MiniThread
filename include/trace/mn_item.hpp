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
                uint8_t sec;
                /** time minits */
                uint8_t min;
                /** time hour */
                uint8_t hour;
                /** reserved */
                uint8_t reserved;
                /** current os ticks, or debug */
                uint32_t ticks; 
            };
            /** the buffer of the time - this variable are send */
            uint64_t bytetime;
        };
    };
    /**
     * Construtor
     * 
     * @param _Object The object 
     * @param _Name The human readebly debug name for the Object
     */ 
    basic_trace_item(void* _Object, std::string _Name)
        : m_pObject(_Object), m_strName(_Name), m_tTime(get_time()) {  }

    template <class TRET> 
        TRET get_as() { return static_cast<TRET>(m_pObject); }

    /**
     * Get the human readebly debug name of the object
     * @return The human readebly debug name of the object
     */ 
    std::string get_name() { return m_strName; }

    /** 
     * get the message as c-style string, for sending
     * style: time:message;
     * @return The message as c-style string 
     */
    const char* get_message();
protected:
    std::string time_to_string(time tm);
    /** 
     * get the user message as c-style string
     * example: object:name:state
     * @return The message as c-style string
     */
    virtual const char* message() = 0;
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


#endif