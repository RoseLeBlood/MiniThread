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
    IMemPool(unsigned int nItemSize, unsigned int nElements)
        : m_uiItemSize(nItemSize), m_uiElements(nElements) { 
            if(m_uiItemSize % 4 != 0) m_uiItemSize += 4 - (m_uiItemSize % 4); 
    }
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
protected:
    unsigned int m_uiItemSize;
    unsigned int m_uiElements;
};

#endif
