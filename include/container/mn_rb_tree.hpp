/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.  
*/
#ifndef _MINILIB_4b715a5a_43d4_4442_88e5_4dcc9d8860bb_H_
#define _MINILIB_4b715a5a_43d4_4442_88e5_4dcc9d8860bb_H_

#include "../mn_config.hpp"
#include "../mn_typetraits.hpp"
#include <stddef.h>
#include "../mn_allocator.hpp"
#include "../mn_algorithm.hpp"


namespace mn {
	namespace container {
        namespace internal {

            template<typename TKey> 
            struct rb_tree_key_wrapper {
                TKey    key;
                rb_tree_key_wrapper() {}
                rb_tree_key_wrapper(const TKey& key_): key(key_) {}
                const TKey& get_key() const { return key; }
            };

            template<typename TKey> 
            struct rb_tree_traits {
                using key_type = TKey;
                using value_type = rb_tree_key_wrapper<TKey>;
            };
        } 

        enum class rb_tree_color {
            red,
            black
        };
        
        template<typename TVALUE, class TAllocator>
        struct rb_tree_node {
            MNALLOC_OBJECT(TAllocator);

            rb_tree_node() { }
            rb_tree_node(rb_tree_node* node)
                : left(node), parent(node), right(node), color(rb_tree_color::black) { }

            rb_tree_node(rb_tree_color color_, rb_tree_node* left_, rb_tree_node* right_, rb_tree_node* parent_)
                : left(left_), parent(parent_), right(right_), color(color_) { }

            rb_tree_node(const rb_tree_node& other) 
                : left(other.left), parent(other.parent), right(other.right), color(other.color) { }

            void swap(rb_tree_node& other) {
                rb_tree_node tmp(this);
                left = other.left; other.left = tmp.left;
                parent = other.parent; other.parent = tmp.parent;
                right = other.right; other.right = tmp.right;
                value = other.value; other.left = tmp.value;
                color = other.color; other.color = tmp.color;
            }

            rb_tree_node*           left;
            rb_tree_node*           parent;
            rb_tree_node*           right;
            TVALUE                  value;
            rb_tree_color           color;
        };
        MNALLOC_OBJECT_DTWO(rb_tree_node, typename, TVALUE, class, TAllocator );
        
        template<typename TVALUE, class TAllocator>
        void swap(rb_tree_node<TVALUE, TAllocator>& a, rb_tree_node<TVALUE, TAllocator>& b) {
            a.swap(b);
        }

        template<class TTreeTraits, class TAllocator>
        class base_rb_tree  {
        public:
            using key_type = typename TTreeTraits::key_type;
            using value_type = typename TTreeTraits::value_type;
            using allocator_type = TAllocator;
            using self_type = base_rb_tree<TTreeTraits, TAllocator>;
            using size_type = mn::size_t;
            using node_type = rb_tree_node<value_type, TAllocator>;

            typedef void (*TravFunc)(node_type* n, size_type left, size_type depth);

            explicit base_rb_tree(const allocator_type& allocator = allocator_type())
        	    : m_size(0), m_allocator(allocator) {
                ms_sentinel.color       = rb_tree_color::black;
                ms_sentinel.left        = &ms_sentinel;
                ms_sentinel.right       = &ms_sentinel;
                ms_sentinel.parent      = &ms_sentinel;
                m_root                  = &ms_sentinel;
            }
            ~base_rb_tree() {
                clear();
            }

            node_type* insert(const value_type& v) {
                node_type* iter(m_root);
                node_type* parent(&ms_sentinel);
                while (iter != &ms_sentinel)
                {
                    parent = iter;
                    if (iter->value.get_key() < v.get_key())
                        iter = iter->right;
                    else if (v.get_key() < iter->value.get_key())
                        iter = iter->left;
                    else    // v.key == iter->key
                            return iter;
                }

                node_type* new_node = new node_type();
                new_node->color = rb_tree_color::red;
                new_node->value = v;
                new_node->left  = &ms_sentinel;
                new_node->right = &ms_sentinel;
                new_node->parent = parent;

                if (parent != &ms_sentinel) {
                    if (v.get_key() < parent->value.get_key())
                            parent->left = new_node;
                    else
                            parent->right = new_node;
                } else {   // empty tree
                    m_root = new_node;
                }

                rebalance(new_node);
                validate();
                ++m_size;
                return new_node;
            }
            node_type* find_node(const key_type& key) {
                node_type* iter(m_root);
                while (iter != &ms_sentinel) {
                    const key_type& iter_key = iter->value.get_key();
                    if (iter_key < key)
                            iter = iter->right;
                    else if (key < iter_key)
                            iter = iter->left;
                    else // key == iter->key
                            return iter;
                }
                return 0;       // not found
            }

            size_type erase(const key_type& key) {
                node_type* toErase = find_node(key);
                size_type erased(0);
                if (toErase != 0) {
                    erase(toErase);
                    erased = 1;
                }
                return erased;
            }
            void erase(node_type* n) {
                assert(m_size > 0);
                node_type* toErase;

                if (n->left == &ms_sentinel || n->right == &ms_sentinel) {
                    toErase = n;
                } else {
                    toErase = n->right;
                    while (toErase->left != &ms_sentinel)
                        toErase = toErase->left;
                }

                node_type* eraseChild = (toErase->left != &ms_sentinel ? toErase->left : toErase->right);
                eraseChild->parent = toErase->parent;

                if (toErase->parent != &ms_sentinel) {
                    if (toErase == toErase->parent->left)
                        toErase->parent->left = eraseChild;
                    else
                        toErase->parent->right = eraseChild;
                } else {
                    m_root = eraseChild;
                }

                n->value = toErase->value;

                if (toErase->color == rb_tree_color::black)
                    rebalance_after_erase(eraseChild);

                free_node(toErase, false);

                validate();
                --m_size;
            }

            void clear() {
                if (!empty()) {
                    free_node(m_root, true);
                    m_root = &ms_sentinel;
                    m_size = 0;
                }
            }
            void swap(base_rb_tree& other) {
                if (&other != this) {
                    validate();
                    assert(m_allocator == other.m_allocator);
                    m_root.swap(other.m_root);
                    mn::swap<size_type>(m_size, other.m_size);
                    validate();
                }
            }

            bool empty() const { 
                return m_size == 0; 
            }
            size_type size() const {
                return m_size;
            }

            const node_type* begin() {
                node_type* iter(0);
                if (m_root != &ms_sentinel) {
                    iter = m_root;
                    while (iter->left != &ms_sentinel)
                        iter = iter->left;
                }
                return iter;
            }
            const node_type* find_next(node_type* n) const {
                node_type* next(0);

                if (n != 0) {
                    if (n->right != &ms_sentinel) {
                        next = n->right;
                        while (next->left != &ms_sentinel)
                            next = next->left;
                    } else if (n->parent != &ms_sentinel) {
                        if (n == n->parent->left) {
                            return n->parent;
                        } else {
                            next = n;

                            while (next->parent != &ms_sentinel) {
                                if (next == next->parent->right)
                                    next = next->parent;
                                else {
                                    return next->parent;
                                }
                            }
                            next = 0;
                        }
                    } else {   // 'n' is root.
                        assert(n == m_root);
                    }
                }
                return next;
            }
            const size_type size(const node_type* n) const {
       		    return n == &ms_sentinel ? 0 : 1 + size(n->left) + size(n->right);
            }
            void validate() {
                assert(m_root->color == rb_tree_color::black);
                validate_node(m_root);
            }
            void validate(node_type* n)  {
                // - we're child of our parent.
                assert(n->parent == &ms_sentinel || n->parent->left == n || n->parent->right == n);

                // - both children of rb_tree_color::red node_type are rb_tree_color::black
                if (n->color == rb_tree_color::red) {
                        assert(n->left->color == rb_tree_color::black);
                        assert(n->right->color == rb_tree_color::black);
                }
                if (n->left != &ms_sentinel) validate(n->left);
                if (n->right != &ms_sentinel) validate(n->right);
            }
            void rotate_left(node_type* n) {
                // Right child's left child becomes n's right child.
                node_type* rightChild = n->right;
                n->right = rightChild->left;

                if (n->right != &ms_sentinel) n->right->parent = n;

                // n's right child replaces n
                rightChild->parent = n->parent;

                if (n->parent == &ms_sentinel) {
                        m_root = rightChild;
                } else {
                    if (n == n->parent->left)
                        n->parent->left = rightChild;
                    else
                        n->parent->right = rightChild;
                }

                rightChild->left = n;
                n->parent = rightChild;
            }
            void rotate_right(node_type* n) {

                node_type* leftChild(n->left);
                n->left = leftChild->right;
                
                if (n->left != &ms_sentinel) n->left->parent = n;

                leftChild->parent = n->parent;

                if (n->parent == &ms_sentinel) {
                    m_root = leftChild;
                } else {
                    // Substitute us in the parent list with left child.
                    if (n == n->parent->left)
                        n->parent->left = leftChild;
                    else
                        n->parent->right = leftChild;
                }
                leftChild->right = n;
                n->parent = leftChild;
            }
            void free_node(node_type* n, bool recursive) {
                if (recursive) {
                    if (n->left != &ms_sentinel) free_node(n->left, true);
                    if (n->right != &ms_sentinel) free_node(n->right, true);
                }
                if (n != &ms_sentinel) {
                    delete n;
                }
            }


            base_rb_tree(const base_rb_tree&) = delete;
            base_rb_tree& operator=(const base_rb_tree&) = delete; 
        public:
            void traverse_node(node_type* n, TravFunc func, int depth) {
                int left(-1);
                if (n->parent != &ms_sentinel) {
                    left = n->parent->left == n;
                }
                func(n, left, depth);

                if (n->left != &ms_sentinel)
                    traverse_node(n->left, func, depth + 1);
                if (n->right != &ms_sentinel)
                    traverse_node(n->right, func, depth + 1);
            }

            void traverse(TravFunc func) {
                int depth(0);
                traverse_node(m_root, func, depth);
            } 
        protected:
            inline void rebalance(node_type* new_node) {
                assert(new_node->color == rb_tree_color::red);

                node_type* iter(new_node);
                
                while (iter->parent->color == rb_tree_color::red) {

                    node_type* grandparent(iter->parent->parent);

                    if (iter->parent == grandparent->left) {
                        node_type* uncle = grandparent->right;

                        if (uncle->color == rb_tree_color::red) {
                            iter->parent->color = rb_tree_color::black;
                            uncle->color = rb_tree_color::black;
                            grandparent->color = rb_tree_color::red;
                            iter = grandparent;
                        } else  {
                            
                            if (iter == iter->parent->right) {
                                iter = iter->parent;
                                rotate_left(iter);
                            }

                            grandparent = iter->parent->parent;
                            iter->parent->color = rb_tree_color::black;
                            grandparent->color = rb_tree_color::red;
                            rotate_right(grandparent);
                        }
                    } else {
                        node_type* uncle = grandparent->left;
                        if (uncle->color == rb_tree_color::red) {
                            grandparent->color = rb_tree_color::red;
                            iter->parent->color = rb_tree_color::black;
                            uncle->color = rb_tree_color::black;
                            iter = grandparent;
                        } else {
                            if (iter == iter->parent->left) {
                                iter = iter->parent;
                                rotate_right(iter);
                            }
                            grandparent = iter->parent->parent;
                            iter->parent->color = rb_tree_color::black;
                            grandparent->color = rb_tree_color::red;
                            rotate_left(grandparent);
                        }
                    }
                }
                m_root->color = rb_tree_color::black;
            }

            void rebalance_after_erase(node_type* n) {
                node_type* iter(n);

                while (iter != m_root && iter->color == rb_tree_color::black) {

                    if (iter == iter->parent->left) {
                        node_type* sibling = iter->parent->right;
                        if (sibling->color == rb_tree_color::red)  {
                            sibling->color = rb_tree_color::black;
                            iter->parent->color = rb_tree_color::red;
                            rotate_left(iter->parent);
                            sibling = iter->parent->right;
                        }

                        if (sibling->left->color == rb_tree_color::black && 
                           sibling->right->color == rb_tree_color::black) {
                            sibling->color = rb_tree_color::red;
                            iter = iter->parent;
                        } else {

                            if (sibling->right->color == rb_tree_color::black) {
                                sibling->left->color = rb_tree_color::black;
                                sibling->color = rb_tree_color::red;
                                rotate_right(sibling);
                                sibling = iter->parent->right;
                            }

                            sibling->color = iter->parent->color;
                            iter->parent->color = rb_tree_color::black;
                            sibling->right->color = rb_tree_color::black;
                            rotate_left(iter->parent);
                            iter = m_root;
                        }
                    } else  {  // iter == right child

                        node_type* sibling = iter->parent->left;
                        if (sibling->color == rb_tree_color::red) {
                            sibling->color = rb_tree_color::black;
                            iter->parent->color = rb_tree_color::red;
                            rotate_right(iter->parent);
                            sibling = iter->parent->left;
                        }
                        if (sibling->left->color == rb_tree_color::black && 
                            sibling->right->color == rb_tree_color::black) {
                            sibling->color = rb_tree_color::red;
                            iter = iter->parent;
                        } else {

                            if (sibling->left->color == rb_tree_color::black)  {
                                sibling->right->color = rb_tree_color::black;
                                sibling->color = rb_tree_color::red;
                                rotate_left(sibling);
                                sibling = iter->parent->left;
                            }

                            sibling->color = iter->parent->color;
                            iter->parent->color = rb_tree_color::black;
                            sibling->left->color = rb_tree_color::black;
                            rotate_right(iter->parent);
                            iter = m_root;
                        }
                    }
                }
                iter->color = rb_tree_color::black;
            }
        private:
            node_type*              m_root;
            size_type               m_size;
            allocator_type  		m_allocator;
            static node_type        ms_sentinel;
        };

        template<class TTreeTraits, class TAllocator>
        typename base_rb_tree<TTreeTraits, TAllocator>::node_type 
            base_rb_tree<TTreeTraits, TAllocator>::ms_sentinel( 
                &base_rb_tree<TTreeTraits, TAllocator>::ms_sentinel);

        

        template<typename TKey, class TAllocator>
        using rb_tree = base_rb_tree<internal::rb_tree_traits<TKey>, TAllocator>;
    }
}

#endif