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
#include "memory/mn_basic_mempool.hpp"
#include <malloc.h>
#include <iostream>
#include "mn_autolock.hpp"
#include <string.h>


#define _MEMPOOL_CLASS_LOCK(Mutex, xTicksRemaining) if(Mutex.lock(xTicksRemaining) != NO_ERROR) break;

#define _MEMPOOL_CLASS_UNLOCK(Mutex) Mutex->unlock();
#define _MEMPOOL_CLASS_UNLOCK_BREAK(Mutex) Mutex->unlock(); break;

MN_VECTOR_MEMPOOL_CLASS_NAME::MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements, 
    unsigned int iAlignment) : basic_mempool_interface(nItemSize, nElements, iAlignment) { 
    
}
void* MN_VECTOR_MEMPOOL_CLASS_NAME::allocate(unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    void* buffer = nullptr;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

        if(m_vChunks.size() == 0) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); }
        if( is_empty() ) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); }

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
            (it != m_vChunks.end()); it++) {
            chunk_t* entry = *it;

            if(entry->state == chunk_state::Free) {
                entry->state = chunk_state::Used;

                buffer = entry->realBuffer;
                _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);
            }
            
        }
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
    }
    return buffer;
}


#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
bool MN_VECTOR_MEMPOOL_CLASS_NAME::free(void* mem, bool* wasCurropted, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    bool _ret = false, _wasCurropted = false;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

        if(m_vChunks.size() == 0) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); }
        if(mem == NULL) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); } 

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
            (it != m_vChunks.end()); it++) {

            chunk_t* entry = *it; 

            if( (_ret = entry->realBuffer == mem) ) {
                if( (_wasCurropted = is_chunk_curropted(entry)) ) {
                    std::cout << "[MN_VECTOR_MEMPOOL_CLASS_NAME] entry was corrupted" << std::endl;

                    entry->theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                    entry->theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
                }

                entry->state = chunk_state::Free;
                memset_timed(entry->theBuffer, 0, m_uiItemSize, xTicksRemaining);
                if(wasCurropted) *wasCurropted = _wasCurropted;

                _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);

            } // if(entry->theBuffer == mem)

            if (timeout != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
            _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
        } // for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
    }
    return _ret;
}
#else

bool MN_VECTOR_MEMPOOL_CLASS_NAME::free(void* mem, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    bool _ret = false;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

        if(m_vChunks.size() == 0) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); }
        if(mem == NULL) { _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd); } 

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
            (it != m_vChunks.end()); it++) {

            chunk_t* entry = *it; 

            if( (_ret = entry->realBuffer == mem) ) {
                entry->state = chunk_state::Free;
                memset_timed(entry->theBuffer, 0, m_uiItemSize, xTicksRemaining);

                _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);

            } // if(entry->theBuffer == mem)

            if (timeout != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
            _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
        } // for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
    }
    return _ret;
}

#endif

int MN_VECTOR_MEMPOOL_CLASS_NAME::add_memory(unsigned int nElements, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    int i = 0;

    //unsigned char *address = (unsigned char *)MALLOC_TIMED(m_uiItemSize, nElements, xTicksRemaining);
    unsigned char *address =  m_allocator.malloc(m_uiItemSize*nElements, xTicksRemaining);
    
    if(address == NULL)  return ERR_NULL;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining); 

        for (i = 0; (i < nElements); i++) {
            m_vChunks.push_back(new chunk_t(address));
            address += m_uiItemSize;
        }

        if (timeout != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
    }
    return (i < nElements) ? NO_ERROR : ERR_MEMPOOL_UNKNOW;
}


int MN_VECTOR_MEMPOOL_CLASS_NAME::add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    
    if(preallocatedMemory == NULL) return ERR_NULL;
    unsigned char *address = (unsigned char *)preallocatedMemory;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

        while ((sSizeOf >= m_uiItemSize)  ) {
            m_vChunks.push_back(new chunk_t(address) );
            address += m_uiItemSize;
            sSizeOf -= m_uiItemSize;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
    }
    
    return ( (sSizeOf >= m_uiItemSize) ? ERR_MEMPOOL_UNKNOW : NO_ERROR );
}

unsigned long MN_VECTOR_MEMPOOL_CLASS_NAME::size() {
    automutx_t lock(m_mutex); 
    return m_uiItemSize * m_vChunks.size();
}
unsigned int chunk_size() {
    automutx_t lock(m_mutex); 
    return m_vChunks.size(); 
} 

bool MN_VECTOR_MEMPOOL_CLASS_NAME::set_blocked(const int id, const bool blocked, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    bool _ret = false;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

        if( (m_vChunks.at(id)->state != chunk_state::Used) ) {
            m_vChunks.at(id)->state = blocked ? chunk_state::Blocked : chunk_state::Free; 
            _ret = true; 
            
            _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
    }
    return _ret;
}

MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_state 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_state(const int id) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    chunk_state state = chunk_state::NotHandle;

    while((xTicksRemaining <= xTicksToWait)) {
        _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
        if(id > m_vChunks.size() ) break;

        if ( (state = m_vChunks.at(id)->state) != chunk_state::NotHandle) {
            _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);
        }
        
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
    }


    return state;
}

MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_t* 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_chunk(const int id, unsigned int xTicksToWait) {
    automutx_t lock(m_mutex);

    _cpyChunk = (chunk_t*)malloc_timed(sizeof(chunk_t), xTicksToWait);
    memcpy_timed(_cpyChunk, m_vChunks.at(id), sizeof(chunk_t), xTicksToWait);

    return _cpyChunk;
}

unsigned int MN_VECTOR_MEMPOOL_CLASS_NAME::get_used() {
    automutx_t lock(m_mutex);
    unsigned int _ret = 0;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
        if((*it)->state == chunk_state::Used) _ret++;
    }

    return _ret; 
}
unsigned int MN_VECTOR_MEMPOOL_CLASS_NAME::get_free() {
    automutx_t lock(m_mutex); 
    unsigned int _ret = 0;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
        if((*it)->state == chunk_state::Free) _ret++;
    }

    return _ret;
}
unsigned int MN_VECTOR_MEMPOOL_CLASS_NAME::get_blocked() {
    automutx_t lock(m_mutex); 
    unsigned int _ret = 0;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
        if((*it)->state == chunk_state::Blocked) _ret++;
    }

    return _ret;
}

#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
bool MN_VECTOR_MEMPOOL_CLASS_NAME::is_chunk_curropted(MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_t* chnk) {
    return (chnk->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
            chnk->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_END);
}
#endif