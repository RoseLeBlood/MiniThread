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

namespace nm {
    /**
     * An Intarface for all randoms classes in this library
     */ 
    template <typename TSEEDTYPE>
    class IPseudoRandomUtil {
    protected:
        union convers {
            unsigned int g32;
            unsigned char g[4]; 
            unsigned short g16[2];
            
        };
    public:
        using seed_t = TSEEDTYPE;

        IPseudoRandomUtil(seed_t startSeed) 
            : m_startSeed(startSeed) { }

        virtual unsigned char rand8() = 0;
        virtual unsigned short rand16() = 0;
        virtual unsigned int rand32() = 0;

        seed_t get_seed() { return m_startSeed; }
        void set_seed(seed_t seed) { m_startSeed = seed; }
    protected:
        seed_t m_startSeed; 
    };

    /**
     * Pseudro-Random Number Generator based on a 32-bit linear-feedback
     * shift register.  For reference, the implementation is based on the
     * Maxim IC application note "AN4000"
     * 
     * http://www.maximintegrated.com/app-notes/index.mvp/id/4400
     */ 
    class basic_random_lfsr : public IPseudoRandomUtil<unsigned int> {
    public:
        basic_random_lfsr(unsigned int startSeed = 0xFEABDC72, unsigned int polyMask = 0xA1B3C6DF) 
            : IPseudoRandomUtil<unsigned int>(startSeed), m_uiPolyMask(0xA1B3C6DF) { }

        virtual unsigned char rand8();
        virtual unsigned short rand16();
        virtual unsigned int rand32();

        unsigned int get_polymask() { return m_uiPolyMask; }
        void set_polymask(unsigned int mask) { m_uiPolyMask = mask; }

        void set_seed(unsigned int seed, unsigned int mask) {
            m_startSeed = seed; m_uiPolyMask = mask; 
        }
    private:
        unsigned int shift(unsigned int* x, unsigned int y);
    private:
        unsigned int m_uiPolyMask;
    };
}

#endif