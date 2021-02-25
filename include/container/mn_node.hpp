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
#ifndef MINLIB_d80db0c1_69c6_478d_b3a8_364a8bc23f0b_H_
#define MINLIB_d80db0c1_69c6_478d_b3a8_364a8bc23f0b_H_

#include "../mn_iterator.hpp"
#include "../mn_allocator.hpp"

namespace mn {
    namespace container {

        template <class TALLOCATOR>
        struct basic_node : public memory::basic_alloc_object<basic_node, TALLOCATOR> {
            using self_type = basic_node<TALLOCATOR>;
            using base_type = memory::basic_alloc_object<basic_node, TALLOCATOR>;

            using node_type = basic_node<TALLOCATOR>*;
            using reference = basic_node<TALLOCATOR>&;

            /**
             * @brief Construct a new base node object
             */
            basic_node() : Next(0), Prev(0) { }
            
            /**
             * @brief Inserts this standalone node before the node pNext in pNext's . 
             */
            void insert(node_type pNext) {
                Next = pNext;
                Prev = pNext->Prev;
                pNext->Prev->Next = this;
                pNext->Prev = this;
            }    
            /**
             * @brief Removes this node from the  it's in. 
             */
            void remove() {
                Next->Prev = Prev;
                Prev->Next = Next;
            }

            bool is() const { 
                return this != Next; }
            
            /**
             * @brief Removes [pFirst,pLast) from the  it's in and inserts it before this in this node's .
             */
            void splice(node_type first, node_type last) {
                last->Prev->Next  = this;
                first->Prev->Next = last;
                this->Prev->Next  = first;

                node_type const pTemp = this->Prev;
                this->Prev  = last->Prev;
                last->Prev  = first->Prev;
                first->Prev = pTemp;
            } 
            /**
             * @brief Reverses the order of nodes in the circular  this node is a part of.
             */
            void reverse() {
                node_type pNode = this;

                do {
                    if(pNode != NULL) {
                        node_type const pTemp = pNode->Next;
                        pNode->Next = pNode->Prev;
                        pNode->Prev = pTemp;
                        pNode         = pNode->Prev;
                    }
                } while(pNode != this);
            }
            /**
             * @brief Add a range of elements
             */
            void insert_range(node_type pFirst, node_type pFinal) {
                Prev->Next = pFirst; pFirst->Prev = Prev;
                Prev       = pFinal; pFinal->Next = this;
            }
            void swap(reference other) {
                const basic_node temp(this);
                this = other; other = temp;

                if(this.Next == &other) this.Next = this.Prev = this;
                else this.Next->Prev = this.Prev->Next = this;

                if(other.Next == this) other.Next = other.Prev = &other;
                else other.Next->Prev = other.Prev->Next = &other;

            }
            /**
             * @brief remove a range of elements
             */   
            static void remove_range(node_type pFirst, node_type pFinal) {
                pFinal->Next->Prev = pFirst->Prev;
                pFirst->Prev->Next = pFinal->Next;
            }    
            /**
             * @brief The pointer to the next node
             */
            node_type Next;
            /**
             * @brief The pointer to the prev node
             */
            node_type Prev;
        };

        template <class TALLOCATOR>
        inline void swap(basic_node<TALLOCATOR>& a, basic_node<TALLOCATOR>& b) {
            a.swap(b);
        }

        
        template<typename T, class TAllocator> 
        struct basic_value_node : public basic_node<TAllocator> {
            basic_value_node() 
                : basic_node<TAllocator>(), Value(0) { }

            explicit basic_value_node(const T& value) 
                : basic_node<TAllocator>(), Value(value) { }

            T  Value;
        };

        template<typename T, class TAllocator> 
        inline basic_value_node<T, TAllocator>* upcast(basic_node<TAllocator>* n) {
	        return static_cast<basic_value_node<T, TAllocator>*>(n);
	    }
    }
}

#endif
