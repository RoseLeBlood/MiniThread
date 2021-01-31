/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef _MINLIB_IMEMPOOL_INTERFACE_H_
#define _MINLIB_IMEMPOOL_INTERFACE_H_

#include "../mn_config.hpp"
#include "../mn_error.hpp"
#include "../mn_mutex.hpp"


/**
 * Interface for all mempools in this library
 * This is an abstract base class.
 * To use this, you need to subclass it. All of your MemPools should
 * be derived from the ILockObject class. Then implement the virtual allocate
 * and free functions.
 * 
 * \ingroup memory
 */ 
class IMemPool {
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
public:
    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     */ 
    IMemPool(unsigned int nItemSize) 
        : IMemPool(nItemSize, MN_THREAD_CONFIG_MEMPOOL_ELEMENTS, MN_THREAD_CONFIG_BASIC_ALIGNMENT) { }

    /**
     * Ctor 
     * @param[in] nItemSize The size of a item
     * @param[in] nElements How many elements are handle with the  pool
     * @param[in] iAlignment The alignment of the memory
     */ 
    IMemPool(unsigned int nItemSize, unsigned int nElements, unsigned int iAlignment)
        : m_uiItemSize(nItemSize), m_uiElements(nElements), m_iAlignment(iAlignment) { 
    }
    /**
     * Create the mempool 
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return - ERR_MEMPOOL_OK: Mempool are created
     *         - ERR_MEMPOOL_BADALIGNMENT: The given Alignment not work
     *         - ERR_MEMPOOL_CREATE: Can't create the mempool 
     */ 
    virtual int create(unsigned int xTicksToWait);

    /**
     * Create the mempool
     * @param[in] nElements How many elements are store in the pool
     * @param[in] iAlignment The alignment
     * @return - ERR_MEMPOOL_OK: Mempool are created
     *         - ERR_MEMPOOL_BADALIGNMENT: The given Alignment not work
     *         - ERR_MEMPOOL_CREATE: Can't create the mempool 
     */ 
    virtual int create(unsigned int nElements, unsigned int iAlignment, unsigned int xTicksToWait);
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty.
     */ 
    virtual void* allocate(unsigned int xTicksToWait) = 0;
    /**
     * Returns the item back to the pool.
     * 
     * @note There is no checking that the item is actually
     *  valid to be returned to this pool.
     * 
     * @return true if The item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem, unsigned int xTicksToWait) = 0;

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] itemCount How many more items max do you want to allocate
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
     */
    virtual int add_memory(unsigned int nElements, unsigned int xTicksToWait) = 0;

    /**
     * Add memory to a basic_vector_mempool.
     * @param[in] preallocatedMemory [in] The pointer of the preallocated memory to add.
     * @param[in] sSizeOf [in] The size of the preallocated memory
     * @param[in] xTicksToWait How long to wait to get until giving up.
     * @return Return NO_ERROR when was added and '1' on error
     */
    virtual int add_memory( void *preallocatedMemory, size_t sSizeOf, unsigned int xTicksToWait) = 0;

    /**
     * Return the size of memory they are handle in the pool
     * @return The size of memory they are handle in the pool
     */ 
    virtual unsigned long size() = 0;

    /**
     * Is the mempool empty?
     * @return True the mempool is empty and false when not
     */ 
    virtual bool is_empty() = 0;
protected:
    bool calcAligentAndSize();
protected:
    unsigned int m_uiItemSize;
    unsigned int m_uiElements;
    unsigned int m_iAlignment;

    mutex_t      m_mutex;
};

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES 
    void* malloc_timed(unsigned long size, unsigned int xTicksToWait);
    void* realloc_timed(void* addr, unsigned long size, unsigned int xTicksToWait);
    void* calloc_timed(unsigned long nmemb, unsigned long size, unsigned int xTicksToWait);
    void* memcpy_timed(void* dest, const void* src, unsigned int size, unsigned int xTicksToWait);
    void* memset_timed(void* addr, int set, unsigned int size, unsigned int xTicksToWait);
#else
    #define malloc_timed(size, xTicksToWait)            malloc(size)
    #define realloc_timed(addr, size, xTicksToWait)     realloc(addr, size)
    #define calloc_timed(nmemb, size, xTicksToWait)     calloc(nmemb, size)
    #define memcpy_timed(dest, src, size, xTicksToWait)  memcpy(dest, src, size)
    #define memset_timed(addr, set, size, xTicksToWait); memset(addr, set, size)
#endif

#if MN_THREAD_CONFIG_MEMPOOL_USETIMED == MN_THREAD_CONFIG_YES
    #define MALLOC_TIMED(uiItemSize, nElements, xTicksToWait) \
    malloc_timed(uiItemSize * nElements, xTicksToWait)
#else
    #define MALLOC_TIMED(uiItemSize, nElements, xTicksToWait) \
    malloc(uiItemSize * nElements, xTicksToWait)
#endif

template<typename T>
inline T* zeroset(T* addr, unsigned long size, unsigned int xTicksToWait) {
    return (T*)memset_timed(addr, 0, size, xTicksToWait);
} 

template<>
inline void* zeroset(void* addr, unsigned long size, unsigned int xTicksToWait) {
    return zeroset<void>(addr, size, xTicksToWait);
} 

#include "mn_mempool_vector.hpp"


#endif
