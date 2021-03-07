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
        
        template <class TNODE>
        class basic_node_itarrator {
        public:
            using iterator_category = bidirectional_iterator_tag;
            using value_type = typename TNODE::value_type;
            using pointer = value_type*;
            using reference = value_type&;
            using self_type = basic_node_itarrator<TNODE>;
            using difference_type = ptrdiff_t;
            using node_type = TNODE;
            
            basic_node_itarrator() : m_pNode(NULL) { } 
            basic_node_itarrator(node_type _pNode) : m_pNode(_pNode) { }
            basic_node_itarrator(const self_type& _pOther ) : m_pNode(_pOther.m_pNode) { } 

            self_type& operator ++ () {
                if(m_pNode) m_pNode = m_pNode->Next;
                return *this;
            } 
            self_type& operator -- () {
                if(m_pNode) m_pNode = m_pNode->Prev;
                return *this;
            } 

            reference operator*() const { return m_pNode->get(); }
            pointer operator->() const { return &m_pNode->get(); }

            self_type operator++(int) { 
                self_type copy(*this); ++(*this); return copy; }

            self_type operator--(int) {
                self_type copy(*this); --(*this); return copy; }

            bool operator == (const self_type& rhs) const {
                return rhs.m_pNode == m_pNode; }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); }
        private:
            node_type m_pNode;
        };

        template <class TNODE>
        class basic_node_itarrator_res {
        public:
            using iterator_category = bidirectional_iterator_tag;
            using value_type = typename TNODE::value_type;
            using pointer = value_type*;
            using reference = value_type&;
            using self_type = basic_node_itarrator_res<TNODE>;
            using difference_type = ptrdiff_t;
            using node_type = TNODE;
            
            basic_node_itarrator_res() : m_pNode(NULL) { } 
            basic_node_itarrator_res(node_type _pNode) : m_pNode(_pNode) { }
            basic_node_itarrator_res(const self_type& _pOther ) : m_pNode(_pOther.m_pNode) { } 

            self_type& operator ++ () {
                if(m_pNode) m_pNode = m_pNode->Prev;
                return *this;
            } 
            self_type& operator -- () {
                if(m_pNode) m_pNode = m_pNode->Next;
                return *this;
            } 

            reference operator*() const { return m_pNode->get(); }
            pointer operator->() const { return &m_pNode->get(); }

            self_type operator++(int) { 
                self_type copy(*this); ++(*this); return copy; }

            self_type operator--(int) {
                self_type copy(*this); --(*this); return copy; }

            bool operator == (const self_type& rhs) const {
                return rhs.m_pNode == m_pNode; }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); }
        private:
            node_type m_pNode;
        };

        template <typename T, class TALLOCATOR>
        class basic_node {
            MNALLOC_OBJECT(TALLOCATOR);

            using iterator_category = bidirectional_iterator_tag;
            using self_type = basic_node<T, TALLOCATOR>;
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const pointer;
            using reference = value_type&;
            using const_reference = const reference;
            
            using iterator = basic_node_itarrator<self_type>;
            using const_iterator = const basic_node_itarrator<const self_type>;

            using reverse_iterator = basic_node_itarrator_res<self_type>;
            using const_reverse_iterator = const basic_node_itarrator_res<const self_type>;
            /**
             * @brief Construct a new base node object
             */
            explicit basic_node(value_type value ) 
                : Next(0), Prev(0), m_tValue(value) { }

            basic_node(self_type* _pNext, self_type* _pPrev)  
                : Next(_pNext), Prev(_pPrev), m_tValue( value_type() ) { }
            basic_node(const self_type& _pOther) 
                : Next(_pOther.Next), Prev(_pOther.Prev), m_tValue(_pOther.m_tValue) { }

            iterator            begin()         { return iterator( root() ); }
            reverse_iterator    rbegin()        { return reverse_iterator( last() ); }
            const_iterator      cbegin() const  { return const_iterator( root() ); }
            
            iterator            end()           { return iterator( last() ); }
            reverse_iterator    rend()          { return reverse_iterator( root() ); }
            const_iterator      cend() const    { return const_iterator( last() ); }
            
            self_type*   root() {
                return (Prev != 0) ? Prev->root() : this;
            }
            const self_type* root() const {
                return (Prev != 0) ? Prev->root() : this;
            }
            self_type*   last() {
                return (Next != 0) ? Next->last() : this;
            }
            const self_type* last() const {
                return (Next != 0) ? Next->last() : this;
            }

            /**
             * @brief Inserts this standalone node before the node pNext in pNext's . 
             */
            void insert(self_type* pNext) {
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

            bool is() const { return this != Next; }
            
            /**
             * @brief Removes [pFirst,pLast) from the  it's in and inserts it before this in this node's .
             */
            void splice(self_type* first, self_type* last) {
                last->Prev->Next  = this;
                first->Prev->Next = last;
                this->Prev->Next  = first;

                self_type* pTemp = this->Prev;
                this->Prev  = last->Prev;
                last->Prev  = first->Prev;
                first->Prev = pTemp;
            } 
            /**
             * @brief Reverses the order of nodes in the circular  this node is a part of.
             */
            void reverse() {
                self_type*  pNode = this;

                do {
                    if(pNode != NULL) {
                        self_type* pTemp = pNode->Next;
                        pNode->Next = pNode->Prev;
                        pNode->Prev = pTemp;
                        pNode         = pNode->Prev;
                    }
                } while(pNode != this);
            }
            /**
             * @brief Add a range of elements
             */
            void insert_range(self_type*  pFirst, self_type*  pFinal) {
                Prev->Next = pFirst; pFirst->Prev = Prev;
                Prev       = pFinal; pFinal->Next = this;
            }
            void swap(self_type& other) {
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
            static void remove_range(self_type* pFirst, self_type* pFinal) {
                pFinal->Next->Prev = pFirst->Prev;
                pFirst->Prev->Next = pFinal->Next;
            }   

            value_type get() { return m_tValue; }

            bool operator == (const self_type& rhs) const {
                if( rhs.Next != Next) return false; 
                if( rhs.Prev != Prev) return false; 
                return (rhs.m_tValue == m_tValue);
            }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); 
            }

            /**
             * @brief The pointer to the next node
             */
            self_type*  Next;
            /**
             * @brief The pointer to the prev node
             */
            self_type*  Prev; 
        private:
            value_type m_tValue;
        };
        
        MNALLOC_OBJECT_DTWO( basic_node, typename, T, class, TAllocator );

        template <typename T, class TALLOCATOR>
        inline void swap(basic_node<T, TALLOCATOR>& a, basic_node<T, TALLOCATOR>& b) {
            a.swap(b);
        }
        /**
         * @brief Node type witch allocated in global heap
         */
        template<typename T>
        using node = basic_node<T, memory::default_allocator_t>;
    }
}

#endif
