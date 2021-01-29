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



MN_VECTOR_MEMPOOL_CLASS_NAME::MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements, 
    unsigned int iAlignment) : IMemPool(nItemSize, nElements, iAlignment) { 
    
}
int MN_VECTOR_MEMPOOL_CLASS_NAME::create(unsigned int xTicksToWait) {
    bool _ret = NO_ERROR;

    if((IMemPool::create(xTicksToWait) != NO_ERROR) ) _ret = ERR_MEMPOOL_UNKNOW;
    if(add_memory(m_uiElements, xTicksToWait) != NO_ERROR) _ret = ERR_MEMPOOL_UNKNOW;

    return _ret;
}
void* MN_VECTOR_MEMPOOL_CLASS_NAME::allocate(unsigned int xTicksToWait) {
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

bool MN_VECTOR_MEMPOOL_CLASS_NAME::is_chunk_curropted(MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_t* chnk) {
    return (chnk->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
            chnk->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_END);
}

bool MN_VECTOR_MEMPOOL_CLASS_NAME::free(void* mem, unsigned int xTicksToWait) {
    if(m_vChunks.size() == 0) return false;
    if(mem == NULL) return false;
    bool _ret = false;

    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
        (it != m_vChunks.end()) && (xTicksRemaining <= xTicksToWait); it++) {
        
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        chunk_t* entry = *it; 

        if(entry->theBuffer == mem) {
            entry->wasCurropted = is_chunk_curropted(entry);

            entry->state = chunk_state::Free;
#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
            memset_timed(entry->theBuffer, 0, m_uiItemSize, xTicksRemaining);
#else
            memset(entry->theBuffer, 0, m_uiItemSize);
#endif
            if(entry->wasCurropted) {
                std::cout << "[MN_VECTOR_MEMPOOL_CLASS_NAME] entry was corrupted" << std::endl;

                entry->theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                entry->theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
            }
            _ret = true;
            m_mutex.unlock();
            break;
        } // if(entry->theBuffer == mem)

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutexAdd->unlock();
    } // for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
    return _ret;
}

int MN_VECTOR_MEMPOOL_CLASS_NAME::add_memory(unsigned int nElements, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    int i = 0;

    if(m_mutex.lock(xTicksRemaining) != NO_ERROR) return ERR_MUTEX_LOCK;

    unsigned char *address = (unsigned char *)MALLOC_TIMED(m_uiItemSize, nElements, xTicksRemaining);
    if(address == NULL) {
        m_mutex.unlock;
        return ERR_NULL;
    }

    for (i = 0; (i < nElements) && (xTicksRemaining <= xTicksToWait); i++) {
        m_vChunks.push_back(new chunk_t(address));
        address += m_uiItemSize;
        
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }

    m_mutex.unlock;
 
    return (i < nElements) ? NO_ERROR : ERR_MEMPOOL_UNKNOW;
}


int MN_VECTOR_MEMPOOL_CLASS_NAME::add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) {
    if(preallocatedMemory == NULL) return ERR_NULL;
    
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;
    
    if(m_mutex.lock(xTicksRemaining) != NO_ERROR) return ERR_MUTEX_LOCK;
    unsigned char *address = (unsigned char *)preallocatedMemory;

    
    while ((sSizeOf >= m_uiItemSize) && (xTicksRemaining <= xTicksToWait) ) {
        m_vChunks.push_back(new chunk_t(address) );
        address += m_uiItemSize;
        sSizeOf -= m_uiItemSize;

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
    }
    m_mutexAdd->unlock();
    
    return ( (sSizeOf >= m_uiItemSize) ? ERR_MEMPOOL_UNKNOW : NO_ERROR );
}

unsigned long MN_VECTOR_MEMPOOL_CLASS_NAME::size() {
    automutx_t lock(m_mutex); 
    unsigned long _ret = m_uiItemSize * m_vChunks.size();

    return _ret;
}
unsigned int chunk_size() {
    automutx_t lock(m_mutex); 
    return m_vChunks.size(); 
} 

bool MN_VECTOR_MEMPOOL_CLASS_NAME::set_blocked(const int id, const bool blocked, unsigned int xTicksToWait) {
    bool _ret = false;

    if( (m_mutex.lock(xTicksToWait)) && (m_vChunks.at(id)->state != chunk_state::Used)) {
        m_vChunks.at(id)->state = blocked ? chunk_state::Blocked : chunk_state::Free; 
        _ret = true; 
        m_mutex.unlock();
    }
    
    return _ret;
}

bool MN_VECTOR_MEMPOOL_CLASS_NAME::set_address_blocked(const int address, const bool blocked, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    bool _ret = false;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
        (it != m_vChunks.end()) && (xTicksRemaining <= xTicksToWait) ; it++) {
        
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;
        
        chunk_t* entry = *it; 

        if( (int32_t)(entry->theBuffer) == address) {
           _ret = entry->state == chunk_state::Blocked;
            m_mutex.unlock(); break;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }

        m_mutex.unlock();
    }

    return _ret;
}

MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_state 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_state(const int id, unsigned int xTicksToWait) {
    automutx_t lock(m_mutex);

    if(id > m_vChunks.size() ) return chunk_state::NotHandle;

    return (m_vChunks.at(id)->state);
}
MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_state 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_address_state(const int address, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    chunk_state _ret = chunk_state::NotHandle;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
        (it != m_vChunks.end()) && (xTicksRemaining <= xTicksToWait) ; it++ ) {
        
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        if( (int32_t)( (*it)->theBuffer) == address) {
           _ret = entry->state; m_mutex.unlock(); break;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutex.unlock();
    }

    return _ret;
}

bool MN_VECTOR_MEMPOOL_CLASS_NAME::is_handle(const int address, unsigned int xTicksToWait) {
    TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
    TickType_t xTicksRemaining = xTicksToWait;

    bool _ret = false;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
        (it != m_vChunks.end()) && (xTicksRemaining <= xTicksToWait) ; it++ ) {
        
        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        if( (int32_t)( (*it)->theBuffer ) == address) {
           _ret = true; m_mutex.unlock(); break;
        }

        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutex.unlock();
    }

    return _ret;
}

MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_t* 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_chunk(const int id, unsigned int xTicksToWait) {
    chunk_t* _cpyChunk = NULL;

    if(m_mutex.lock(xTicksToWait) != NO_ERROR) return NULL;

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
        _cpyChunk = (chunk_t*)malloc_timed(sizeof(chunk_t), portMAX_DELAY);
        memcpy_timed(_cpyChunk, m_vChunks.at(id), sizeof(chunk_t), portMAX_DELAY);
#else
        _cpyChunk = (chunk_t*)malloc(sizeof(chunk_t) );
        memcpy(_cpyChunk, m_vChunks.at(id), sizeof(chunk_t) );
#endif

    m_mutex.unlock();

    return _cpyChunk;
}
MN_VECTOR_MEMPOOL_CLASS_NAME::chunk_t* 
MN_VECTOR_MEMPOOL_CLASS_NAME::get_chunk_from_address(const int address, unsigned int xTicksToWait) {
    chunk_t* _cpyChunk = NULL;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); 
        (it != m_vChunks.end()) && (xTicksRemaining <= xTicksToWait) ; it++ ) {

        if(m_mutex.lock(xTicksRemaining) != NO_ERROR) break;

        if( (int32_t)( (*it)->theBuffer ) == address) {

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
            _cpyChunk = (chunk_t*)malloc_timed(sizeof(chunk_t), portMAX_DELAY);
            if(_cpyChunk)
                memcpy_timed(_cpyChunk, (*it), sizeof(chunk_t), portMAX_DELAY);
#else
            _cpyChunk = (chunk_t*)malloc(sizeof(chunk_t));
            if(_cpyChunk)
                memcpy(_cpyChunk, (*it), sizeof(chunk_t));
#endif
            m_mutex.unlock();
            break;
        }
        if (timeout != portMAX_DELAY) {
            xTicksRemaining = xTicksEnd - xTaskGetTickCount();
        }
        m_mutex.unlock();
    }
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
