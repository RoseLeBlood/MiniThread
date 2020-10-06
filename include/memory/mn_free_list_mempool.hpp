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
#ifndef _MINLIB_FREE_LIST_MEMPOOL_H_
#define _MINLIB_FREE_LIST_MEMPOOL_H_

#include "mn_mempool.hpp"

#if MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES

#include <vector>
#include <set>
#include <list>

#include "../mn_mutex.hpp"

/** 
 * A free list mempool, based of a mempool toturial
 * 
 * @note the block sizes can be determined by the user based on approximating 
 * the sizes of object types used in the application 
 * The MemoryPool can be initialised partially or completely , based on the 
 * need and the frequency of occurence of certain types of object; 
 * 
 * @ingroup preview-memory
 */
class basic_free_list_mempool : public IMemPool {
    /**
     * The Intrnal MemBlock
     */ 
    struct memObject {
        union {
            struct {
                /**
                 * The real memory
                 */ 
                char* addr; 
                /**
                 * The two guard bytes for detect heap memory corruption, ie, when someone 
                 * writes beyond the boundary via  memcpy or memset functions
                 */ 
                char memGuard[2]; //0xde 0xad; 
                /**
                 * The size of the memBlock
                 */ 
                char dim; 
                /**
                 * This byte shall indicate wheteher this block is free or not
                 */ 
                char blockAvaible : 1; 
            };
            void* raw_memObject; 
        };
    };
public:
    /**
     * 
     * @param nItemSize The maximum size of the stored item
     * @param nElements The maximum number of items stored in the pool
     * @param uiAlignment Power of 2 value denoting on which address boundary the memory will be aligned to.
     */ 
    basic_free_list_mempool(unsigned int nItemSize, unsigned int nElements, unsigned int uiAlignment);
    
    /**
     * Our Destrutor
     */ 
    ~basic_free_list_mempool()  {
        destroy();
    }

    /**
     * Create the mempool
     * 
     * @return 'ERR_MEMPOOL_OK' The pool are created,
     *         'ERR_MEMPOOL_ALREADYINIT' The pool are already created,
     *         'ERR_MEMPOOL_BADALIGNMENT' Bad alignment, please updatet it, whit set_alignment()
     *         'ERR_MEMPOOL_UNKNOW', The Mempool can't create, out of memory or other problem
     */ 
    virtual int create();

    /**
     * Deleted all memory in the pool
     */ 
    virtual void destroy();

    virtual int add_memory(unsigned int nElements);
    virtual int add_memory(void* preMemory, unsigned int nSize);
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty or the size too big
     */ 
    virtual void* allocate(TickType_t xTicksToWait);
    /**
     * Free the memory
     * 
     * @return true Free the memory, false If not
     */ 
    virtual bool  free(void* mem, TickType_t xTicksToWait);

    /**
     * Get the stored n elements of objects in list
     * 
     * @return The stored n elements of objects in list
     */ 
    int           get_size();

    /**
     * Is the mempool empty
     * 
     * @return true If the mempool empty, false If not 
     */ 
    bool          is_empty();

    /**
     * How many items / blocks are marked free in this pool
     * 
     * @return Number of items / blocks are marked free 
     */ 
    unsigned int  get_free_items();
    /**
     * How many items / bloks are marked used in this pool
     * 
     * @return Number of items / blocks are marked used 
     */ 
    unsigned int  get_used();

    #if MN_THREAD_CONFIG_DEBUG == MN_THREAD_CONFIG_YES
    /**
     * Get the memObject from allocated memory object 
     * 
     * @param The allocated memory object
     * @return The memObject from allocated memory object
     * 
     * @note Return a object with raw_memObject = NULL, when object in list not found
     */ 
    memObject     get_mem_object(void* mem);
    #endif

    /**
     * Set the alignment, only used befor the mempool sucessfull created 
     * and get the real used algnment.
     * 
     * @param alignment The new used alignment
     * 
     * @return The real used algnment, becourse 0 on error, the given algnment is to small 
     */ 
    int set_alignment(uint8_t uiAlignment);

    basic_free_list_mempool(const basic_free_list_mempool&) = delete;
    basic_free_list_mempool& operator=(const basic_free_list_mempool&) = delete;
private:
    /**
     * Get the next free item / block from the pool
     * 
     * @return The next free item / block
     */ 
    memObject*    get_free_block(TickType_t xTicksToWait);
private:
    mutex_t                   m_nMutex;
    
    unsigned int              m_uiAlignment;

    std::vector<memObject*>   m_lBytePtrList;
    std::vector<void*>        m_vMemoryPoolList;

    
    bool                      m_bCreated;
};

using freelist_mempool_t = basic_free_list_mempool;
#endif

#endif