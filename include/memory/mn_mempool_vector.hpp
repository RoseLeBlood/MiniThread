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
    #define MN_VECTOR_MEMPOOL_CLASS_NAME    basic_vector_mempool_timed
#else
    #define MN_VECTOR_MEMPOOL_CLASS_NAME    basic_vector_mempool
#endif

#define MN_VECTOR_MEMPOOL_CLASS_DEF         class MN_VECTOR_MEMPOOL_CLASS_NAME : public IMemPool
/**
 * A very extendeble mempool for debug and more (timed version)
 * 
 * \ingroup memory
 */ 
MN_VECTOR_MEMPOOL_CLASS_DEF {
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
    /**
     * The memory chunk 
     */ 
    struct chunk {
        void* theBuffer;            /*!< The real buffer */
        char theMagicGuard[2];      /*!< The magicGuard of this chunk */
        chunk_state state;          /*!< The state for a memory chunk */
        bool wasCurropted;          /*!< Was the chunk curropted - only use in free */

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

    /**
     * Basic Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     * @param[in] iAlignment The alignment
     */ 
    MN_VECTOR_MEMPOOL_CLASS_NAME(unsigned int nItemSize, unsigned int nElements, unsigned int iAlignment);
    MN_VECTOR_MEMPOOL_CLASS_NAME(const MN_VECTOR_MEMPOOL_CLASS_NAME&) = delete; ///< no copyable 
    
    /**
     * Create the mempool 
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return - NO_ERROR:
     *         - ERR_MEMPOOL_UNKNOW: 
     */ 
    virtual int create(unsigned int xTicksToWait);
    /**
     * Allocate an item from the pool.
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait);
    /**
     * Returns the item back to the pool.
     * @param[in] mem The allocated memory to given back te the pool
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return if true ther the item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait);

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] itemCount How many more items max do you want to allocate
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
     */
    int add_memory(unsigned int nElements, unsigned int xTicksToWait);

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param[in] sSizeOf [in] The size of the preallocated memory
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
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
    virtual bool is_empty() { ( size() ) == ( get_used() + get_blocked() ); }
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
     * block a chunk 
     * @param[in] address The address of the memory block  to block
     * @param[in] blocked If True than block the chunk and if false then unblock the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return False the chunk is in use and can not block or release. If true then 
     * was block or unblock the chunk 
     */ 
    bool set_adress_blocked(const int address, const bool blocked, unsigned int xTicksToWait);

    /**
     * Get the state of the chunk
     * @param[in] id The id of the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return The state of the chunk
     */ 
    chunk_state get_state(const int id, unsigned int xTicksToWait);
    /**
     * Get the chunk state from the handle memory address, 
     * not the address from the chunk pointer
     * 
     * @param[in] address The address of the raw memory block
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return The chunk state from the handle memory address 
     */ 
    chunk_state get_address_state(const int address, unsigned int xTicksToWait);

    /**
     * Is the given address handle with a chunk in this buffer
     * @param address The address of the raw memory block
     * 
     * @return If true then handle the adress with this pool, If false then not
     */ 
    virtual bool is_handle(const int address) override;

    /**
     * Get the chunk from a given chunk id
     * @param[in] id The id of the chunk
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * 
     * @return The chunk from a given chunk id
     */ 
    chunk_t* get_chunk(const int id, unsigned int xTicksToWait);
    /**
     * Get the chunk from a given buffer address - not the chunk address
     * 
     * @param[in] address The address of the raw memory block
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * 
     * @return The chunk from a given raw memory block
     */ 
    chunk_t* get_chunk_from_address(const int address, unsigned int xTicksToWait);

    /**
     * Is the given chunk_t curropted
     * @param [in] chnk The chunk to cheak
     * @return true the given chunk_t is curropted or false when not
     */ 
    bool is_chunk_curropted(chunk_t* chnk);
private:
    MN_VECTOR_MEMPOOL_CLASS_NAME() { }
private:
    std::vector<chunk_t*> m_vChunks;
};


using vector_mempool_t = MN_VECTOR_MEMPOOL_CLASS_NAME;

#endif