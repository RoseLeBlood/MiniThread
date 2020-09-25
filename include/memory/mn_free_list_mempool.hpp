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

/** 
 * A free list mempool, based of the xy mempool toturial
 * 
 * @note the block sizes can be determined by the user based on approximating 
 * the sizes of object types used in the application 
 * The MemoryPool can be initialised partially or completely , based on the 
 * need and the frequency of occurence of certain types of object; 
 * each block shall contain 4 extra bytes to store metadata information 
 * One such information is that indicating whether this block is free 
 * or not. Another tentative information could be guard bytes which detect 
 * heap memory corruption , ie , when someone writes beyond the boundary 
 * of an object accidentally via  memcpy or memset functions.In our 
 * implementation everyblock is guarded by 4 bytes.The secound two 
 * bytes of these four are marked with  special characters 0x6d,0xa8.
 * The next byte will contain the size of block and the 
 * of the byte shall indicate wheteher this block is free or available.
 */
class basic_free_list_mempool : public IMemPool {
    /**
     * The Intrnal MemBlock
     */ 
    struct memObject {
        union {
            struct {
                char* memBlock; //(uiItemSize) + char
                char guardStart; //0xde;
                char guardEnd; //0xad;
                char sizeofBlock;
                char blockAvaible : 1;
            };
            void* raw_memObject;
        };
    };

public:
    basic_free_list_mempool() { }
    /**
     *  To correctly delete a Memory Pool, we'd have to guarantee that
     *  all allocations had been returned to us. We side step this issue
     *  as well as all the associated overhead with supporting this by
     *  not allowing destructors.
     */
    ~basic_free_list_mempool() = delete;

    basic_free_list_mempool(unsigned int nItemSize, unsigned int nElements);

    virtual int create();

    virtual int add_memory(unsigned int nItemSize, unsigned int nElements);
    virtual int add_memory(void* preMemory, unsigned int nSize);
    /**
     * Allocate an item from the pool.
     * @return Pointer of the memory or NULL if the pool is empty or the size too big
     */ 
    virtual void* allocate(unsigned int size);
    /**
     * Returns the item back to the pool.
     * 
     * @note There is no checking that the item is actually
     *  valid to be returned to this pool.
     * 
     * @return true if The item back to it's pool, false If not
     */ 
    virtual bool  free(void* mem);
private:
    unsigned int m_uiItemSize;
    unsigned int m_uiElements;
    unsigned int m_uiSizeOver;

    std::list<memObject*>     m_lBytePtrList;
    std::vector<void*>        m_vMemoryPoolList;
};
#endif

#endif