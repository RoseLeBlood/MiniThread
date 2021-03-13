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

#define HEXPAIR2CHAR(A, B) (mn::hex2char(A) * 16 + mn::hex2char(B))

namespace mn {
    namespace hash {
        basic_uuid::basic_uuid(array_type &bytes) 
            : m_uuidBytes(bytes) { }

        basic_uuid::basic_uuid(const basic_uuid &other) 
            : m_uuidBytes(other.m_uuidBytes) { }

        basic_uuid::basic_uuid(string_type fromString) {
            char charOne = '\0';
            char charTwo = '\0';
            bool lookingForFirstChar = true;
            unsigned nextByte = 0;

            for(int i = 0; i < fromString.length(); i++) {
                if (fromString[i] == '-') continue;

                if (nextByte >= 16 || !isprint(fromString[i])) {
                    make_it_zero();
                    break;
                }

                if (lookingForFirstChar) {
                    charOne = fromString[i];
                    lookingForFirstChar = false;
                } else {
                    charTwo = fromString[i];
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