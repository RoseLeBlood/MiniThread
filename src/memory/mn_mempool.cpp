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
#include "freertos/FreeRTOS.h"
#include "memory/mn_mempool.hpp";
#include <malloc.h>
#include <iostream>

bool IMemPool::calcAligentAndSize() {
    unsigned int acount = 0;
    if (m_iAlignment < (int)sizeof(unsigned char *)) {
        m_iAlignment = (int)sizeof(unsigned char *);
    }
    
    for (int i = 0, a = 0x1; i <= 31; i++, a <<= 1) {
        if(i == 31) return false;
        else if (m_iAlignment == a) {
            break;
        } 
    }

    if (m_uiItemSize <= m_iAlignment)  m_uiItemSize = m_iAlignment;
    else {
        acount = m_uiItemSize / m_iAlignment;
        acount += (m_uiItemSize % m_iAlignment != 0) ? 1 : 0;
        m_uiItemSize = acount * m_iAlignment;
    }
    return true;
        
}
void* malloc_timed(unsigned long size, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    unsigned char *address = NULL;

    while( (xTicksRemaining <= xTicksToWait) && address == NULL ) {
        address = (unsigned char *)malloc(size);

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return address;
}

