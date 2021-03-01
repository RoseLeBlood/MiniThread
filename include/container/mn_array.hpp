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
#ifndef _MINLIB_f4421533_40ce_492a_a99b_8a7ca77e49bd_H_
#define _MINLIB_f4421533_40ce_492a_a99b_8a7ca77e49bd_H_

#include "mn_algorithm.hpp"

namespace mn {
    namespace container {

        template <typename T, size_t N>
        class basic_array  {
        public:
            using value_type = T;
            using const_value_type = const T;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using reference = value_type&;
            using const_reference = const T&;
            using size_type = size_t;
            using self_type = basic_array<T, N>;

            using iterator = pointer;
            using const_iterator = const_pointer;
              
            basic_array() { }
            basic_array(const_value_type a[N])    { mn::copy_n(&a[0], N, m_data); }
            basic_array(const self_type& other)   { mn::copy(other.begin(), other.end(), m_buffer); }

            iterator        begin()         { return m_buffer; }
            const_iterator  cbegin() const  { return m_buffer; }
            
            iterator        end()           { return m_buffer[N-1]; }
            const_iterator  cend() const    { return m_buffer[N-1]; }
            
            reference       front()         { return m_buffer[0]; }
            const_reference cfront() const  { return m_buffer[0]; }
            reference       back()          { return m_buffer[N-1]; }
            const_reference cback() const   { return m_buffer[N-1]; }

            size_type size() { return N; }

            reference operator[](size_type i) 
                { assert(i < size()); return m_buffer[i]; }   

            const_reference operator[](size_type i) const 
                { assert(i < size()); return m_buffer[i]; }
        

            template <typename U>
            self_type& operator = (const basic_array<U, N>& rhs) {
                mn::copy(rhs.begin(), rhs.end(), begin());
                return *this;
            }
            template <typename U, size_t N2>
            self_type& operator = (const basic_array<U, N2>& rhs) {
                size_type size = mn::min<size_t>(size(), rhs.size());
                mn::copy_n(rhs.begin(), size, begin()); 

                return *this;
            }

            void assign (const T& value) { 
                    fill ( value ); }    

            void fill   (const T& value) {
                mn::fill_n<T>(begin(), size(), value); }
      
        
            iterator data() { return &m_buffer[0]; }
            const_iterator data() { return &m_buffer[0]; }
        private:
            value_type m_buffer[N];
        };


        template < typename T, size_t N >
        inline bool operator == (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] != b[i]) { ret = false; break; }
            return ret;
        }
        template < typename T, size_t N >
        inline bool operator != (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] == b[i]) { ret = false; break; }
            return ret;
        }
        template < typename T, size_t N >
        inline bool operator <= (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] > b[i]) { ret = false; break; }
            return ret;
        }
        template < typename T, size_t N >
        inline bool operator >= (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] < b[i]) { ret = false; break; }
            return ret;
        }  
        template < typename T, size_t N >
        inline bool operator < (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] >= b[i]) { ret = false; break; }
            return ret;
        }
        template < typename T, size_t N >
        inline bool operator > (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;
            bool ret = true;
        
            for (int i =0; i < a.size(); i++) 
                if(a[i] <= b[i]) { ret = false; break; }
            return ret;
        }

        /**
         * @brief Array type 
         * 
         * @tparam T The holding type of value
         * @tparam N The Number of Elements
         */
        template <typename T, size_t N> 
        using array = basic_array<T, N>;
        
    } // namespace container
    
} // namespace mn


#endif