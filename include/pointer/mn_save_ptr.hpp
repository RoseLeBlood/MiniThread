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
#ifndef _MINILIB_7bee8ff5_abe8_4176_bc96_f42b0e71632b_H_
#define _MINILIB_7bee8ff5_abe8_4176_bc96_f42b0e71632b_H_

#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {

        template <typename T>
        class basic_save_ptr  : pointer_ptr<T>  {
        public:
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using self_type = basic_save_ptr<T>;
            
            basic_save_ptr() : m_ptr(0)                 { }
            explicit basic_save_ptr(T *v) : m_ptr(v)    { }

            ~basic_save_ptr()                           { if(m_ptr) delete m_ptr; }
    
            value_type const *get() const               { return m_ptr;  }
            value_type *get()                           { return m_ptr; }
    
            value_type const &operator *() const        { return *m_ptr; }
            value_type &operator *()                    { return *m_ptr;  }
            value_type const *operator->() const        {  return m_ptr; }
            value_type *operator->()                    {  return m_ptr; }

            value_type *release() { 
                value_type *tmp = m_ptr; 
                m_ptr = NULL; 
                return tmp; 
            }       
            void reset(value_type *p = NULL) {
                if(m_ptr != NULL)  delete m_ptr;
                m_ptr = p;
            }
            void swap(self_type &other) {                         
                value_type *tmp = other.m_ptr;
                other.m_ptr = m_ptr;
                m_ptr = tmp;
            }

            basic_save_ptr(const basic_save_ptr &other) = delete;
            basic_save_ptr const &operator=(const basic_save_ptr &other) = delete;

        private:
            pointer *m_ptr; 
        };
    }
}


#endif 