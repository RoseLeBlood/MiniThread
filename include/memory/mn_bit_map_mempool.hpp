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

class basic_bitmap_mempool : public IMemPool {
    
};


#endif // MN_THREAD_CONFIG_PREVIEW_FUTURE == MN_THREAD_CONFIG_YES

#endif // _MINLIB_FREE_LIST_MEMPOOL_H_