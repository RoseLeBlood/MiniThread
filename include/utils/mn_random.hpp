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


#ifndef _MINLIB_RANDOM_H_
#define _MINLIB_RANDOM_H_

namespace mn {
    /**
     * @brief An Intarface for all randoms classes in this library
     * @author Amber-Sophia Schröck
     * @tparam TSEEDTYPE Seed Type
     */
    template <typename TSEEDTYPE>
    class IPseudoRandomUtil {
    protected:
        /**
         * @brief Union for type convertion
         */
        union convers {
            unsigned int g32;
            unsigned char g[4]; 
            unsigned short g16[2];
            
        };
    public:
        using seed_t = TSEEDTYPE;

        /**
         * @brief Construct a new IPseudoRandomUtil object
         * 
         * @param startSeed The start seed
         */
        IPseudoRandomUtil(seed_t startSeed) 
            : m_startSeed(startSeed) { }

        /**
         * @brief Get a random unsigned char number 
         * 
         * @return A random unsigned char number
         */
        virtual unsigned char rand8() = 0;
        /**
         * @brief Get a random unsigned short number
         * 
         * @return A random unsigned short number 
         */
        virtual unsigned short rand16() = 0;
        /**
         * @brief Get a random unsigned int number
         * 
         * @return A random unsigned int number 
         */
        virtual unsigned int rand32() = 0;

        /**
         * @brief Get the current used Seed 
         * 
         * @return The current used Seed 
         */
        seed_t get_seed() { return m_startSeed; }
        /**
         * @brief Set the a new seed
         * 
         * @param seed Tde new used seed
         */
        void set_seed(seed_t seed) { m_startSeed = seed; }
    protected:
        seed_t m_startSeed; 
    };
}

#endif