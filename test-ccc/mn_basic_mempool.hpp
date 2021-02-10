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
#include <string.h>
#include <vector>

#include "mn_allocator.hpp"

#define _MEMPOOL_CLASS_LOCK(Mutex, xTicksRemaining) ;
#define _MEMPOOL_CLASS_UNLOCK(Mutex) ;
#define _MEMPOOL_CLASS_UNLOCK_BREAK(Mutex)  break;

#include "mn_config.hpp"
#include "mn_error.hpp"

using TickType_t = unsigned long;

int tick = 0;

inline TickType_t xTaskGetTickCount() {
    return tick++;
}

#define ERR_MEMPOOL_UNKNOW 100

const unsigned long portMAX_DELAY=0xfffffffffUL;

#undef MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC
#define MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC 1
/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
template <typename TType, int nElements, typename TALLOCATOR = default_allocator_t<TType> >
class basic_mempool_vector {
public:
    /**
     * The state for a memory chunk
     */ 
    enum class chunk_state {
        Free,                   /*!< The chunk is free and can allocated */
        Used,                   /*!< The chunk is used */
        Blocked,                /*!< The chunk is blocked and can not use */
        NotHandle = 99          /*!< Return when the address not handle with this mempool */
    };
    using type_t = TType;
    using allocator_t = TALLOCATOR;

    /**
     * The memory chunk 
     */ 
    struct chunk {
        union {
            struct {
                TType* theBuffer;            /*!< The real buffer */
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
                char theMagicGuard[2];      /*!< The magic guard bytes for detect heap memory corruption */
#endif
                chunk_state state;          /*!< The state for a memory chunk */
            };
            TType* realBuffer;
        };
        chunk(TType* buffer) { 
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
     */ 
    basic_mempool_vector() : m_uiElements(nElements) {  }
    basic_mempool_vector(const basic_mempool_vector&) = delete; ///< no copyable 

    /**
     * Create the mempool 
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return - ERR_MEMPOOL_OK: Mempool are created
     *         - ERR_MEMPOOL_CREATE: Can't create the mempool 
     */ 
    virtual int create(unsigned int xTicksToWait) {    
        if(m_allocator.create(0) == false) return ERR_MEMPOOL_CREATE;
        
        TType *address;
        size_t sSizeOf = m_allocator.calloc(m_uiElements, &address, xTicksToWait);
        if(address == NULL)  return ERR_NULL;

        //_MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
        for(int i = 0; i < sSizeOf; i++) {
            m_vChunks.push_back(new chunk_t( &address[i] ) );
        }
        _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
       
        return m_vChunks.size() == sSizeOf ? NO_ERROR : ERR_MEMPOOL_CREATE;
    }

    virtual bool add_memory(unsigned int elements) {
        /*automutx_t lock(m_mutex);*/ 

        TType *address;
        size_t _oldEle = m_uiElements;

        size_t sSizeOf = m_allocator.calloc(elements, &address, (int)portMAX_DELAY);
        if(address == NULL)  return false;

        for(int i = 0; i < sSizeOf; i++) {
            m_vChunks.push_back(new chunk_t( &address[i] ) );
            
        }
        m_uiElements += sSizeOf;

        return _oldEle < m_uiElements;
    }
    virtual int add_memory(TType** address, size_t sSizeOf) {
        /*automutx_t lock(m_mutex);*/ 

        if(address == NULL)  return false;
        size_t _oldEle = m_uiElements;

        for(int i = 0; i < sSizeOf; i++) {
            m_vChunks.push_back(new chunk_t( address[i] ) );
            
        }
        m_uiElements += sSizeOf;

        return _oldEle < m_uiElements;
    }

   


    /**
     * Allocate an item from the pool.
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual type_t* allocate(unsigned int xTicksToWait = portMAX_DELAY) {
        if(m_vChunks.size() == 0) { return NULL; }

        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;

        type_t* buffer = nullptr;

        for(auto it = m_vChunks.begin(); 
            (it != m_vChunks.end() /* && (xTicksRemaining <= xTicksToWait) ) */); it++) {

            _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
            {
                    chunk_t* entry = *it;

                if(entry->state == chunk_state::Free) {
                    entry->state = chunk_state::Used;

                    buffer = entry->realBuffer;
                    _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);
                }
            }
            if (xTicksToWait != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
            _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
        }
          
        return buffer;
    }

    /**
     * Returns the item back to the pool.
     * @param[in]  mem The allocated memory to given back te the pool
     * @param[in]  xTicksToWait How long to wait to get until giving up.
     * @param[out] wasCurropted A pointer of a bool var - not use in the magic free version 
     * if true then was the heap memory corrupted and false when not 
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(type_t* mem, bool* wasCurropted, unsigned int xTicksToWait = portMAX_DELAY) {
        if(m_vChunks.size() == 0) { return false; }
        if(mem == NULL) { return false; }

        TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
        TickType_t xTicksRemaining = xTicksToWait;
        bool _ret = false, _wasCurropted = false;
        chunk_t* entry = NULL; 

        for(auto it = m_vChunks.begin(); 
            (it != m_vChunks.end())/* && (xTicksRemaining <= xTicksToWait)  */; it++) {

            _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
            entry = *it; 
                
            if( (_ret = entry->realBuffer == mem) ) {
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
                if( (_wasCurropted = is_chunk_curropted(entry)) ) {
                    std::cout << "[MN_VECTOR_MEMPOOL_CLASS_NAME] entry was corrupted" << std::endl;

                    entry->theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                    entry->theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
                }
#endif
                if(wasCurropted) *wasCurropted = false;

                entry->state = chunk_state::Free;
                memset(entry->theBuffer, 0, m_allocator.size() );

                _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutexAdd);
            }

            if (xTicksToWait != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
            _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);
        } 
        return _ret;
    }

    /**
     * Return the number of chunks in the mempool
     * @return The number of chunks in the mempool
     */ 
    unsigned int size() {
        /*automutx_t lock(m_mutex);*/ 
        return m_vChunks.size(); 
    }
    /**
     * How many elements are marked as used
     * @return The number of elements / chunks are marked as use
     */ 
    unsigned int get_used() {
        /*automutx_t lock(m_mutex);*/
        unsigned int _ret = 0;

        for(auto it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
            if((*it)->state == chunk_state::Used) _ret++;
        }
        return _ret; 
    }
    /**
     * How many elements are marked as free
     * @return The number of elements / chunks are marked as free
     */ 
    unsigned int get_free() {
        /*automutx_t lock(m_mutex);*/ 
        unsigned int _ret = 0;

        for(auto it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
            if((*it)->state == chunk_state::Free) _ret++;
        }

        return _ret;
    }
    /**
     * How many elements are marked as blocked
     * @return The number of elements / chunks are marked as blocked
     */ 
    unsigned int get_blocked() {
        /*automutx_t lock(m_mutex);*/ 
        unsigned int _ret = 0;

        for(auto it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
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
    bool set_blocked(const int id, const bool blocked, unsigned int xTicksToWait = 512) {
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

            if (xTicksToWait != portMAX_DELAY) {
                xTicksRemaining = xTicksEnd - xTaskGetTickCount();
            }
            _MEMPOOL_CLASS_UNLOCK(m_mutexAdd);

            if(_ret) break;
        }
        return _ret;
    }

    /**
     * Get the state of the chunk
     * @param[in] id The id of the chunk
     * @return The state of the chunk
     */ 
    chunk_state get_state(const int id) {
        /*automutx_t lock(m_mutex);*/
        return (id < size) ?  m_vChunks.at(id)->state : chunk_state::NotHandle;
    }

    /**
     * Get the chunk from a given chunk id
     * @param[in] id The id of the chunk
     * 
     * @return The chunk from a given chunk id
     */ 
    chunk_t* get_chunk(const int id) {
        /*automutx_t lock(m_mutex);*/
        return (id < m_vChunks.size()) ? m_vChunks.at(id) : NULL;
    }
    std::vector<chunk_t*> get_chunks() {
        std::vector<chunk_t*> ret;
        m_vChunks.swap(ret);
        return ret;
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

    /**
     * No copyble
     */ 
    basic_mempool_vector& operator=(const basic_mempool_vector&) = delete;
public:
    inline std::string state2name(chunk_state state) {
        return state == chunk_state::Free ? "Free" : 
            (state == chunk_state::Used) ? "Used" : 
            (state == chunk_state::Blocked) ? "Blocked" :
            "NotHandle";
    }

    inline void print_chunk(chunk* chunk) {
        std::cout << "addreoss of chunk @" << chunk 
            << " with size of: " << sizeof(chunk[0]) <<  std::endl;
        std::cout << "address of theBuffer @ " << chunk->theBuffer
            << " with size of: " << sizeof(chunk->theBuffer[0]) <<  std::endl;
        std::cout << "address of realBuffer@ " << chunk->realBuffer 
            << " with size of: " << sizeof(chunk->realBuffer[0]) <<  std::endl;
        
        std::cout << "sate is: " << state2name(chunk->state) << std::endl;
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
        std::cout << "magic start: " << (int)chunk->theMagicGuard[0] << std::endl;
        std::cout << "magic end: " << (int)chunk->theMagicGuard[1] << std::endl;

        if(chunk->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_END && 
           chunk->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START)
            std::cout << "chunk is corrupted" << std::endl;
        else
            std::cout << "chunk is healty" << std::endl;   
#endif      

    }
private:
    std::vector<chunk_t*> m_vChunks;
    unsigned int m_uiElements;

    //mutex_t      m_mutex;
    allocator_t  m_allocator;
};



#endif
