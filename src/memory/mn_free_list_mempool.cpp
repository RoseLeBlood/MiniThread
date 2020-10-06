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

#include "mn_autolock.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SAFE_DELETE(ob) if(ob != NULL) { delete ob; ob = NULL; }

#define MEMPOOL_SAFE_DELETE(mO) { \
    if(blockPtr->addr != NULL) { delete blockPtr->addr; blockPtr->addr = NULL; } \
    blockPtr->dim = 0; \
    blockPtr->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED; \
}

//-----------------------------------
//  __calc_item_size
//-----------------------------------
static int __calc_item_size( unsigned int uiItemSize, unsigned int uiAlignment) {

    if (uiItemSize <= uiAlignment) 
        return (2 * uiAlignment);
    
    int iCount = uiItemSize / uiAlignment;
 
    return (uiItemSize % uiAlignment != 0) ? 
        ( (iCount + 2) *  uiAlignment ) : 
        ( (iCount + 1) *  uiAlignment );
}

//-----------------------------------
//  __calc_alignment
//-----------------------------------
static int __calc_alignment(unsigned int uiAlignment) {
    //portBYTE_ALIGNMENT
    if (uiAlignment < (int)sizeof(unsigned char *))
        return (int)sizeof(unsigned char *);
    
    for (int i = 0, bit = 0x1; i < 31; i++) {
        if (uiAlignment == bit) {
            return uiAlignment;
        }
        bit <<= 1; 
    } 
    return 0; 
}

//-----------------------------------
//  basic_free_list_mempool
//-----------------------------------
basic_free_list_mempool::basic_free_list_mempool(unsigned int uiItemSize, 
                                                 unsigned int uiElements,
                                                 unsigned int uiAlignment)
    : IMemPool(uiItemSize, uiElements), m_nMutex() 
{ 
    m_uiAlignment = __calc_alignment(uiAlignment);
    m_bCreated = false;
}

//-----------------------------------
//  create
//-----------------------------------
int basic_free_list_mempool::create() {
    if(m_uiAlignment == 0) return ERR_MEMPOOL_BADALIGNMENT;

    m_uiItemSize = __calc_item_size(m_uiItemSize, m_uiAlignment);

    m_nMutex.lock();
    if(m_bCreated) { 
        m_nMutex.unlock();
        return ERR_MEMPOOL_ALREADYINIT;
    }
    m_bCreated = true;
    m_nMutex.unlock();
    
    if(m_lBytePtrList.empty())  {
        void* pMemObject = new char [m_uiItemSize *  m_uiElements];
        m_vMemoryPoolList.push_back(pMemObject);

        //initialise byte list
        for (int i = 0; i< m_uiElements; ++i) {
            memObject* __memObject = new memObject();

            __memObject->addr = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]);
            __memObject->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
            __memObject->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
            __memObject->dim = m_uiItemSize;
            __memObject->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE;

            m_nMutex.lock();
                m_lBytePtrList.push_back(__memObject); 
            m_nMutex.unlock();
        }
        return ERR_MEMPOOL_OK;
    }

    return ERR_MEMPOOL_UNKNOW;
}

//-----------------------------------
//  destroy
//-----------------------------------
void basic_free_list_mempool::destroy() {
    automutx_t lock(m_nMutex);

    memObject* blockPtr = NULL;

    for (int i = 0; i< m_uiElements; ++i) { 
        MEMPOOL_SAFE_DELETE( (m_lBytePtrList[i]) );
    }
    m_lBytePtrList.clear();

    m_vMemoryPoolList.clear();
}


//-----------------------------------
//  set_alignment
//-----------------------------------
int basic_free_list_mempool::set_alignment(uint8_t uiAlignment) {
    m_uiAlignment = __calc_alignment(uiAlignment);
    return m_uiAlignment;
}


//-----------------------------------
//  get_free_block
//-----------------------------------
basic_free_list_mempool::memObject* basic_free_list_mempool::get_free_block(TickType_t xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    memObject* blockPtr = NULL;

    while(xTicksRemaining <= xTicksToWait) {
       if(m_nMutex->lock(xTicksRemaining) != NO_ERROR){
                break;
        }

        for (int i = 0; i < m_uiElements; i++) {
            if(m_lBytePtrList[i]->blockAvaible == MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE) {
                blockPtr = m_lBytePtrList[i];

                m_nMutex->unlock();
                break;
            }
        }

        if (xTicksToWait != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_nMutex->unlock();
    }
    return blockPtr;
}

//-----------------------------------
//  allocate
//----------------------------------- 
void* basic_free_list_mempool::allocate(TickType_t xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    char* _blockAddr = (intptr_t *)0;

    while(xTicksRemaining <= xTicksToWait) {
        if(m_nMutex->lock(xTicksRemaining) != NO_ERROR) {
            break;
        }
    
        memObject* blockPtr = get_free_block(xTicksRemaining); 
        
        if(blockPtr != NULL) {
            blockPtr->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED;
            _blockAddr = blockPtr->addr;

            m_nMutex->unlock();
            break;
        }

        if (xTicksToWait != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_nMutex->unlock();
    }
    
    return (intptr_t *)_blockAddr;
}

//-----------------------------------
//  free
//----------------------------------- 
bool  basic_free_list_mempool::free(void* object, TickType_t xTicksToWait) {
    if(m_nMutex->lock(xTicksToWait) != NO_ERROR) {
        return false;
    }

    for (int i = 0; i < m_uiElements; i++) {
        if( (m_lBytePtrList[i]->addr == object) ) {
            if(m_lBytePtrList[i]->blockAvaible == MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE) return true;

            m_lBytePtrList[i]->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE;

            if(m_lBytePtrList[i]->memGuard[0] != MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START ||
               m_lBytePtrList[i]->memGuard[1] != MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END ) {
                   #if MN_THREAD_CONFIG_DEBUG == MN_THREAD_CONFIG_YES
                    printf("[Info] heap memory corrupted - write over the boundary (%d %d)\n", 
                           *((int*)object), i);
                    #endif
                    m_lBytePtrList[i]->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
                    m_lBytePtrList[i]->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END; 
                    m_lBytePtrList[i]->dim = m_uiItemSize;                   
            }
            m_nMutex->unlock();
            return true;
        }
    }
    return false;
}

//-----------------------------------
//  add_memory
//----------------------------------- 
int basic_free_list_mempool::add_memory(unsigned int nElements) {
    automutx_t lock(m_nMutex);

    void* pMemObject = new char [m_uiItemSize * nElements];
    m_vMemoryPoolList.push_back(pMemObject);

    for (int i = 0; i < nElements; i++) {
        memObject* __memObject = new memObject();

        __memObject->addr = &(static_cast<char*>(pMemObject)[i * m_uiItemSize]); 
        __memObject->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
        __memObject->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
        __memObject->dim = m_uiItemSize;
        __memObject->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE;

        m_lBytePtrList.push_back(__memObject); 
    }

    m_uiElements += nElements;


    return ERR_MEMPOOL_OK;
}

//-----------------------------------
//  add_memory
//----------------------------------- 
int basic_free_list_mempool::add_memory(void* preMemory, unsigned int nSize) {
    automutx_t lock(m_nMutex);
    
    unsigned int nElements = nSize / m_uiItemSize;
    m_vMemoryPoolList.push_back(preMemory);

    for (int i = 0; i < nElements; i++) {
        memObject* __memObject = new memObject();

        __memObject->addr = &(static_cast<char*>(preMemory)[i *  m_uiItemSize]); 
        __memObject->memGuard[0] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START;
        __memObject->memGuard[1] = MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END;
        __memObject->dim = m_uiItemSize;
        __memObject->blockAvaible = MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE;

        m_lBytePtrList.push_back(__memObject); 
    }

    m_uiElements += nElements;

    return ERR_MEMPOOL_OK;
}

//-----------------------------------
//  get_size
//----------------------------------- 
int basic_free_list_mempool::get_size() {
    automutx_t lock(m_nMutex);

    return m_lBytePtrList.size();
}

//-----------------------------------
//  is_empty
//----------------------------------- 
bool basic_free_list_mempool::is_empty() {
    automutx_t lock(m_nMutex);

    return ( get_free_items() ) == 0;
}

//-----------------------------------
//  get_free_items
//----------------------------------- 
unsigned int basic_free_list_mempool::get_free_items() {
    automutx_t lock(m_nMutex);

    unsigned int np = 0;

    for (int i = 0; i < m_uiElements; i++) {
        if(m_lBytePtrList[i]->blockAvaible == MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE)
            np++;
    }

    return np;
}

//-----------------------------------
//  get_used
//----------------------------------- 
unsigned int basic_free_list_mempool::get_used() {
    return (get_size() - get_free_items());
}
#if MN_THREAD_CONFIG_DEBUG == MN_THREAD_CONFIG_YES

//-----------------------------------
//  get_mem_object
//----------------------------------- 
basic_free_list_mempool::memObject basic_free_list_mempool::get_mem_object(void* mObject) {
    automutx_t lock(m_nMutex);

    memObject object = {
        .raw_memObject = nullptr
    };

    for (int i = 0; i < m_uiElements; i++) {
        if(m_lBytePtrList[i]->addr == mObject) {
            object = *(m_lBytePtrList[i]);
            break;
        }
    }
    return object;
}
#endif //MN_THREAD_CONFIG_DEBUG == MN_THREAD_CONFIG_YES

#endif