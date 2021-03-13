/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef MINLIB_ASTL_CLONE_PTR_
#define MINLIB_ASTL_CLONE_PTR_

#include "mn_algorithm.hpp"
#include <stdlib.h>
#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {
        
        template<typename T>
        class clone_ptr_interface {
        protected:
            using pointer = T*;
            using value_type = T;

            virtual pointer clone(pointer src) {
                pointer _clone = NULL;
                
                if(src != NULL) {
                    _clone = new value_type();
                    memcpy(_clone, src, sizeof(src));
                }
                return _clone;
            }
        };

        template<typename T, class TInterface = clone_ptr_interface<T> >
        class basic_clone_ptr : public TInterface,  pointer_ptr<T> 
        {
        public:
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using self_type = basic_clone_ptr<value_type >;
            using cloneptrinterface = TInterface;
            
            basic_clone_ptr() : m_ptr(0) { }
            explicit basic_clone_ptr(T *v) : m_ptr(v) { }
            
            //basic_clone_ptr(save_ptr const &other) { if(other.get() != NULL)  m_ptr = other->clone(); }
            basic_clone_ptr(const self_type& other) { if(other.get() != NULL)  m_ptr = other->clone(); }

            ~basic_clone_ptr() { if(m_ptr) delete m_ptr; }

            pointer get()                           { return m_ptr;  }
    
            value_type operator *()                { return *m_ptr; }
            pointer operator->()                    { return m_ptr; }
            pointer release()                       { pointer tmp = m_ptr;  m_ptr = NULL; return tmp; }

            void reset(pointer p = NULL)            { if(m_ptr) delete m_ptr; m_ptr = p; }

            void swap(self_type &other) { 
                pointer tmp = clone();             
                
                reset(other->clone());
                other.reset(tmp);
            }

            self_type& operator = (const self_type& other) {
                if(other.m_ptr != this.m_ptr) {
                    reset(other.clone()); 
                }     
                return *this;
            }
        
            pointer clone() {
                return clone(m_ptr);
            }
        private:
            pointer m_ptr;
        };
    }
}

#endif