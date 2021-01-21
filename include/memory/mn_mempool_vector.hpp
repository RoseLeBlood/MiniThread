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
#ifndef _MINLIB_SIMPLE_MEMPOOL_H_
#define _MINLIB_SIMPLE_MEMPOOL_H_

#include "mn_mempool.hpp"
#include <vector>

/**
 * A very simple mempool
 * 
 * \ingroup memory
 */ 
class basic_vector_mempool : public IMemPool {
public:
    enum class chunk_state {
        Free,
        Used,
        Blocked
    };
    struct chunk {
        void* theBuffer;
        char theMagicGuard[2];
        chunk_state state;
        bool wasCurropted;

        chunk(void* buffer)
            : theBuffer(buffer), 
            theMagicGuard( {0} ),
            state(chunk_state::Free),
            wasCurropted(false) { 
                theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
            }
    };
    
public:
    using chunk_t = chunk;

    basic_vector_mempool(unsigned int nItemSize, unsigned int nElements);
    basic_vector_mempool(const basic_vector_mempool&) = delete;
    
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait);
    /**
     * Returns the item back to the pool.
     * 
     * @note There is no checking that the item is actually
     *  valid to be returned to this pool.
     * 
     * @return true if The item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait);

    basic_vector_mempool& operator=(const basic_vector_mempool&) = delete;
private:
    basic_vector_mempool();

    bool internal_create_chunks();
private:
    std::vector<chunk_t*> m_vChunks;
    bool    m_bInit;

};


#endif