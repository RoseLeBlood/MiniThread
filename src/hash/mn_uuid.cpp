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

#include "hash/mn_uuid.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>

#define HEXPAIR2CHAR(A, B) (mn::hex2char(A) * 16 + mn::hex2char(B))

namespace mn {
    namespace hash {
        const basic_uuid basic_uuid::Empty = basic_uuid({0});

        basic_uuid::basic_uuid(array_type &bytes) 
            : m_uuidBytes(bytes) { }

        basic_uuid::basic_uuid(const basic_uuid &other) 
            : m_uuidBytes(other.m_uuidBytes) { }

        basic_uuid::basic_uuid(string_type fromString) {
            char charOne = '\0';
            char charTwo = '\0';
            bool lookingForFirstChar = true;
            unsigned nextByte = 0;

            for (const char &ch : fromString) {
                if (ch == '-') continue;

                if (nextByte >= 16 || !is_visible(ch)) {
                    make_it_zero();
                    break;
                }

                if (lookingForFirstChar) {
                    charOne = ch;
                    lookingForFirstChar = false;
                } else {
                    charTwo = ch;
                    auto byte = HEXPAIR2CHAR(charOne, charTwo);
                    m_uuidBytes[nextByte++] = byte;
                    lookingForFirstChar = true;
                }
            }
            if (nextByte < 16) {
                make_it_zero();
            }
        }

        bool basic_uuid::is_valid() {
	        return *this != basic_uuid::Empty;
        }
        typename basic_uuid::string_type basic_uuid::to_string (string_type seperator) {
            std::stringstream str;

            str << std::hex << std::setfill('0')
                << std::setw(2) << (int)m_uuidBytes[0]
                << std::setw(2) << (int)m_uuidBytes[1]
                << std::setw(2) << (int)m_uuidBytes[2]
                << std::setw(2) << (int)m_uuidBytes[3]
                << seperator
                << std::setw(2) << (int)m_uuidBytes[4]
                << std::setw(2) << (int)m_uuidBytes[5]
                << seperator
                << std::setw(2) << (int)m_uuidBytes[6]
                << std::setw(2) << (int)m_uuidBytes[7]
                << seperator
                << std::setw(2) << (int)m_uuidBytes[8]
                << std::setw(2) << (int)m_uuidBytes[9]
                << seperator
                << std::setw(2) << (int)m_uuidBytes[10]
                << std::setw(2) << (int)m_uuidBytes[11]
                << std::setw(2) << (int)m_uuidBytes[12]
                << std::setw(2) << (int)m_uuidBytes[13]
                << std::setw(2) << (int)m_uuidBytes[14]
                << std::setw(2) << (int)m_uuidBytes[15];
            
	        return str.str();
        }
        void basic_uuid::make_it_zero() {
            m_uuidBytes = array_type({0});
        }
        const typename basic_uuid::array_type& basic_uuid::get_bytes() const {
            return m_uuidBytes;
        }

        void basic_uuid::swap(basic_uuid &other) {
            array_type tmp(m_uuidBytes);

            m_uuidBytes = other.m_uuidBytes;
            other.m_uuidBytes = tmp;
        }
    }
}