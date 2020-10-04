/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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

/**
 * config items for next major version
 * @ingroup preview
 */

#ifndef __MINLIB_MNTHREAD_PREVIEW_CONFIG_H__
#define __MINLIB_MNTHREAD_PREVIEW_CONFIG_H__

#include "mn_config.hpp"

// INFO: Defines for all preeview futures for the next major version
//=================================================================

#ifndef MN_THREAD_CONFIG_DEBUG
    #define  MN_THREAD_CONFIG_DEBUG     MN_THREAD_CONFIG_YES
#endif

#ifndef MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING  
    /// Using pre defines (usings) for basic types for the shared objects
    #define MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING MN_THREAD_CONFIG_YES
#endif //MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING


#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START
    ///The start magic guard byte for detect heap memory corruption
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_START   0x6d //109
#endif

#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END
    ///The end magic guard byte for detect heap memory corruption
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_MAGIC_END     0xa8 //168 
#endif



/** Ab hier nichts verändern | DO NOT EDIT AFTER THIS LINE!!!
 * =================================================================
 */

#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE
    ///The mempool object is not used
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_FREE 1
#endif

#ifndef MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED
    ///The mempool object is used, not free
    #define MN_THREAD_CONFIG_FREELIST_MEMPOOL_USED 0
#endif

#endif //__MINLIB_MNTHREAD_PREVIEW_CONFIG_H__