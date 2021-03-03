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
#ifndef _d778eba9_4f4c_412d_90f7_10d7b8f99c49_H_
#define _d778eba9_4f4c_412d_90f7_10d7b8f99c49_H_


#include "mn_string_storage.hpp"

namespace mn {
    namespace strings {

        template<typename E, class TAllocator, 
                 typename TStorage = string_storage<E, TAllocator> >
        class basic_string : private TStorage {
        public:
            using value_type = typename TStorage::value_type;
            using size_type = typename TStorage::size_type;
            using const_iterator = typename TStorage::const_iterator ;
            using allocator_type = typename TStorage::allocator_type;
            using self_type = basic_string<E, TAllocator, TStorage >;

            //For find
            static const size_type npos = size_type(-1);

            explicit basic_string(const allocator_type& allocator = allocator_type())
                : TStorage(allocator) { }

            explicit basic_string(const value_type* str, const allocator_type& allocator = allocator_type())
                : TStorage(str, allocator) {  }
            
            basic_string(size_type len, const allocator_type& allocator = allocator_type())
                : TStorage(len, allocator) { }

            basic_string(const value_type* str, size_type len, const allocator_type& allocator = allocator_type())
                : TStorage(str, len, allocator) {  }

            basic_string(const basic_string& str, const allocator_type& allocator = allocator_type())
                : TStorage(str, allocator) { }

            ~basic_string() { }


            size_type capacity() const { 
                return TStorage::capacity(); 
            }

            basic_string& operator << (const value_type* str) {
                append(str, strlen(str));
                return *this;
            }
            basic_string& operator << (const value_type ch) {
                append(&ch, 1);
                return *this;
            }
            basic_string& operator <<(const basic_string& str) {
                append(str.c_str(), str.length());
                return *this;
            }
            
            value_type operator[](size_type i) const {
                assert(i < length());
                assert(invariant());
                return c_str()[i];
            }
            
            basic_string& operator=(const basic_string& rhs) {
                assert(rhs.invariant());
                if (this != &rhs)  {
                        TStorage::operator=((TStorage&)rhs);
                }
                assert(invariant());
                return *this;
            }

            basic_string& operator=(const value_type* str) {
                return assign(str);
            }
            basic_string& assign(const value_type* str, size_type len) {
                TStorage::assign(str, len);
                assert(invariant());
                return *this;
            }

            basic_string& assign(const value_type* str) {
                return assign(str, strlen(str));
            }

            basic_string substr(size_type begin, size_type end) const {
                assert(end >= begin && end <= length() && begin >= 0);
                return basic_string(c_str() + begin, end - begin);
            }

            basic_string substr(size_type begin) const {
                return substr(begin, length());
            }

            void append(const value_type* str, size_type len) {
                if( !str || len == 0 || *str == 0 ) return;
                TStorage::append(str, len);
            }

            void append(const basic_string& str) {
                append(str.c_str(), str.length());
            }

            void append(const value_type* str) {
                append(str, strlen(str));
            }

            void append(const value_type ch) {
                append(&ch, 1);
            }

            basic_string& operator+=(const basic_string& rhs) {
                append(rhs);
                return *this;
            }

            int compare(const value_type* str) const {
                const size_type thisLen = length();
                const size_type strLen = strlen(str);
                if (thisLen < strLen)
                return -1;
                if (thisLen > strLen)
                return 1;

                return strcmp(c_str(), str, thisLen);
            }

            int compare(const basic_string& rhs) const {
                const size_type thisLen = length();
                const size_type rhsLen = rhs.length();
                if (thisLen < rhsLen)
                        return -1;
                if (thisLen > rhsLen)
                        return 1;

                return strcmp(c_str(), rhs.c_str(), thisLen);
            }

            const value_type* c_str() const {
                assert(invariant());
                return TStorage::c_str();
            }

            const_iterator begin() const { 
                assert(invariant()); return c_str(); 
            }
            const_iterator end() const { 
                assert(invariant()); return c_str() + length(); 
            }

            size_type length() const {
                return TStorage::length();
            }
            value_type* data() {
                return TStorage::get_data();
            }

            bool empty() const  { 
                return length() == 0; 
            }

            const allocator_type& get_allocator() const  { 
                return TStorage::get_allocator; 
            }

            value_type* reserve(size_type capacity_hint) {
                return TStorage::reserve(capacity_hint);
            }
            void clear() {
                TStorage::clear();
            }
            void resize(size_type size) {
                TStorage::resize(size);
            }

            void make_lower() {
                const size_type len = length();
                TStorage::make_unique(len);
                static const int chDelta = 'a' - 'A';
                E* data = TStorage::get_data();
                for (size_type i = 0; i < len; ++i) {
                    if (data[i] < 'a') data[i] += chDelta;             
                }
            }

            void make_upper() {
                const size_type len = length();
                TStorage::make_unique(len);
                static const int chDelta = 'a' - 'A';
                E* data = TStorage::get_data();
                for (size_type i = 0; i < len; ++i) {
                    if (data[i] > 'Z') data[i] -= chDelta;
                }
            }

            size_type find_index_of(const value_type ch) const {
                size_type retIndex(basic_string::npos);
                const E* ptr = c_str();
                size_type currentIndex(0);
                while (*ptr != value_type(0))
                {
                        if (*ptr == ch)
                        {
                                retIndex = currentIndex;
                                break;
                        }
                        ++ptr;
                        ++currentIndex;
                }
                return retIndex;
            }

            size_type find_index_of_last(const value_type ch) const {
                size_type retIndex(basic_string::npos);
                const value_type* ptr = c_str();
                size_type currentIndex(0);
                while (*ptr != value_type(0))
                {
                        if (*ptr == ch)
                                retIndex = currentIndex;
                        ++ptr;
                        ++currentIndex;
                }
                return retIndex;
            }

            size_type find(const value_type* needle) const {       
                const value_type* s(c_str());    
                size_type si(0);
                while(*s)
                {
                    const value_type* n = needle;
                    if( *s == *n ) //first character matches
                    {
                        const value_type* x = s; 
                        size_type match = 0;
                        while(*x && *n) 
                        {
                            if( *n == *x )
                                ++match;
                            ++n;
                            ++x;
                        }
                        if( match == strlen(needle) )
                            return si;
                    }
                    ++s;
                    ++si;
                }
                return basic_string::npos;
            }

            size_type rfind(const value_type* needle) const {   
                const value_type* s(c_str() + length());
                size_type si(length()+1); 

                while(--si >= 0) 
                {
                        if( *s-- == *needle ) 
                        {
                                const value_type* x = c_str() + si; 
                                const value_type* n = needle;
                                size_type match = 0;
                                while(*x && *n) 
                                {
                                        if( *n == *x )
                                                ++match;
                                        ++n;
                                        ++x;
                                }
                                if( match == strlen(needle) )
                                        return si;
                        }
                }
                return basic_string::npos;
            }
        private:
            bool invariant() const  {
                return TStorage::invariant();
            }
        };

        template<typename E, class TStorage, class TAllocator>
        bool operator==(const basic_string<E, TStorage, TAllocator>& lhs, const basic_string<E, TStorage, TAllocator>& rhs) {
                return lhs.compare(rhs) == 0;
        }

        template<typename E, class TStorage, class TAllocator>
        bool operator!=(const basic_string<E, TStorage, TAllocator>& lhs,  const basic_string<E, TStorage, TAllocator>& rhs) {
                return !(lhs == rhs);
        }

        template<typename E, class TStorage, class TAllocator>
        bool operator<(const basic_string<E, TStorage, TAllocator>& lhs,  const basic_string<E, TStorage, TAllocator>& rhs) {
                return lhs.compare(rhs) < 0;
        }

        template<typename E, class TStorage, class TAllocator>
        bool operator>(const basic_string<E, TStorage, TAllocator>& lhs,  const basic_string<E, TStorage, TAllocator>& rhs) {
                return lhs.compare(rhs) > 0;
        }
    }
}

#endif