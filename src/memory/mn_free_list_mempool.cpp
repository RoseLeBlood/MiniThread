/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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

#include "memory/mn_free_list_mempool.hpp"
#include <stdlib.h>
#if MN_THREAD_CONFIG_PREVIEW_DEV_ACTIVATE == MN_THREAD_CONFIG_YES
#warning("use on your own risk - not for production use")

basic_free_list_mempool::basic_free_list_mempool(unsigned int uiItemSize, 
                                                 unsigned int uiElements)
    : m_uiItemSize(uiItemSize), m_uiElements(uiElements), m_uiSizeOver(uiItemSize - sizeof(char))  { }

int basic_free_list_mempool::create() {
    if(m_lBytePtrList.empty())  {
        void* pMemObject = new char [m_uiItemSize * m_uiElements];
        m_vMemoryPoolList.push_back(pMemObject);

        //initialise byte list
        for (int i = 0; i< m_uiElements; ++i) {
            memObject* __memObject = new memObject();

            __memObject->memBlock = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]) + m_uiSizeOver;
            __memObject->guardStart = 0x6d;
            __memObject->guardEnd = 0xa8;
            __memObject->sizeofBlock = m_uiItemSize;
            __memObject->blockAvaible = 1;

            m_lBytePtrList.push_front(__memObject); 
        }
        return 0;
    }
    return -1;
}
void* basic_free_list_mempool::allocate(unsigned int size) {
    if(m_uiItemSize < size) return NULL;

    memObject* blockPtr =  m_lBytePtrList.front();
    if(blockPtr == NULL) return NULL;

    blockPtr->sizeofBlock = size;
    blockPtr->blockAvaible = 0;

    m_lBytePtrList.pop_front();

    return blockPtr;
}
bool  basic_free_list_mempool::free(void* object) {
    memObject* init = static_cast<memObject*>(object);

    if(init == NULL) return false;

    if( (init->guardStart != 0x6d) && (init->guardStart != 0xa8)  )
        return false;

    init->blockAvaible = 1;
    init->sizeofBlock = m_uiItemSize;

    m_lBytePtrList.push_back(init);

    return true;
}
int basic_free_list_mempool::add_memory(unsigned int nItemSize, unsigned int nElements) {

}
int basic_free_list_mempool::add_memory(void* preMemory, unsigned int nSize) {

}
#endif