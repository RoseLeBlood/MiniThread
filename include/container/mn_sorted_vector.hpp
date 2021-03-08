/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2021 Amber-Sophia Schroeck
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

#ifndef _MINLIB_3b4e9cd5_8d8d_4268_bbff_2f66acdac76e_H_
#define _MINLIB_3b4e9cd5_8d8d_4268_bbff_2f66acdac76e_H_


#include "../mn_functional.hpp"
#include "mn_pair.hpp"
#include "../utils/mn_sort.hpp"
#include "mn_vector.hpp"

namespace mn {
    namespace container {
        namespace internal {

            /**
             * @brief Internal helper for sorting 
             * 
             * @tparam TPair   The pair
             * @tparam TFunctor The sort function
             */
            template<class TPair, class TFunctor>
            struct compare_func {
                bool operator()(const TPair& lhs, const TPair& rhs) const {
                    return TFunctor()(lhs.first, rhs.first);
                }
                bool operator()(const TPair& lhs, const typename TPair::first_type& rhs) const {
                    return TFunctor()(lhs.first, rhs);
                }
                bool operator()(const typename TPair::first_type& lhs, const TPair& rhs) const {
                    return TFunctor()(lhs, rhs.first);
                }
            };
        }

        template<typename TKey, typename TValue, class TAllocator, 
        class TCompare = mn::less<TKey>, class TStorage = basic_vector_storage< basic_pair<TKey, TValue>, TAllocator> >
	    class basic_sorted_vector : private basic_vector<basic_pair<TKey, TValue>, TAllocator, TStorage > {  
            using base_type = basic_vector<basic_pair<TKey, TValue>, TAllocator, TStorage>;
        public:
            using key_type = TKey;
            using mapped_type = TValue;
            using size_type = typename base_type::size_type;
            using value_type =  typename base_type::value_type;
            using pointer = typename base_type::pointer;
            using reference = typename base_type::reference;
            

            using iterator =  typename base_type::iterator;
            using const_iterator = typename base_type::const_iterator;
            using allocator_type = typename base_type::allocator_type;

            using compare_type = internal::compare_func<value_type, TCompare>;
            using pair_type = basic_pair<iterator, bool>;

            explicit basic_sorted_vector(const allocator_type& allocator = allocator_type())
        	    : base_type(allocator) { }
            
            template <class InputIterator> 
            basic_sorted_vector(InputIterator first, InputIterator last, 
                                const allocator_type& allocator = allocator_type())
        	                    : base_type(first, last, allocator) {
         	    mn::quick_sort(begin(), end(), m_compare);
         	    assert(invariant());
            }

            basic_sorted_vector(const basic_sorted_vector&) = delete;
            basic_sorted_vector& operator=(const basic_sorted_vector&) = delete;

            using base_type::begin;
            using base_type::end;
            using base_type::size;
            using base_type::empty;
            using base_type::capacity;
            using base_type::clear;
            using base_type::get_allocator;
            using base_type::set_allocator;

            pair_type insert(const value_type& val) {
                assert(invariant());
                bool found(true);

                iterator it = lower_bound(val.first);
                assert(it == end() || !m_compare(*it, val));

                if (it == end() || m_compare(*it, val)){
                    it = base_type::insert(it, val);
                    found = false;
                }
                assert(invariant());
                return pair_type(it, !found);
            }
            inline pair_type insert(const key_type& k, const mapped_type& v) {
                return insert(value_type(k, v));
            }

            iterator find(const key_type& k) {
                assert(invariant());
                iterator i(lower_bound(k));
                if (i != end() && m_compare(k, *i)) {
                    i = end();
                }
                return i;
            }
            const_iterator find(const key_type& k) const {       
                assert(invariant());
                const_iterator i(lower_bound(k));
                if (i != end() && m_compare(k, *i)) i = end();
                return i;
            }

            inline iterator erase(iterator it) {
                assert(invariant());
                return base_type::erase(it);
            }
            size_type erase(const key_type& k) {
                iterator i(find(k));
                if (i == end()) return 0;
                erase(i);
                assert(invariant());
                return 1;
            }
            iterator lower_bound(const key_type& k) {
                return mn::lower_bound(begin(), end(), k, m_compare);
            }
            const_iterator lower_bound(const key_type& k) const {
                return mn::lower_bound(begin(), end(), k, m_compare);
            }
            iterator upper_bound(const key_type& k) {
                return mn::upper_bound(begin(), end(), k, m_compare);
            }
            const_iterator upper_bound(const key_type& k) const {
                return mn::upper_bound(begin(), end(), k, m_compare);
            }
        private:
            bool invariant() const {
                const_iterator first = begin();
                const_iterator last = end();

                assert(last >= first);
                if (first != last)  {
                    const_iterator next = first;
                    if (++next != last) {
                        assert(m_compare(*first, *next));
                        first = next;
                    }
                }
                return true;
            }
        private:
		    compare_type   m_compare;
        };


        template<typename TKey, typename TValue, class TCompare = mn::less<TKey> >
        using sorted_vector = basic_sorted_vector<TKey,TValue, mn::memory::default_allocator_t, TCompare>;

    }
}

#endif