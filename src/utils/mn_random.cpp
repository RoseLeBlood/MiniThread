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

#include "utils/mn_random.hpp"

using namespace nm;

#define NM_THREAD_CONFIG_RANDOM_LFSR_PM32 0xB4BCD35C
#define NM_THREAD_CONFIG_RANDOM_LFSR_PM31 0x7A5BC2E3

unsigned char basic_random_lfsr::rand8() {
    IPseudoRandomUtil::convers tp;
    tp.g16[0] = rand16();
    return tp.g[0] & tp.g[1];
}
unsigned short basic_random_lfsr::rand16() {
    shift(&m_uiPolyMask, NM_THREAD_CONFIG_RANDOM_LFSR_PM32);

    return static_cast<unsigned short>(
        shift(&m_uiPolyMask, NM_THREAD_CONFIG_RANDOM_LFSR_PM32) ^ 
        shift(&m_startSeed , NM_THREAD_CONFIG_RANDOM_LFSR_PM31));
}
unsigned int basic_random_lfsr::rand32() {
    IPseudoRandomUtil::convers tp;
    tp.g16[0] = rand16();
    tp.g16[1] = rand16();

    return tp.g32;
}

unsigned int basic_random_lfsr::shift(unsigned int* lfsr, unsigned int polyMask) {
    auto feedback = *lfsr & 1;
    *lfsr >>=1;

    if(feedback != 0U) *lfsr ^ polyMask;
    return *lfsr;
}