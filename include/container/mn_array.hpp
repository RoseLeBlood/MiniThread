/** 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
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

#ifndef MINLIB_STL_ARRAY_H_
#define MINLIB_STL_ARRAY_H_

#include <stddef.h>
#include "../mn_algorithm.hpp"

namespace mn {
    namespace container {
        /**
         * @brief A standard container for storing a fixed size sequence of elements.
         * @author Amber-Sophia Schröck
         * 
         * @tparam T Type of element. Required to be a complete type.
         * @tparam N Number of elements.
         */
        template <typename T, size_t N>
        class basic_array  {
        public:
            /** @brief Type for the iterator */
            using iterator = T*;
            /** @brief Type for the const iterator */
            using const_iterator = const T*; 
            /** @brief Type for the reference */
            using reference = T&;
            /** @brief Type for the const reference */
            using const_reference = const T&;
            /** @brief Type for the size */
            using size_type = size_t;
            
            /**
             * @brief Construct a new array object
             */
            basic_array() { }
            /**
             * @brief Construct a new array object, from a given array
             * @param a The given array
             */
            basic_array(const T a[N]) { mn::copy_n(&a[0], N, m_buffer); }
            
            /**
             * @brief Get the first element of the array
             * @return The first element of the array 
             */
            iterator        begin()       { return m_buffer; }
            /**
             * @brief Get the first element of the array
             * @return The first element of the array 
             */
            const_iterator  cbegin() const { return m_buffer; }
            
            /**
             * @brief Get the last element of the array
             * @return The last element of the array 
             */
            iterator        end()       { return m_buffer[N-1]; }

            /**
             * @brief Get the last element of the array
             * @return The last element of the array 
             */
            const_iterator  cend() const { return m_buffer[N-1]; }
            
            /**
             * @brief Get the first element of the array
             * @return The first element of the array 
             */
            reference       front() { return m_buffer[0]; }
            /**
             * @brief Get the first element of the array
             * @return The first element of the array 
             */
            const_reference cfront() const { return m_buffer[0]; }
            /**
             * @brief Get the last element of the array
             * @return The last element of the array 
             */
            reference       back() { return m_buffer[N-1];  }
            /**
             * @brief Get the last element of the array
             * @return The last element of the array 
             */
            const_reference cback() const { return m_buffer[N-1]; }

            /**
             * @brief Get the maximal size of elements to store in this array.
             * @return The maximal size of elements to store in this array.
             */
            size_type size() { return N; }
        
            
            
            /**
             * @brief Get a reference from the element at given position
             * 
             * @param i The position
             * @return Get the element at given position as reference.
             */
            reference operator[](size_type i) { assert(i < size()); return m_buffer[i]; }    
            /**
             * @brief Get a reference from the element at given position
             * 
             * @param i The position
             * @return Get the element at given position as reference.
             */    
            const_reference operator[](size_type i) const { assert(i < size()); return m_buffer[i]; }
            

            template <typename T2>
            basic_array<T,N>& operator = (const basic_array<T2,N>& rhs) {
                
                mn::copy(rhs.begin(),rhs.end(), begin());
                return *this;
            }

            void assign (const T& value) { fill ( value ); }    
            void fill   (const T& value) { mn::fill_n<T>(begin(), size(), value); }
        
            /**
             * @brief Get the raw data of the array
             * @return The raw data of the array 
             */
            iterator data() { return &m_buffer[0]; }
            /**
             * @brief Get the raw data of the array
             * @return The raw data of the array 
             */
            const_iterator cdata() { return &m_buffer[0]; }
                
        private:
            T m_buffer[N];
        };

        template <typename T, size_t N>
        using basic_fixed_array = basic_array<T, N> ;

            
        template < typename T, size_t N >
        inline bool operator == (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            if(a.size() != b.size()) return false;

            for (int i =0; i < a.size(); i++)
                if(a[i] != b[i]) return false;

            return true;
        }
        template < typename T, size_t N >
        inline bool operator != (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            return !(a == b);
        }
        template < typename T, size_t N >
        inline bool operator <= (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            size_t size = mn::min<size_t>(a.size(), b.size());

            for (int i =0; i < size; i++)
                if(a[i] >= b[i]) return false;

            return true;
        }
        template < typename T, size_t N >
        inline bool operator >= (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            size_t size = mn::min<size_t>(a.size(), b.size());

            for (int i =0; i < size; i++)
                if(a[i] <= b[i]) return false;

            return true;
        }  
        template < typename T, size_t N >
        inline bool operator < (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            size_t size = mn::min<size_t>(a.size(), b.size());

            for (int i =0; i < size; i++)
                if(a[i] > b[i]) return false;

            return true;
        }
        template < typename T, size_t N >
        inline bool operator > (const basic_array<T, N>& a, const basic_array<T, N>& b) {
            size_t size = mn::min<size_t>(a.size(), b.size());

            for (int i =0; i < size; i++)
                if(a[i] < b[i]) return false;

            return true;
        }

        
    }
}

#endif