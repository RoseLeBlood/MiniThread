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

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
    #define MN_VECTOR_MEMPOOL_CLASS_NAME    basic_mempool_timed
#else
    #define MN_VECTOR_MEMPOOL_CLASS_NAME    basic_mempool
#endif

#define MN_VECTOR_MEMPOOL_CLASS_DEF         class MN_VECTOR_MEMPOOL_CLASS_NAME : public basic_mempool_interface

/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
MN_VECTOR_MEMPOOL_CLASS_DEF {
public:
    /**
     * The memory chunk 
     */ 
    struct chunk {
        union {
            struct {
                void* theBuffer;            /*!< The real buffer */
#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
                char theMagicGuard[2];      /*!< The magic guard bytes for detect heap memory corruption */
#endif
                chunk_state state;          /*!< The state for a memory chunk */
            };
            void* realBuffer;
        };
        chunk(void* buffer) { 
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
    explicit MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize) 
        : MN_VECTOR_MEMPOOL_CLASS_NAME(nItemSize, 20, MN_THREAD_CONFIG_BASIC_ALIGNMENT) { }

    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     */ 
    MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements)
        : MN_VECTOR_MEMPOOL_CLASS_NAME(nItemSize, nElements, MN_THREAD_CONFIG_BASIC_ALIGNMENT) { }

    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     * @param[in] iAlignment Requested alignment, in bytes.
     */ 
    MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements, unsigned int iAlignment);

    MN_VECTOR_MEMPOOL_CLASS_NAME(const MN_VECTOR_MEMPOOL_CLASS_NAME&) = delete; ///< no copyable 

    /**
     * Allocate an item from the pool.
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait);
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
    int add_memory(unsigned int nElements, unsigned int xTicksToWait);

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param[in] sSizeOf [in] The size of the preallocated memory
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return 0 when was added and '1' on error
     */
    int add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait);

    /**
     * No copyble
     */ 
    MN_VECTOR_MEMPOOL_CLASS_NAME& operator=(const MN_VECTOR_MEMPOOL_CLASS_NAME&) = delete;

    /**
     * Return the number of chunks in the mempool
     * @return The number of chunks in the mempool
     */ 
    unsigned int chunk_size();

    /**
     * Return the size of memory they are handle in the pool
     * @return The size of memory they are handle in the pool
     */ 
    virtual unsigned long size() override;

    /**
     * How many elements are marked as used
     * @return The number of elements / chunks are marked as use
     */ 
    unsigned int get_used();
    /**
     * How many elements are marked as free
     * @return The number of elements / chunks are marked as free
     */ 
    unsigned int get_free();
    /**
     * How many elements are marked as blocked
     * @return The number of elements / chunks are marked as blocked
     */ 
    unsigned int get_blocked();

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
    bool set_blocked(const int id, const bool blocked, unsigned int xTicksToWait);

    /**
     * Get the state of the chunk
     * @param[in] id The id of the chunk
     * @return The state of the chunk
     */ 
    chunk_state get_state(const int id);

    /**
     * Get the chunk from a given chunk id
     * @param[in] id The id of the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * 
     * @return The chunk from a given chunk id
     */ 
    chunk_t* get_chunk(const int id, unsigned int xTicksToWait);

#if MN_THREAD_CONFIG_MEMPOOL_USE_MAGIC == MN_THREAD_CONFIG_YES
    /**
     * Is the given chunk_t curropted
     * @param [in] chnk The chunk to cheak
     * @return true the given chunk_t is curropted or false when not
     */ 
    bool is_chunk_curropted(chunk_t* chnk);
#endif

private:
    std::vector<chunk_t*> m_vChunks;
};


using basic_mempool_t = MN_VECTOR_MEMPOOL_CLASS_NAME;

#endif
