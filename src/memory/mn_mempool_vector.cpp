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
#include "memory/mn_mempool_vector.hpp"
#include <malloc.h>
#include <iostream>
#include "mn_autolock.hpp"
#include <string.h>



basic_vector_mempool_timed::basic_vector_mempool_timed(unsigned int nItemSize, unsigned int nElements, 
    unsigned int iAlignment) : IMemPool(nItemSize, nElements, iAlignment) { 
    
}
int basic_vector_mempool_timed::create(unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    bool _ret = NO_ERROR;

    while( (xTicksRemaining <= xTicksToWait) ) {
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) {  
            _ret = ERR_UNKN; break; }

        if((IMemPool::create(xTicksRemaining) == NO_ERROR) && 
           (add_memory(m_uiElements, xTicksRemaining) != NO_ERROR) ) {  
            break;
        } else {
            _ret = ERR_UNKN; 
            break;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    return _ret;
}
void* basic_vector_mempool_timed::allocate(unsigned int xTicksToWait) {
    if(m_vChunks.size() == 0) { return nullptr; }

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    void* buffer = nullptr;

    while( (xTicksRemaining <= xTicksToWait) && (buffer == nullptr) ) {
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;
        
        
        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); it != m_vChunks.end(); it++) {
            chunk_t* entry = *it;

            if(entry->state == chunk_state::Free) {
                entry->state = chunk_state::Used;

                buffer = entry->theBuffer;
                break;
            }
        }
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        
        m_mutexAdd->unlock();
    }
    return buffer;
}

bool basic_vector_mempool_timed::free(void* mem, unsigned int xTicksToWait) {
    if(m_vChunks.size() == 0) return false;
    if(mem == NULL) return false;
    bool _ret = false;

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    while( (xTicksRemaining <= xTicksToWait)  ) {
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); it != m_vChunks.end(); it++) {
            chunk_t* entry = *it; 

            if(entry->theBuffer == mem) {
                entry->wasCurropted = 
                    (entry->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
                    entry->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START);

                entry->state = chunk_state::Free;

                memset_timed(entry->theBuffer, 0, m_uiItemSize);

                if(entry->wasCurropted) {
                    std::cout << "[basic_vector_mempool_timed] entry was corrupted" << std::endl;

                    entry->theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                    entry->theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
                }
                _ret = true;
                break;
            } 
        }
        if(ret) { m_mutexAdd->unlock(); break; }
        
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutexAdd->unlock();
    }

    return _ret;
}

int basic_vector_mempool_timed::add_memory(unsigned int nElements, unsigned int xTicksToWait) {
    int _return = ERR_UNKN;

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    while( (xTicksRemaining <= xTicksToWait)  ) {
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        unsigned char *address = (unsigned char *)MALLOC_TIMED(m_uiItemSize, nElements, xTicksRemaining);
        if (address == NULL) { m_mutex.unlock() _return = ERR_NULL; break; }
        
        for (int i = 0; i < nElements; i++) {
            m_vChunks.push_back(new chunk_t(address));
            address += m_uiItemSize;
            _return = NO_ERROR;
        }

        if(_return == NO_ERROR) { m_mutexAdd->unlock(); break; }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutexAdd->unlock();
    }

    return _return;
}


int basic_vector_mempool_timed::add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) {
    if(preallocatedMemory == NULL) return ERR_NULL;

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    bool _bIsPushed = false;

    while( (xTicksRemaining <= xTicksToWait) && (_bIsPushed == false) ) {
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;
    
        unsigned char *address = (unsigned char *)preallocatedMemory;

        while (sSizeOf >= m_uiItemSize) {
            m_vChunks.push_back(new chunk_t(address) );
            address += m_uiItemSize;
            sSizeOf -= m_uiItemSize;

            _bIsPushed = true;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        
        m_mutexAdd->unlock();
    }
    return NO_ERROR;
}

chunk_t* basic_vector_mempool_timed::get_chunk_from_mem(void* mem) {
    automutx_t lock(m_mutex); 

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); it != m_vChunks.end(); it++) {
        if((*it)->theBuffer == mem) return (*it);
    }

    return nullptr;
}
unsigned long basic_vector_mempool_timed::size() {
    automutx_t lock(m_mutex); 
    unsigned long _ret = m_uiItemSize * m_vChunks.size();

    return _ret;
}
unsigned int chunk_size() {
    automutx_t lock(m_mutex); 
    return m_vChunks.size(); 
} 