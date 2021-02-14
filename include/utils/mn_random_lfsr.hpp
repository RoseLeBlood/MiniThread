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


#ifndef _MINLIB_RANDOM_LFSR_H_
#define _MINLIB_RANDOM_LFSR_H_

#include "mn_random.hpp"

namespace mn {
    /**
     * @brief Pseudro-Random Number Generator based on a 32-bit linear-feedback
     * shift register.  
     * 
     * @note For reference, the implementation is based on the
     * Maxim IC application note "AN4000"
     * http://www.maximintegrated.com/app-notes/index.mvp/id/4400
     * 
     * @author Amber-Sophia Schröck
     */ 
    class basic_random_lfsr : public IPseudoRandomUtil<unsigned int> {
    public:
        basic_random_lfsr(unsigned int startSeed = 0xFEABDC72, unsigned int polyMask = 0xA1B3C6DF) 
            : IPseudoRandomUtil<unsigned int>(startSeed), m_uiPolyMask(0xA1B3C6DF) { }

        /**
         * @brief Get a random unsigned char number 
         * 
         * @return A random unsigned char number
         */
        virtual unsigned char rand8() override;
        /**
         * @brief Get a random unsigned short number
         * 
         * @return A random unsigned short number 
         */
        virtual unsigned short rand16() override;
        /**
         * @brief Get a random unsigned int number
         * 
         * @return A random unsigned int number 
         */
        virtual unsigned int rand32() override;

        /**
         * @brief Get the polymask 
         * 
         * @return The polymask
         */
        unsigned int get_polymask() { return m_uiPolyMask; }
        /**
         * @brief Set the a new polymask
         * 
         * @param mask The new polymask
         */
        void set_polymask(unsigned int mask) { m_uiPolyMask = mask; }
        /**
         * @brief Set a new seed, with new polymask
         * 
         * @param seed The new seed
         * @param mask The new polymask
         */
        void set_seed(unsigned int seed, unsigned int mask) {
            m_startSeed = seed; m_uiPolyMask = mask; 
        }
    private:
        unsigned int m_uiPolyMask;
    };

}

#endif
    