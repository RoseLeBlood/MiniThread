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

#include "../mn_allocator.hpp"

#include "mn_basic_mempool.hpp"

namespace mn {
    namespace memory {
        
    #if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32
        template <typename TType, int nElements>
        using basic_mempool_spiram_t = basic_mempool_vector<TType, nElements, 
            basic_mutex, basic_allocator_spiram  >;
    #endif

        template <typename TType, int nElements>
        using basic_mempool_system_t = basic_mempool_vector<TType, nElements>;

        template <typename TType, int nElements>
        using basic_mempool_t = basic_mempool_system_t<TType, nElements>;

    }
}



#endif
