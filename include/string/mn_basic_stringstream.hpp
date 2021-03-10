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
#ifndef _MINLIB_c5893a16_5b52_40be_b357_099691882e2c_H_
#define _MINLIB_c5893a16_5b52_40be_b357_099691882e2c_H_

#include "mn_basic_string.hpp"
#include "../container/mn_vector.hpp"

namespace mn {
    namespace strings {
        template< typename E, typename TAllocator > 
        class basic_stringstream {
        public:
            using value_type = E;
            using buffer_type =  mn::container::basic_vector<value_type, TAllocator>;
            using size_type =  typename buffer_type::size_type;
            using string_type = basic_string<value_type, TAllocator>;
            using const_iterator = typename buffer_type::const_iterator;
            
            explicit basic_stringstream(const value_type* inp) { 
                init(inp); 
            }
            explicit basic_stringstream(const string_type& inp){ 
                init(inp.c_str()); 
            }

            basic_stringstream() {
                init("");
            }
    
            bool good() const { 
                return m_ieBuffer.size() ? m_itCourser != m_ieBuffer.end() : false; 
            }

            bool eof() const { 
                return !good(); 
            }

            void reset(const value_type* inp) {
                init(inp);
            }

            void reset(const string_type& inp) {
                init(inp.c_str());
            }

            string_type str(){
                string_type	strr = string_type();
                
                while(next()) {
                    strr.append(m_vCurrent);
                }
                return strr;
            }

            operator bool() const { 
                return good(); 
            }

            //------------------------------------------------------
            //Output operators
            basic_stringstream& operator >> (int& x) {
                if( next() ) 
                    x = atoi((const char*)m_vCurrent.c_str());
                return *this;
            }
            basic_stringstream& operator >> (long& x) {
                if( next() ) 
                    x = atol((const char*)m_vCurrent.c_str());
                return *this;
            }
            basic_stringstream& operator >> (float& x) {
                if( next() ) 
                    x = atof((const char*)m_vCurrent.c_str());
                return *this;
            }
            basic_stringstream& operator >> (string_type& x) {
                if( next() ) 
                    x = m_vCurrent;
                return *this;
            }  
            basic_stringstream& operator >> (const value_type& x) {
                if( next() ) 
                    x = m_vCurrent.c_str();
                return *this;
            }

            basic_stringstream& operator << (string_type& x) {
                m_vCurrent.append( x ); return *this;
            }  
            basic_stringstream& operator << (const value_type& x) {
                m_vCurrent.append( x ); return *this;
            }
            basic_stringstream& operator << (const value_type* x) {
                m_vCurrent.append( x ); return *this;
            }
        private:
            void init(const value_type* inp) {
                if( !inp || !strlen(inp) ) {
                    m_itCourser = m_ieBuffer.end();
                    return;
                }
                const size_type len = strlen(inp);
                m_ieBuffer.resize(len);
                memmove(m_ieBuffer.begin(), inp, len);
                m_itCourser = m_ieBuffer.begin();
                m_vCurrent.clear();
                ltrim();
            }
            
            void ltrim() {
                while(mn::isblank(*m_itCourser) || mn::isspace(*m_itCourser)) 
                    ++m_itCourser;
            }
            
            bool next() {
                if(!m_ieBuffer.size())
                    return false;
                m_vCurrent.clear();
                for(; m_itCourser!=m_ieBuffer.end(); ++m_itCourser) 
                {
                    if( mn::isprint(*m_itCourser) )  {
                        m_vCurrent.append(*m_itCourser);   
                    } else {
                        ltrim();
                        break;
                    }
                }
                return m_vCurrent.length();
            }
            
            string_type 	m_vCurrent;
            buffer_type 	m_ieBuffer;
            const_iterator  m_itCourser;
        };    
        using stringstream =  basic_stringstream<char, mn::memory::default_allocator_t>;
    }
}

#endif