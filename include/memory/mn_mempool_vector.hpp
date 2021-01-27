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

#include "mn_mempool.hpp"


#include <vector>

/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
class basic_vector_mempool_timed : public IMemPool {
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

    basic_vector_mempool_timed(unsigned int nItemSize, unsigned int nElements, unsigned int iAlignment);
    basic_vector_mempool_timed(const basic_vector_mempool_timed&) = delete;
    
    virtual int create(unsigned int xTicksToWait);
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait);
    /**
     * Returns the item back to the pool.
     * 
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait);

    /**
     * Add memory to a basic_vector_mempool.
     * @param itemCount [in] How many more items max do you want to allocate
     * @return Return NO_ERROR when was added and '1' on error
     */
    int add_memory(unsigned int nElements, unsigned int xTicksToWait);

    /**
     * Add memory to a basic_vector_mempool.
     * @param preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param sSizeOf [in] The size of the preallocated memory
     * @return Return NO_ERROR when was added and '1' on error
     */
    int add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait);

    /**
     * No copyble
     */ 
    basic_vector_mempool_timed& operator=(const basic_vector_mempool_timed&) = delete;

    /**
     * get the chunk from a mem pointer (address) 
     * @param mem [in] The pointer (adress) from the memory 
     * 
     * @return the chunk from a mem pointer (address)
     */ 
    chunk_t* get_chunk_from_mem(void* mem);

    /**
     * Return the number of chunks in the mempool
     * @return The number of chunks in the mempool
     */ 
    unsigned int chunk_size();

    /**
     * Return the size of memory they are handle in the pool
     * @return The size of memory they are handle in the pool
     */ 
    unsigned long size();
private:
    basic_vector_mempool();
private:
    std::vector<chunk_t*> m_vChunks;
    mutex_t m_mutex;
};




#endif