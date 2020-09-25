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
#include "mn_mempool.hpp"

#if MN_THREAD_CONFIG_PREVIEW_DEV_ACTIVATE == MN_THREAD_CONFIG_YES
#warning("use on your own risk - not for production use")

/*
class IMemObject {
public:
    using MemObject_t = void;

    inline void* operator new(size_t size) {
        return m_mempool.allocate(size);
    }
    inline void  operator delete(void* object) {
      m_mempool.free(object);
    }
protected:
    MemObject_t m_mempool;
};
*/

class IMempoolObject {

};

class basic_mempool_object_factory {

};

using mempool_factory_t = basic_mempool_object_factory;

#endif