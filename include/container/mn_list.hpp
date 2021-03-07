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
#ifndef MINLIB_50f2b4fa_c256_450b_9595_0a6ed07718ca_H_
#define MINLIB_50f2b4fa_c256_450b_9595_0a6ed07718ca_H_

#include "../mn_algorithm.hpp"
#include "mn_node.hpp"

namespace mn {
    namespace container {

        template<typename TNodePtr, typename TPtr, typename TRef> 
        class list_node_iterator {
	    public:
            using  iterator_category = bidirectional_iterator_tag ;
            using  value_type = TNodePtr;
            using  pointer = TPtr;
            using  reference = TRef;
            using  difference_type = ptrdiff_t;
            using  node_type = TNodePtr;
            using  self_type = list_node_iterator<TNodePtr, TPtr, TRef>;

            explicit list_node_iterator()
                : m_node(NULL) {/**/}            
            explicit list_node_iterator(node_type node) 
                :  m_node(node) {/**/}
                
            template<typename UNodePtr, typename UPtr, typename URef> 
            list_node_iterator(const list_node_iterator<UNodePtr, UPtr, URef>& rhs)
                : m_node(rhs.node()) { }

            node_type node() const { return m_node; }

            reference operator*() const { return m_node->value; }
            pointer operator->() const { return &m_node->value; }

            self_type& operator++() { 
                m_node = m_node->Next; return *this; }

            self_type& operator--()  { 
                m_node = m_node->Prev; return *this; }

            self_type operator++(int) { 
                self_type copy(*this); ++(*this); return copy; }

            self_type operator--(int) {
                self_type copy(*this); --(*this); return copy; }

            bool operator == (const self_type& rhs) const {
                return rhs.m_node == m_node; }

            bool operator != (const self_type& rhs) const {
                return !(rhs == *this); }

        private:
            node_type m_node;
        };

        template<typename T, class TAllocator > 
        class basic_list {
        public:
            using self_type = basic_list<T, TAllocator>;
            using value_type = T;
            using allocator_type = TAllocator;
            using size_type = mn::size_t;
            using node_type = basic_node<T, TAllocator>;

            using iterator = list_node_iterator<node_type*, const value_type*, const value_type&>;
            using const_iterator = list_node_iterator<const node_type*, const value_type*, const value_type&>;

            static const size_type NodeSize = sizeof(node_type);

            /**
             * @brief Construct a new basic list object
             */
            explicit basic_list(const allocator_type& allocator = allocator_type() )
                : m_allocator(allocator) { m_root.reset(); }

            /**
             * @brief Construct a new basic list object
             */
            template<class InputIterator> 
            basic_list(InputIterator first, InputIterator last, const allocator_type& allocator = allocator_type())
                : m_allocator(allocator) {
                    m_root.reset();
                    assign(first, last);
            }
            /**
             * @brief Construct a new basic list object
             */
            basic_list(const basic_list& rhs, const allocator_type& allocator = allocator_type())
                : m_allocator(allocator) {
                    m_root.reset();
                    assign(rhs.begin(), rhs.end());
            }
            /**
             * @brief Destroy the basic list object
             */
            ~basic_list() {
                clear();
            }

            iterator begin()                { return iterator((m_root.Next)); }
            const_iterator begin() const    { return const_iterator((m_root.Next));  }

            iterator end()                  { return iterator(&m_root); }
            const_iterator end() const      { return const_iterator(&m_root); }

            const T& front() const          { assert(!empty()); return (m_root.Next)->Value; }
            T& front()                      { assert(!empty()); return (m_root.Next)->Value; }

            const T& back() const           { assert(!empty()); return (m_root.Prev)->Value; }
            T& back()                       { assert(!empty()); return (m_root.Prev)->Value; }

            void push_front(const T& value) {
                node_type* newNode = construct_node(value);
                newNode->insert(m_root.Next);
            }
            inline void pop_front() {
                assert(!empty());
                node_type* frontNode = (m_root.Next);
                frontNode->remove();
                destruct_node(frontNode);
            }

            void push_back(const T& value) {
                node_type* newNode = construct_node(value);
                newNode->insert(&m_root);
            }
            inline void pop_back() {
                assert(!empty());
                node_type* backNode = (m_root.Prev);
                backNode->remove();
                destruct_node(backNode);
            }

            iterator insert(iterator pos, const T& value) {
                node_type* newNode = construct_node(value);
                newNode->insert(pos.node());
                return iterator(newNode);
            }

            iterator erase(iterator it) {
                assert(it.node()->is());
                iterator itErase(it);
                ++it;
                itErase.node()->remove();
                destruct_node(itErase.node());
                return it;
            }

            iterator erase(iterator first, iterator last) {
                while (first != last) first = erase(first);
                return first;
            }

            template<class InputIterator>
            void assign(InputIterator first, InputIterator last) {
                clear();
                while (first != last) {
                    push_back(*first);
                    ++first;
                }
            }

            bool empty() const { 
                return !m_root.is(); 
            }

            void clear() {
                node_type* it = (m_root.Next);

                while (it != &m_root) {
                    node_type* nextIt = (it->Next);
                    destruct_node(it);
                    it = nextIt;
                }
                m_root.reset();
            }

            size_type size() const {
                const node_type* it = (m_root.Next);
                size_type size(0);

                while (it != &m_root) {
                    ++size;
                    it = (it->Next);
                }
                return size;
            }

            const allocator_type& get_allocator() const { 
                return m_allocator; 
            }
            void set_allocator(const allocator_type& allocator) {
                m_allocator = allocator;
            }

            basic_list& operator = (const basic_list& rhs) {
                if (this != &rhs) {
                    assign(rhs.begin(), rhs.end());
                }
                return *this;
            }
        private:
            node_type* construct_node(const T& value) {
                //void* mem = m_allocator.alloc(NodeSize);
                //return new (mem) node(value);
                return new node_type(value);
            }
            void destruct_node(node_type* n) {
                //n->~node_type(); m_allocator.free(n);
                if(n) delete n;
            }

            allocator_type  m_allocator;
            node_type       m_root;
        };

        /**
         * @brief List type witch allocated in global heap
         * @tparam T The holding type for the value 
         */
        template<typename T > 
        using list = basic_list<T, memory::default_allocator_t >;

        /**
         * @brief List type witch allocated on the staked memory buffer
         * 
         * @tparam T The holding type for the value 
         * @tparam TBUFFERSIZE The size of the memory buffer
         */
        template<typename T, int TBUFFERSIZE> 
        using stacked_list = basic_list<T, memory::basic_allocator_stack<TBUFFERSIZE> >;
    }
}

#endif