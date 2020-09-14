/** This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2020 Amber-Sophia Schroeck
 * 
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
**/
#include "mn_memory.hpp"

using namespace cpp_freertos;


bool basic_memory_pool::calc_valid_aligment() {
    if (m_iAlignment < (int)sizeof(unsigned char *)) {
        m_iAlignment = (int)sizeof(unsigned char *);
    }

    int alignmentBit = 0x1;
    int i;

    for (i = 0; i < 31; i++, alignmentBit <<= 1) {
        if (m_iAlignment == alignmentBit)
            break;
    }
    return (i >= 31) ? false : true;
}


void basic_memory_pool::calc_item_size() {
    if (m_iItemSize <= m_iAlignment) {
        m_iItemSize = m_iAlignment;
    } else {
        int alignmentCount = m_iItemSize / Alignment;
        if (m_iItemSize % m_iAlignment != 0) {
            alignmentCount++;
        }
        m_iItemSize = alignmentCount * m_iAlignment;
    }
}

int basic_memory_pool::create(int itemSize, int itemCount, int alignment) {
    return create(itemSize, malloc(ItemSize * itemCount), ItemSize * itemCount, alignment);
}
int basic_memory_pool::create(int itemSize, void *preallocatedMemory,
            int preallocatedMemorySize, int alignment) {

    m_iItemSize = itemSize;
    m_iAlignment = alignment;

    if(m_pLock == NULL) {
        m_pLock = new mutex_t();

        if(m_pLock->create() != ERR_MUTEX_OK)
            return -1;
    }
    if(!calc_valid_aligment()) return -3;

    calc_item_size();

    if(preallocatedMemory != 0) {
        add_memory(address, preallocatedMemorySize);
    } 
    return 0;
}

int basic_memory_pool::add_memory( void *pMemory, int size) {
    automutx_t autolock(*m_pLock);

    if(pMemory == 0) return -2;

    unsigned char *address = (unsigned char *)pMemory;

    while (size >= m_iItemSize) {
        FreeItems.push_back(address);
        address += m_iItemSize;
        size -= m_iItemSize;
    }
    return 0;
}
int basic_memory_pool::add_memory(int iCount) {
    return add_memory( malloc(m_iItemSize * iCount), m_iItemSize * iCount );
}
void *basic_memory_pool::allocate() {
    automutx_t autolock(*m_pLock);

    if (m_plFreeItems.empty())
        return NULL;

    void *item = m_plFreeItems.front();
    m_plFreeItems.pop_front();

    return item;
}


void basic_memory_pool::free(void *item) {
    automutx_t autolock(*m_pLock);

    m_plFreeItems.push_back(item);
}