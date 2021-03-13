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
#ifndef MINLIB_de6432e5_32f4_4ce2_bec3_0a8f4e20a0f4_H_
#define MINLIB_de6432e5_32f4_4ce2_bec3_0a8f4e20a0f4_H_

#include <stddef.h>
#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {

        template <typename T>
        class basic_any_ptr : pointer_ptr<T> {   
        public:
            using size_type = size_t;
            using pointer = T*;
            using const_pointer = const T*;
            using self_type = basic_any_ptr<T>;

            // Default Constructor

            /**
             * @brief Construct a new basic any ptr object
             */
            basic_any_ptr() : m_pObject(NULL) { }
            /**
             * @brief Construct a new basic any ptr object
             */
            basic_any_ptr(const_pointer pObject) 
                : m_pObject( const_cast<const_pointer>(m_pObject) ) { }

            /**
             * @brief Construct a new basic any ptr object from a copy
             */
            basic_any_ptr(const basic_any_ptr &other) 
                : m_pObject( const_cast<const_pointer>(other.m_pObject) ) { }

            /**
             * @brief Construct a new basic any ptr object from a copy
             * @tparam U        The type of the other any_ptr type
             * @param other      The copy
             */
            template <typename U> 
            basic_any_ptr(const basic_any_ptr<U> &other) 
                : m_pObject( const_cast<const_pointer>(other.m_pObject) ) { }

            /**
             * @brief Assignment Operator
             * @param other The assignment object
             * @return self
             */
            const self_type &operator = (const self_type &other) {
                m_pObject = const_cast<const_pointer>(other.m_pObject); return *this;
            }

            /**
             * @brief Assignment Operator
             * @tparam U The type of the other any_ptr type
             * @param other The assignment object
             * @return self
             */
            template <class U>
            const self_type &operator = (const basic_any_ptr<U> &other) {
                m_pObject = const_cast<const_pointer>(other.m_pObject); return *this;
            }

            /**
             * @brief Assign from object pointer
             */
            template <class U>
            const self_type &operator = (const U* pObject) {
                m_pObject = const_cast<const_pointer>(m_pObject); return (*this);
            }

            /**
             * @brief 
             */
            operator const_pointer() { return static_cast<T*>( m_pObject ); } 


            /**
             * @brief Implicit cast to non-const object pointer
             */
            template <class U> 
            operator const U*()             { return static_cast<U*>( m_pObject ); }

            /**
             * @brief Is the pointer null
             * @return true The pointer hold a null pointer
             * @return false The pointer is set
             */
            bool is_null()     { return (m_pObject == 0); }

        private:
            const_pointer m_pObject;
        };
    }
}

#endif