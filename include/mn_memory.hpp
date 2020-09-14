/** This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2020 Amber-Sophia Schroeck
 * 
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
**/
#ifndef MINLIB_ESP32_MEMORY_
#define MINLIB_ESP32_MEMORY_

#include "mn_mutex.hpp"

class basic_memory_pool {
public:
    /**
     *  create a basic_memory_pool.
     *
     *  This constructor uses the system malloc to actually obtain
     *  the memory.
     *
     *  @param itemSize How big is each item you want to allocate.
     *  @param itemCount How many items max do you want to allocate
     *      at once.
     *  @param Alignment Power of 2 value denoting on which address boundary the
     *      memory will be aligned to. Must be at least sizeof(unsigned char *).
     */
    int create(int itemSize, int itemCount, int alignment);

    /**
     * create a basic_memory_pool.
     *
     *  @param itemSize How big is each item you want to allocate.
     *  @param preallocatedMemory Pointer to the preallocated memory
     *  you are dedicating to this pool.
     *  @param preallocatedMemorySize How big is the buffer you are
     *  passing in.
     *  @param Alignment Power of 2 value denoting on which address boundary the
     *      memory will be aligned to. Must be at least sizeof(unsigned char *).
     */
    int create(int itemSize, void *preallocatedMemory,
            int preallocatedMemorySize, int alignment);

    /**
     *  Allows you to add memory to a basic_memory_pool.
     *
     *  @param itemCount How many more items max do you want to allocate
     */
    int add_memory(int iCount);

    /**
     *  Allows you to add memory to a basic_memory_pool.
     *
     *  Items will be the same size as you initially asked for.
     *
     *  @param pMemory Pointer to the preallocated memory
     *  you are dedicating to this pool.
     *  @param size How big is the buffer you are
     *  passing in.
     */
    int add_memory(void *pMemory, int size);

    /**
     *  Allocate an item from the pool.
     *
     *  @return Pointer of the memory or NULL if the pool is empty.
     */
    void* allocate();

    /**
     *  Returns the item back to it's pool.
     *
     *  @note There is no checking that the item is actually
     *  valid to be returned to this pool.
     */
    void free(void *item);
private:
    /**
     *  Adjusts and validates the alignment argument
     */
    bool calc_valid_aligment();

    /**
     *  Calculate the true item size, based on alignment.
     */
    void calc_item_size();
private:
    /**
     *  Standard Mutex to allow thread safety.
     */
    mutex_t *m_pLock;

    /**
     *  Save the item size for additions.
     */
    int m_iItemSize;

    /**
     *  The overall alignment of an item.
     */
    int m_iAlignment;

    /**
     *  All of the real work is done with STL lists.
     */
    std::list<void *> m_plFreeItems;
};

using memory_pool = basic_memory_pool;

#endif