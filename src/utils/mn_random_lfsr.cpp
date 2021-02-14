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

#include "utils/mn_random_lfsr.hpp"

using namespace mn;

#define NM_THREAD_CONFIG_RANDOM_LFSR_PM32 0xB4BCD35C
#define NM_THREAD_CONFIG_RANDOM_LFSR_PM31 0x7A5BC2E3

#define MN_RANDOM_LFSR_SHIF(LFSR, POLYMASK) \
( ((LFSR & 1) != 0U) ? ( (LFSR >>= 1) ^ POLYMASK) : (LFSR >>= 1) )

unsigned char basic_random_lfsr::rand8() {
    IPseudoRandomUtil::convers tp;
    tp.g16[0] = rand16();
    return tp.g[0] & tp.g[1];
}
unsigned short basic_random_lfsr::rand16() {
    unsigned int _randomNumber =  0;

    MN_RANDOM_LFSR_SHIF(m_uiPolyMask, NM_THREAD_CONFIG_RANDOM_LFSR_PM32);

    _randomNumber  = MN_RANDOM_LFSR_SHIF(m_uiPolyMask, NM_THREAD_CONFIG_RANDOM_LFSR_PM32);
    _randomNumber ^= MN_RANDOM_LFSR_SHIF(m_startSeed, NM_THREAD_CONFIG_RANDOM_LFSR_PM31);

    return static_cast<unsigned short>(_randomNumber);
}
unsigned int basic_random_lfsr::rand32() {
    IPseudoRandomUtil::convers tp;
    tp.g16[0] = rand16();
    tp.g16[1] = rand16();

    return tp.g32;
}