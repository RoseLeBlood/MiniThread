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
#ifndef _MINILIB_4dcc23da_d2cf_41da_9e29_571d9f17efbe_H_
#define _MINILIB_4dcc23da_d2cf_41da_9e29_571d9f17efbe_H_

#include "../mn_typetraits.hpp"
#include "../mn_algorithm.hpp"
#include "../mn_allocator.hpp"

namespace mn {
	namespace container {

        template<typename T, class TAllocator> 
        struct basic_vector_storage {
            using allocator_type = TAllocator;
            using self_type = basic_vector_storage<T, TAllocator>;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using size_type = mn::size_t;

            explicit basic_vector_storage(const allocator_type& allocator)
        	    : m_begin(0), m_end(0), m_capacityEnd(0), m_allocator(allocator) { }

            void reallocate(size_type newCapacity, size_type oldSize) {
                pointer newBegin = static_cast<pointer>(m_allocator.alloc(newCapacity * sizeof(T)));

                const size_type newSize = oldSize < newCapacity ? oldSize : newCapacity;
                // Copy old data if needed.
                if (m_begin) {
                    mn::copy_construct_n(m_begin, newSize, newBegin);
                    destroy(m_begin, oldSize);
                }
                m_begin = newBegin;
                m_end = m_begin + newSize;
                m_capacityEnd = m_begin + newCapacity;
                assert(invariant());
            }
            void reallocate_discard_old(size_type newCapacity) {

                assert(newCapacity > size_type(m_capacityEnd - m_begin));
                pointer newBegin = static_cast<pointer>(m_allocator.alloc(newCapacity * sizeof(T)));
                const size_type currSize((size_type)(m_end - m_begin));

                if (m_begin) destroy(m_begin, currSize);

                m_begin = newBegin;
                m_end = m_begin + currSize;
                m_capacityEnd = m_begin + newCapacity;
                assert(invariant());
            }

            void destroy(pointer ptr, size_type n) {
                mn::destruct_n(ptr, n);
                m_allocator.free(ptr);
            }
            void reset()  {
                if (m_begin) m_allocator.free(m_begin);
                m_begin = m_end = 0;
                m_capacityEnd = 0;
            }

            bool invariant() const {
                return m_end >= m_begin;
            }

            pointer              m_begin;
            pointer              m_end;
            pointer              m_capacityEnd;
            allocator_type       m_allocator;
        };

        template<typename T, class TAllocator, class TStorage = basic_vector_storage<T, TAllocator> >
        class basic_vector : private TStorage {
        public:
            using iterator_category = random_access_iterator_tag ;
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using difference_type = ptrdiff_t;

            using iterator = pointer;
            using const_iterator = const pointer;

            using allocator_type = TAllocator;
            using size_type = mn::size_t;

            static const size_type  npos = size_type(-1);
            static const size_type  kInitialCapacity = 16;
    
            explicit basic_vector(const allocator_type& allocator = allocator_type())
                : TStorage(allocator) { }

            explicit basic_vector(size_type initialSize, const allocator_type& allocator = allocator_type())
                : TStorage(allocator) { resize(initialSize); }

            basic_vector(const pointer first, const pointer last, const allocator_type& allocator = allocator_type())
                : TStorage(allocator) { assign(first, last); }

            basic_vector(const basic_vector& rhs, const allocator_type& allocator = allocator_type())
                : TStorage(allocator) {
                if(rhs.size() == 0) return;

                this->reallocate_discard_old(rhs.capacity());
                mn::copy_construct_n(rhs.m_begin, rhs.size(), m_begin);

                m_end = m_begin + rhs.size();
                assert(invariant());
            }

            ~basic_vector() {
                if (TStorage::m_begin != 0) TStorage::destroy(TStorage::m_begin, size());
            }

            
        
            void copy(const basic_vector& rhs) {
                const size_type newSize = rhs.size();

                if (newSize > capacity())
                     reallocate_discard_old(rhs.capacity());
                
                mn::copy_construct_n(rhs.m_begin, newSize, m_begin);
                m_end = m_begin + newSize;
                
                assert(invariant());
            }

            iterator begin()                        { return m_begin; }
            const_iterator cbegin() const           { return m_begin; }
            iterator end()                          { return m_end; }
            const_iterator cend()  const            { return m_end; }

            size_type size() const                  { return size_type(m_end - m_begin); }
            bool empty() const                      { return m_begin == m_end; }

            size_type capacity()                    { return size_type(m_capacityEnd - m_begin); }

            pointer data()                          { return empty() ? 0 : m_begin; }
            const pointer cdata()                   { return empty() ? 0 : m_begin; }

            reference front()                       { assert(!empty()); return *begin(); }
            const reference cfront()                { assert(!empty());  return *begin(); }
            reference back()                        { assert(!empty()); return *(end() - 1);  }
            const reference cback()                 { assert(!empty()); return *(end() - 1); }

            reference at(size_type i)                { assert(i < size()); return m_begin[i]; }
            const reference const_at(size_type i)    { assert(i < size()); return m_begin[i]; }
            
            void push_back(const reference v) {
                if (m_end >= m_capacityEnd) grow(); 
                mn::copy_construct(m_end++, v);
            }
            void push_back() {
                if (m_end == m_capacityEnd) grow();
                mn::construct(m_end); ++m_end;
            }
            void pop_back() {
                assert(!empty()); --m_end;
                mn::destruct(m_end);
            }

            void assign(const pointer first, const pointer last) {
                assert(!validate_iterator(first));
                assert(!validate_iterator(last));

                const size_type count = size_type(last - first);
                assert(count > 0);
                clear();

                if (m_begin + count > m_capacityEnd)
                    reallocate_discard_old(compute_new_capacity(count));

                mn::copy_n(first, count, m_begin);
                m_end = m_begin + count;
                
                assert(invariant());
            }

            void insert(size_type index, size_type n, const reference val) {
                assert(invariant());

                const size_type indexEnd = index + n;
                const size_type prevSize = size();

                if (m_end + n > m_capacityEnd) {
                    reallocate(compute_new_capacity(prevSize + n), prevSize);
                }

                if (indexEnd > prevSize) {
                    const size_type numCopy         = prevSize - index;
                    const size_type numAppend       = indexEnd - prevSize;

                    assert(numCopy >= 0 && numAppend >= 0);
                    assert(numAppend + numCopy == n);

                    iterator itOut = m_begin + prevSize;
                    for (size_type i = 0; i < numAppend; ++i, ++itOut) {
                        mn::copy_construct(itOut, val);
                    }

                    mn::copy_construct_n(m_begin + index, numCopy, itOut);

                    for (size_type i = 0; i < numCopy; ++i) {
                            m_begin[index + i] = val;
                    }
                } else {
                    mn::copy_construct_n(m_end - n, n, m_end);
                    iterator insertPos = m_begin + index;
                    mn::move_n(insertPos, prevSize - indexEnd, insertPos + n);
                    for (size_type i = 0; i < n; ++i) {
                        insertPos[i] = val;
                    }
                }
                m_end += n; 
            }

            void insert(iterator it, size_type n, const reference val) {
                assert(validate_iterator(it));
                assert(invariant());
                insert(size_type(it - m_begin), n, val);
            }

            iterator insert(iterator it, const reference val) {
                assert(validate_iterator(it));
                assert(invariant());

                const size_type index = (size_type)(it - m_begin);
                if (m_end == m_capacityEnd) {
                    grow();
                    it = m_begin + index;
                } else {
                    mn::construct(m_end);
                }

                if (m_end > it) {
                    if(!has_trivial_copy<T>::value) {
                        const size_type prevSize = size();
                        assert(index <= prevSize);
                        const size_type toMove = prevSize - index;

                        mn::internal::move_n(it, toMove, it + 1, int_to_type<has_trivial_copy<T>::value>());
                    } else {
                        assert(it < m_end);
                        const size_t n = reinterpret_cast<uintptr_t>(m_end) - reinterpret_cast<uintptr_t>(it);
                        memmove(it + 1, it, n);
                    }
                }
                *it = val;
                ++m_end;
                assert(invariant());
                
                return it;
            }

            iterator erase(iterator it) {
                assert(validate_iterator(it));
                assert(it != end());
                assert(invariant());

                // Move everything down, overwriting *it
                if (it + 1 < m_end) {
                    move_down_1(it, int_to_type<has_trivial_copy<T>::value>());
                }
                --m_end;
                mn::destruct(m_end);
                return it;
            }
            iterator erase(iterator first, iterator last) {
                assert(validate_iterator(first));
                assert(validate_iterator(last));
                assert(invariant());

                if (last <= first) return end();
                
                const size_type indexFirst = size_type(first - m_begin);
                const size_type toRemove = size_type(last - first);

                if (toRemove > 0) {
                    move_down(last, first, int_to_type<has_trivial_copy<T>::value>());
                    shrink(size() - toRemove);
                }
                return m_begin + indexFirst;
            }

            inline void erase_undestroy(iterator it) {
                assert(validate_iterator(it));
                assert(it != end());
                assert(invariant());

                const iterator itNewEnd = end() - 1;
                if (it != itNewEnd) *it = *itNewEnd;
                pop_back();
            }

            void resize(size_type n) {
                if (n > size()) insert(m_end, n - size(), value_type());
                else shrink(n);
            }
            void reserve(size_type n) {
                if (n > capacity()) reallocate(n, size());
            }
            void clear() {
                shrink(0);
                assert(invariant());
            }
            
            void reset() {
                TStorage::reset();
                assert(invariant());
            }
            void set_capacity(size_type newCapacity) {
                reallocate(newCapacity, size());
            }

            size_type index_of(const reference item, size_type index = 0) const {
                assert(index >= 0 && index < size());
                size_type _pos = npos;

                for ( ; index < size(); ++index) {
                    if (m_begin[index] == item) { 
                        _pos = index; break; 
                    }
                }
                return _pos;
            }
            iterator find(const reference item) {
                iterator itEnd = end();

                for (iterator it = begin(); it != itEnd; ++it)
                    if (*it == item) { itEnd = it; break; }
                return itEnd;
            }

            const allocator_type& get_allocator() const { 
                return m_allocator; 
            }

            void set_allocator(const allocator_type& allocator) {
                if(empty()) m_allocator = allocator;
            }

            bool validate_iterator(const_iterator it) const {
                return it >= begin() && it <= end();
            }

            basic_vector& operator=(const basic_vector& rhs) {
                copy(rhs);              
                return *this;
            }
            reference operator[](size_type i) {
                return at(i);
            }

            const reference operator[](size_type i) const {
                return at(i);
            }
        private:
            size_type compute_new_capacity(size_type newMinCapacity) const {
                const size_type c = capacity();
                return (newMinCapacity > c * 2 ? newMinCapacity : (c == 0 ? kInitialCapacity : c * 2));
            }

            inline void grow() {
                assert(m_end == m_capacityEnd);
                const size_type c = capacity();
                reallocate((m_capacityEnd == 0 ? kInitialCapacity : c * 2), c);
            }

            inline void shrink(size_type newSize) {
                assert(newSize <= size());
                const size_type toShrink = size() - newSize;
                mn::destruct_n(m_begin + newSize, toShrink);
                m_end = m_begin + newSize;
            }
            inline void move_down_1(iterator it, int_to_type<true> itt) {
                internal::move(it + 1, m_end, it, itt);
            }
            inline void move_down_1(iterator it, int_to_type<false> itt) {
                internal::copy(it + 1, m_end, it, itt);
            }

            inline void move_down(iterator it_start, iterator it_result, int_to_type<true> itt) {
                assert(it_start > it_result);
                internal::move(it_start, m_end, it_result, itt);
            }
            inline void move_down(iterator it_start, iterator it_result, int_to_type<false> itt) {
                assert(it_start > it_result);
                internal::copy(it_start, m_end, it_result, itt);
            }
        private:
            using TStorage::m_begin;
            using TStorage::m_end;
            using TStorage::m_capacityEnd;
            using TStorage::m_allocator;
            using TStorage::invariant;
            using TStorage::reallocate;
        };

        template<typename T >
        using vector = basic_vector<T, mn::memory::default_allocator_t>;


        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator + (const basic_vector<T, TAllocator, TStorage>& a, 
                                                                 const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++) 
                c.push_back(a[i] + b[i]);
            return c;    
        }
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator + (const basic_vector<T, TAllocator, TStorage>& a, 
                                                        const T& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++)
                c.push_back(a[i] + b);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator + (const T& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : b.size());

            for(int i = 0; i < size; i++)
                c.push_back(a + b[i]);
            return c;    
        }

        //sub ------
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator - (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++)
                c.push_back(a[i] - b[i]);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator - (const basic_vector<T, TAllocator, TStorage>& a) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++)
                c.push_back(-a[i]);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator - (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const T& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());
        
            for(int i = 0; i < size; i++)
                c.push_back(a[i] - b);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator - (const T& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : b.size());

            for(int i = 0; i < size; i++)
                c.push_back(a - b[i]);
            return c;    
        }

        //mul ----
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator * (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++)
                c.push_back(a[i] * b[i]);
            return c;    
        }
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator * (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const T& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());
        
            for(int i = 0; i < size; i++)
                c.push_back(a[i] * b);
            return c;    
        }
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator * (const T& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : b.size());

            for(int i = 0; i < size; i++)
                c.push_back(a * b[i]);
            return c;    
        }

        // div -----
        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator / (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());
        
            for(int i = 0; i < size; i++)
                c.push_back(a[i] / b[i]);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator / (const basic_vector<T, TAllocator, TStorage>& a, 
                                                    const T& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : a.size());

            for(int i = 0; i < size; i++)
                c.push_back(a[i] / b);
            return c;    
        }

        template<typename T, int calc = 0, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> operator / (const T& a, 
                                                    const basic_vector<T, TAllocator, TStorage>& b) {
            basic_vector<T, TAllocator, TStorage> c = basic_vector<T, TAllocator, TStorage>();
            int size = (calc != 0 ? calc : b.size());
        
            for(int i = 0; i < size; i++)
                c.push_back(a / b[i]);
            return c;    
        }
        
        // scale ----
        template<typename T, class TAllocator =  mn::memory::default_allocator_t, class TStorage = basic_vector_storage<T, TAllocator> >
        inline basic_vector<T, TAllocator, TStorage> scale(const basic_vector<T, TAllocator, TStorage>& v,
                                                    const T s) {
            return v * s;        
        }
    }
}
#endif 