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
#ifndef _MINLIB_VECTOR_MEMPOOL_H_
#define _MINLIB_VECTOR_MEMPOOL_H_


#include <malloc.h>
#include <iostream>
#include "mn_autolock.hpp"
#include <string.h>
#include <vector>

#include "mn_mempool.hpp"

#define _MEMPOOL_CLASS_LOCK(Mutex, xTicksRemaining) if(Mutex.lock(xTicksRemaining) != NO_ERROR) break;

#define _MEMPOOL_CLASS_UNLOCK(Mutex) Mutex->unlock();
#define _MEMPOOL_CLASS_UNLOCK_BREAK(Mutex) Mutex->unlock(); break;

/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
template <typename TType, typename TALLOCATOR = default_allocator_t<unsigned char> >
class basic_mempool_vector : public mempool_interface<TALLOCATOR> {
public:
    /**
     * The memory chunk 
     */ 
    struct chunk {
        union {
            struct {
                unsigned char* theBuffer;            /*!< The real buffer */
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
                char theMagicGuard[2];      /*!< The magic guard bytes for detect heap memory corruption */
#endif
                chunk_state state;          /*!< The state for a memory chunk */
            };
            unsigned char* realBuffer;
        };
        chunk(unsigned char* buffer) { 
            theBuffer = buffer;
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
            theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
            theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
#endif
            state = chunk_state::Free;
        }
    };
public:
    using chunk_t = chunk;

    /**
     * Ctor
     * @param[in] nItemSize The size of a item
     */ 
    explicit basic_mempool_vector() 
        : basic_mempool_vector(20, MN_THREAD_CONFIG_BASIC_ALIGNMENT) { }

    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     */ 
    basic_mempool_vector(unsigned int nElements)
        : basic_mempool_vector(nElements, MN_THREAD_CONFIG_BASIC_ALIGNMENT) { }

    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     * @param[in] iAlignment Requested alignment, in bytes.
     */ 
    basic_mempool_vector(unsigned int nElements, unsigned int iAlignment)
        : basic_mempool_interface(sizeof(TType), nElements, iAlignment)  { }

    basic_mempool_vector(const basic_mempool_vector&) = delete; ///< no copyable 

    /**
     * Allocate an item from the pool.
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait) {
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
    /**
     * Returns the item back to the pool.
     * @param[in]  mem The allocated memory to given back te the pool
     * @param[in]  xTicksToWait How long to wait to get until giving up.
     * @param[out] wasCurropted A pointer of a bool var, 
     * if true then was the heap memory corrupted and false when not 
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, bool* wasCurropted, unsigned int xTicksToWait);
#else 
    /**
     * Returns the item back to the pool.
     * @param[in]  mem The allocated memory to given back te the pool
     * @param[in]  xTicksToWait How long to wait to get until giving up.
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait);
#endif

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] itemCount How many more items max do you want to allocate
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return 0 when was added and '1' on error
     */
    int add_memory(unsigned int nElements, unsigned int xTicksToWait) {
        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;
        int i = 0;

        //unsigned char *address = (unsigned char *)MALLOC_TIMED(m_uiItemSize, nElements, xTicksRemaining);
        unsigned char *address =  (unsigned char *)m_allocator.alloc_raw(m_uiItemSize, nElements, xTicksRemaining);
        
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

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param[in] sSizeOf [in] The size of the preallocated memory
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return 0 when was added and '1' on error
     */
    int add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) {
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

    /**
     * No copyble
     */ 
    basic_mempool_vector& operator=(const basic_mempool_vector&) = delete;

    /**
     * Return the number of chunks in the mempool
     * @return The number of chunks in the mempool
     */ 
    unsigned int chunk_size() {
        automutx_t lock(m_mutex); 
        return m_vChunks.size(); 
    }

    /**
     * Return the size of memory they are handle in the pool
     * @return The size of memory they are handle in the pool
     */ 
    virtual unsigned long size() override {
        automutx_t lock(m_mutex); 
        return m_uiItemSize * m_vChunks.size();
    }

    /**
     * How many elements are marked as used
     * @return The number of elements / chunks are marked as use
     */ 
    unsigned int get_used() {
        automutx_t lock(m_mutex);
        unsigned int _ret = 0;

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
            if((*it)->state == chunk_state::Used) _ret++;
        }
        return _ret; 
    }
    /**
     * How many elements are marked as free
     * @return The number of elements / chunks are marked as free
     */ 
    unsigned int get_free() {
        automutx_t lock(m_mutex); 
        unsigned int _ret = 0;

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
            if((*it)->state == chunk_state::Free) _ret++;
        }

        return _ret;
    }
    /**
     * How many elements are marked as blocked
     * @return The number of elements / chunks are marked as blocked
     */ 
    unsigned int get_blocked() {
        automutx_t lock(m_mutex); 
        unsigned int _ret = 0;

        for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
            if((*it)->state == chunk_state::Blocked) _ret++;
        }

        return _ret;
    }

    /**
     * Is the mempool empty?
     * @return True when all chunks are used (empty) and false when not
     */ 
    virtual bool is_empty() { return ( size() ) == ( get_used() + get_blocked() ); }
    /**
     * block a chunk 
     * @param[in] id The id of the chunk
     * @param[in] blocked If True than block the chunk and if false then unblock the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return False the chunk is in use and can not block or release. If true then 
     * was block or unblock the chunk 
     */ 
    bool set_blocked(const int id, const bool blocked, unsigned int xTicksToWait) {
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

    /**
     * Get the state of the chunk
     * @param[in] id The id of the chunk
     * @return The state of the chunk
     */ 
    chunk_state get_state(const int id) {
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

    /**
     * Get the chunk from a given chunk id
     * @param[in] id The id of the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * 
     * @return The chunk from a given chunk id
     */ 
    chunk_t* get_chunk(const int id, unsigned int xTicksToWait) {
        automutx_t lock(m_mutex);

        _cpyChunk = (chunk_t*)m_allocator.alloc_raw(sizeof(chunk_t), 1, xTicksToWait);
        memcpy_timed(_cpyChunk, m_vChunks.at(id), sizeof(chunk_t), xTicksToWait);

        return _cpyChunk;
    }

#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
    /**
     * Is the given chunk_t curropted
     * @param [in] chnk The chunk to cheak
     * @return true the given chunk_t is curropted or false when not
     */ 
    bool is_chunk_curropted(chunk_t* chnk) {
        return (chnk->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
            chnk->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_END);
    }
#endif

private:
    std::vector<chunk_t*> m_vChunks;
};


#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
template <typename TType, typename TALLOCATOR >
bool basic_mempool_vector<TType, TALLOCATOR>::free(void* mem, bool* wasCurropted, unsigned int xTicksToWait) {
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

template <typename TType, typename TALLOCATOR >
bool basic_mempool_vector<TType, TALLOCATOR>::free(void* mem, unsigned int xTicksToWait) {
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


#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
using basic_mempool_spiram_t = basic_mempool_vector< basic_allocator_spiram <unsigned char> >;
#endif
using basic_mempool_system_t = basic_mempool_vector< basic_allocator_system <unsigned char> >;
using basic_mempool_t = basic_mempool_system_t;

#endif
