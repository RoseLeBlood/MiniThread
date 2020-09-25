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
#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES
#warning("use on your own risk - not for production use")

#include "mn_autolock.hpp"

static int __calc_item_size( unsigned int uiItemSize, unsigned int uiAlignment) {

    if (uiItemSize <= uiAlignment) 
        return (2 * uiAlignment);
    
    int alignmentCount = uiItemSize / uiAlignment;
    if (uiItemSize % uiAlignment != 0) 
        alignmentCount++;
        
    return ((alignmentCount + 1) * uiAlignment);
}
static int __calc_alignment(unsigned int uiAlignment) {

    if (uiAlignment < (int)sizeof(unsigned char *))
        return (int)sizeof(unsigned char *);
    
    for (int i = 0, bit = 0x1; i < 31; i++) {
        if (uiAlignment == bit) {
            return uiAlignment;
        }
        bit <<= 1; 
    }  
}

basic_free_list_mempool::basic_free_list_mempool(unsigned int uiItemSize, 
                                                 unsigned int uiElements,
                                                 unsigned int uiAlignment)
    : IMemPool(uiItemSize, uiElements) 
{ 
    m_uiAlignment = __calc_alignment(uiAlignment);
    m_bCreated = false;
}

int basic_free_list_mempool::create() {
    if(m_uiAlignment == 0) return -5;

    m_uiItemSize = __calc_item_size(m_uiItemSize, m_uiAlignment);

    m_nMutex.create();

    m_nMutex.lock();
    if(m_bCreated) { 
        m_nMutex.unlock();
        return ERR_MEMPOOL_ALREADYINIT;
    }
    m_bCreated = true;
    m_nMutex.unlock();
    
    if(m_lBytePtrList.empty())  {
        void* pMemObject = new char [m_uiItemSize * m_uiElements];
        m_vMemoryPoolList.push_back(pMemObject);

        //initialise byte list
        for (int i = 0; i< m_uiElements; ++i) {
            memObject* __memObject = new memObject();

            __memObject->memBlock = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]);
            __memObject->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
            __memObject->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
            __memObject->sizeofBlock = m_uiItemSize;
            __memObject->blockAvaible = 1;

            m_nMutex.lock();
                m_lBytePtrList.push_front(__memObject); 
            m_nMutex.unlock();
        }
        return ERR_MEMPOOL_OK;
    }

    return ERR_MEMPOOL_UNKNOW;
}
void* basic_free_list_mempool::allocate() {

    m_nMutex.lock();
        memObject* blockPtr =  m_lBytePtrList.front();
    m_nMutex.unlock();

    if(blockPtr == NULL) return NULL;

    blockPtr->blockAvaible = 0;

    m_nMutex.lock();
        m_lBytePtrList.pop_front();
    m_nMutex.unlock();

    return blockPtr;
}
bool  basic_free_list_mempool::free(void* object) {
    automutx_t lock(m_nMutex)

    memObject* init = static_cast<memObject*>(object);

    if(init == NULL) return false;

    if( (init->memGuard[0] != MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START) && 
        (init->memGuard[1] != MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END)  )
        return false;

    memset(init->memBlock, 0, m_uiItemSize);
    init->blockAvaible = 1;
    init->sizeofBlock = m_uiItemSize;

    m_lBytePtrList.push_back(init);

    object = NULL;

    return true;
}
int basic_free_list_mempool::add_memory(unsigned int nElements) {
    automutx_t lock(m_nMutex);

    void* pMemObject = new char [m_uiItemSize * nElements];
    m_vMemoryPoolList.push_back(pMemObject);

    for (int i = 0; i < nElements; i++) {
        memObject* __memObject = new memObject();

        __memObject->memBlock = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]); 
        __memObject->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
        __memObject->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
        __memObject->sizeofBlock = m_uiItemSize;
        __memObject->blockAvaible = 1;

        m_lBytePtrList.push_front(__memObject); 
    }

    m_uiElements += nElements;


    return ERR_MEMPOOL_OK;
}
int basic_free_list_mempool::add_memory(void* preMemory, unsigned int nSize) {
    automutx_t lock(m_nMutex);
    
    unsigned int nElements = nSize / m_uiItemSize;
    m_vMemoryPoolList.push_back(preMemory);

    for (int i = 0; i < nElements; i++) {
        memObject* __memObject = new memObject();

        __memObject->memBlock = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]); 
        __memObject->guardStart = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
        __memObject->guardEnd = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
        __memObject->sizeofBlock = m_uiItemSize;
        __memObject->blockAvaible = 1;

        m_lBytePtrList.push_front(__memObject); 
    }

    m_uiElements += nElements;

    return ERR_MEMPOOL_OK;
}
int basic_free_list_mempool::get_size() {
    automutx_t lock(m_nMutex);

    return m_lBytePtrList.size();
}
bool basic_free_list_mempool::is_empty() {
    automutx_t lock(m_nMutex);

    return (m_lBytePtrList.size()) == 0;
}
#endif