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
#ifndef _MINLIB_STAK_H_
#define _MINLIB_STAK_H_

#include "../mn_config.hpp"
#include "../mn_error.hpp"

#include <stdio.h>
#include <malloc.h>
#include <stdint.h>

namespace mn {
    namespace container {
        /**
         * A simple template for a Stack

         */ 
        template<typename TType>
        class basic_stack {
        public:
            using basic_type_t = TType;

            /**
             * This constructor use malloc to create the buffer
             * @param nSize The size of the stack
             */ 
            explicit basic_stack(size_t nSize) { 
                internal_create(nSize, (basic_type_t*)malloc( size_of(basic_type_t) * nSize), 0 ); }

            /**
             * This constructor use a given address as buffer
             * @param nSize The size of the stack (The last position [addr+(nSize)] )
             * @param addr The first buffer or adress to use as stack 
             */ 
            basic_stack(size_t nSize, basic_type_t* addr) { 
                internal_create(nSize, addr, 0); }

            /**
             * This constructor use a given address as buffer
             * @param nSize The size of the stack (The last position [addr+(nSize)] )
             * @param addr The first buffer or adress to use as stack 
             * @param offset The offset, when start position in the buffer not address 0 is
             */ 
            basic_stack(size_t nSize, basic_type_t* addr, int offset) { 
                internal_create(nSize, addr, offset); }

            /**
             *  Add an item to the stack
             *
             *  @param item The item you are adding.
             *  @return - NO_ERROR the item was added
             *          - ERR_UNKN the stack is full
             */
            int push(basic_type_t item) {
                int _ret = ERR_UNKN;

                if(m_iCurrent < m_iLast - 1) {
                    m_iCurrent++;
                    m_ulAdrr[m_iCurrent] = item;
                    _ret = NO_ERROR;
                } 
                return _ret;
            }
            /**
             * Pop an item from the stack
             * @param item Where the item you are removing will be returned to.
             * @return  - NO_ERROR the item was removed
             *          - ERR_NULL the given item was null
             *          - ERR_UNKN the stack is empty
             */ 
            int pop(basic_type_t* item) {
                if(item == NULL) return ERR_NULL;

                if(count() > 0 ) {
                    basic_type_t x = m_ulAdrr[m_iCurrent];
                    m_iCurrent--;
                    *item = x;
                    return NO_ERROR;
                }
                return ERR_UNKN;
            }
            /**
             * get the current obj from the stack, this will not remove it
             * 
             * @param item Where the item you are getting will be returned to.
             * @return  - NO_ERROR the item was removed
             *          - ERR_NULL the given item was null
             *          - ERR_UNKN the stack is empty
             */ 
            int peek(basic_type_t* item) {
                if(item == NULL) return ERR_NULL;

                if(count() > 0 ) {
                    basic_type_t x = m_ulAdrr[m_iCurrent];
                    *item = x;
                    return NO_ERROR;
                }
                return ERR_UNKN;
            }
            /**
             * Reset the stack, will remove all objects in the stack
             */ 
            void reset() {
                internal_reset();
            }

            /**
             * Get the count of the stack 
             * @return The count of the stack 
             */ 
            int count()     { return m_iCurrent + 1; }
            /**
             * Get the size of the stack
             * @return The size of the stack.
             */ 
            int size()      { return m_ulSize; }
            
            /**
             *  Is the stack empty?
             *  @return true the stack is empty and false when not
             */
            bool is_empty() { return (m_iCurrent == -1); }
            /**
             *  Is the stack full?
             *  @return true the stack is full and false when not
             */
            bool is_full()  { return (m_iCurrent == (m_iLast - 1) ); }
            
            /**
             * How many empty spaves are currently left in the stack.
             */ 
            int get_left()  { return m_ulSize - (m_iCurrent + 1); }
        private:
            /**
             * the internal creating function for the stack
             * @param nSize The size of the stack (The last position [addr+(nSize)] )
             * @param addr The first buffer or adress to use as stack 
             * @param offset The offset, when start position in the buffer not address 0 is
             */ 
            void internal_create(size_t nSize, basic_type_t* addr, int offset) {
                m_ulAdrr = (addr); 
                m_ulSize = (nSize - offset);

                m_iFirst = (offset - 1); 
                m_iCurrent = (m_iFirst);
                m_iLast = (nSize);
            }
            /**
             * The internal reset function
             */ 
            void internal_reset() {
                while(!is_empty()) 
                    pop();
                m_iCurrent = m_iFirst;
            }
        private:
            basic_type_t *m_ulAdrr;
            size_t m_ulSize;
            size_t m_iFirst;
            size_t m_iLast;
            size_t m_iCurrent;
        };

        using stack_t = basic_stack<MN_THREAD_CONFIG_STACK_TYPE>;

        using uint8_stack_t  = basic_stack<uint8_t>;
        using uint16_stack_t = basic_stack<uint16_t>;
        using uint32_stack_t = basic_stack<uint32_t>;
        using uint64_stack_t = basic_stack<uint64_t>;

        using int8_stack_t  = basic_stack<int8_t>;
        using int16_stack_t = basic_stack<int16_t>;
        using int32_stack_t = basic_stack<int32_t>;
        using int64_stack_t = basic_stack<int64_t>;

        using string_stack_t = basic_stack<const char*>;
        using float_stack_t = basic_stack<float>;
    }
}

#endif