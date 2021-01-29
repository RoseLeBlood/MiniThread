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

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
    #define MN_VECTOR_SMEMPOOL_CLASS_NAME    basic_simple_mempool_timed
#else
    #define MN_VECTOR_SMEMPOOL_CLASS_NAME    basic_simple_mempool
#endif

#define MN_VECTOR_SMEMPOOL_CLASS_DEF         class MN_VECTOR_SMEMPOOL_CLASS_NAME : public IMemPool
/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
MN_VECTOR_SMEMPOOL_CLASS_DEF {
public:
    struct chunk {
    union {
        struct {
            int* buffer;
            int used : 2;
        };
        int* rawBuffer;
    };
};
public:
    using chunk_t = chunk;
    /**
     * Basic Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     * @param[in] iAlignment The alignment
     */ 
    MN_VECTOR_SMEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements, unsigned int iAlignment);
    MN_VECTOR_SMEMPOOL_CLASS_NAME(const MN_VECTOR_SMEMPOOL_CLASS_NAME&) = delete; ///< no copyable 
    
    /**
     * Create the mempool 
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return - NO_ERROR:
     *         - ERR_MEMPOOL_UNKNOW: 
     */ 
    virtual int create(unsigned int xTicksToWait) override;
    /**
     * Allocate an item from the pool.
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait) override;
    /**
     * Returns the item back to the pool.
     * @param[in] mem The allocated memory to given back te the pool
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait) override;

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] itemCount How many more items max do you want to allocate
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
     */
    virtual int add_memory(unsigned int nElements, unsigned int xTicksToWait) override;

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param[in] sSizeOf [in] The size of the preallocated memory
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
     */
    virtual int add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) override;

    /**
     * Return the size of memory they are handle in the pool
     * @return The size of memory they are handle in the pool
     */ 
    virtual unsigned long size() override;

    /**
     * Is the mempool empty?
     * @return True when all chunks are used (empty) and false when not
     */ 
    virtual bool is_empty() override;

    /**
     * Is the given address handle with a chunk in this buffer
     * @param address The address of the raw memory block
     * 
     * @return If true then handle the adress with this pool, If false then not
     */ 
    virtual bool is_handle(const int address) override;
private:
    MN_VECTOR_MEMPOOL_CLASS_NAME() { }
private:
    std::vector<chunk_t*> m_vChunks;
};

#endif