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
#ifndef _MINLIB_b780618e_dce7_4154_a2e0_bcb2b286318d_H_
#define _MINLIB_b780618e_dce7_4154_a2e0_bcb2b286318d_H_

#include "../mn_algorithm.hpp"
#include <array>
#include <string>

namespace mn {
    namespace hash {
        /**
         * @brief This class represent a GUID/UUID. Each instance acts as a wrapper around a 
         * 16 byte value that can be passed around by value.
         */
        class basic_uuid {
        public:
            /**
             * @brief A null empty GUID/UUID
             * 00000000-0000-0000-0000-000000000000
             */
            static const basic_uuid Empty;
        public:
            using array_type = std::array<uint8_t, 16>;
            using string_type = std::string;

            explicit basic_uuid(string_type fromString);
            explicit basic_uuid(array_type &bytes);
            basic_uuid(const basic_uuid &other);
            
            /**
             * @brief Get the bytes object
             * @return The raw array of data
             */
            const array_type& get_bytes() const;

            void swap(basic_uuid &other);

            /**
             * @brief Is the GUID/UUID valid
             * @return true or false
             */
            bool is_valid();
            
            /**
             * @brief Is the other GUID/UUID equel with this 
             * 
             * @param other The other GUID/UUID
             * @return true The other GUID/UUID equel with this
             * @return false the other GUID/UUID not equel with this
             */
            bool operator == (const basic_uuid &other) {
                return (m_uuidBytes == other.m_uuidBytes);
            }
            /**
             * @brief Is the other GUID/UUID not equel with this 
             * 
             * @param other The other GUID/UUID
             * @return true The other GUID/UUID not equel with this
             * @return false the other GUID/UUID equel with this
             */
            bool operator != (const basic_uuid &other) {
                return (m_uuidBytes != other.m_uuidBytes);
            }
            
            basic_uuid &operator = (const basic_uuid &other) {
                m_uuidBytes = other.m_uuidBytes;
                return *this;
            }
        private:
            /**
             * @brief Create a null GUID/UUID
             * 00000000-0000-0000-0000-000000000000
             */
            void make_it_zero();
        private:
            array_type  m_uuidBytes;
        };

        /**
         * @brief Template specialization for mn::swap<basic_uuid>() 
         */ 
        inline void swap(basic_uuid &a, basic_uuid  &b) {
            a.swap(b);
        }

        bool operator < (const basic_uuid &lhs, const basic_uuid &rhs) {
            return lhs.get_bytes() <  rhs.get_bytes();
        }
        bool operator > (const basic_uuid &lhs, const basic_uuid &rhs) {
            return lhs.get_bytes() >  rhs.get_bytes();
        }
        bool operator <= (const basic_uuid &lhs, const basic_uuid &rhs) {
            return lhs.get_bytes() <  rhs.get_bytes();
        }
        bool operator >= (const basic_uuid &lhs, const basic_uuid &rhs) {
            return lhs.get_bytes() >  rhs.get_bytes();
        }

        using uuid_t = basic_uuid;
        using guid_t = basic_uuid;
        
        
    }
}

#endif