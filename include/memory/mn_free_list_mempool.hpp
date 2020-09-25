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

// Move in the future to mn_config.h
#define MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START   0x6d //109
#define MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END     0xa8 //168 

// Move in the future to mn_error.h
#define ERR_MEMPOOL_OK                    NO_ERROR
#define ERR_MEMPOOL_ALREADYINIT           -1
#define ERR_MEMPOOL_UNKNOW                -2
/** 
 * A free list mempool, based of a mempool toturial
 * 
 * @note the block sizes can be determined by the user based on approximating 
 * the sizes of object types used in the application 
 * The MemoryPool can be initialised partially or completely , based on the 
 * need and the frequency of occurence of certain types of object; 
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
                char* memBlock; 
                /**
                 * The two guard bytes for detect heap memory corruption, ie,  when someone 
                 * writes beyond the boundary via  memcpy or memset functions
                 */ 
                char[2] memGuard; //0xde 0xad; 
                /**
                 * The size of the memBlock
                 */ 
                char sizeofBlock; 
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
     * @param nItemSize The size of a 
     */ 
    basic_free_list_mempool(unsigned int nItemSize, unsigned int nElements, unsigned int uiAlignment);
    basic_free_list_mempool(const basic_free_list_mempool&) = delete;
    
    /**
     *  To correctly delete a Memory Pool, we'd have to guarantee that
     *  all allocations had been returned to us. We side step this issue
     *  as well as all the associated overhead with supporting this by
     *  not allowing destructors.
     */
    ~basic_free_list_mempool() = delete;

    virtual int create();

    virtual int add_memory(unsigned int nElements);
    virtual int add_memory(void* preMemory, unsigned int nSize);
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty or the size too big
     */ 
    virtual void* allocate();
    /**
     * Free the memory
     * 
     * @return true Free the memory, false If not
     */ 
    virtual bool  free(void* mem);

    int           get_size();
    bool          is_empty();

private:
    unsigned int              m_uiAlignment

    std::list<memObject*>     m_lBytePtrList;
    std::vector<void*>        m_vMemoryPoolList;

    mutex_t                   m_nMutex;
    bool                      m_bCreated;
};
#endif

#endif