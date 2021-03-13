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
#ifndef MINLIB_aff55cd7_915b_4f3d_82b9_124e264a3df5_H_
#define MINLIB_aff55cd7_915b_4f3d_82b9_124e264a3df5_H_

#include <container/mn_node.hpp>
#include "mn_base_ptr.hpp"

namespace mn {
    namespace pointer {

        template <typename T, class TAllocator>
        class node_ptr  : pointer_ptr<T>  {
        public:
            using self_type = node_ptr<T, TAllocator>;
            using value_type = T;
            using pointer = T*;
            using reference = T&;
            using node_type = container::basic_value_node<pointer, TAllocator>*;

            node_ptr() 
                : m_pNode(0) { }
            explicit node_ptr(const pointer value) { 
                m_pNode = new node_type(value); }
        
            ~node_ptr() { 
                clear(); if(m_pNode) delete m_pNode;
            }

            void add_child(const pointer value) {
                node_type* newNode = new node_type(value); ;
                newNode->insert(m_pNode->Next);
            }
            void remove_child(const node_type* pChild) {
                if(pChild == NULL) return;

                node_type* nextNode = container::upcast(pChild->Next);

                if(this->m_pNode == pChild) {
                    nextNode->remove(); delete nextNode;
                } else {
                    remove_child(nextNode);
                }
            }
            node_type child() {
                return m_pNode;
            }
            void clear() {
                node_type it = upcast(m_pNode->Next);

                while (it != m_pNode) {
                    node_type nextIt = upcast(it->Next);
                    if(it) delete it;
                    it = nextIt;
                }
                m_root.reset();
            }

            pointer release() { 
                pointer tmp = m_pNode->Value; 
                m_pNode->Value = NULL; 
                return tmp; 
            }       
            void reset(pointer p = NULL) {
                clear();

                delete m_pNode;
                m_pNode = new node_type(p);
                
            }
            void swap(self_type &other) {                       
                m_pNode->swap(other);
            }

            const pointer get() const               { return m_pNode->Value;  }
            pointer get()                           { return m_pNode->Value; }
    
            const reference operator *() const          { return *m_pNode->Value; }
            reference operator *()                      { return *m_pNode->Value; }

            const pointer operator->() const            { return m_pNode->Value; }
            pointer operator->()                        { return m_pNode->Value; }

            node_ptr(const node_ptr &other) = delete;
            node_ptr const &operator=(const node_ptr &other) = delete;

        private:
            node_type m_pNode;
        };
    }
}

#endif
