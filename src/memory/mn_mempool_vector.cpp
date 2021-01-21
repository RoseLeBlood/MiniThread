/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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

#include "memory/mn_mempool_vector.hpp";
#include <malloc.h>
#include <iostream>

basic_vector_mempool::basic_vector_mempool(unsigned int nItemSize, unsigned int nElements)
        : IMemPool(nItemSize, nElements) { 
    m_bInit = internal_create_chunks();
}
void* basic_vector_mempool::allocate(unsigned int xTicksToWait) {
    if(!m_bInit) return nullptr;
    void* buffer = nullptr;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); it != m_vChunks.end(); it++) {
        chunk_t* entry = *it;

        if(entry->state == chunk_state::Free) {
            entry->state = chunk_state::Used;

            buffer = entry->theBuffer;
            break;
        }
    }
    return buffer;
}
bool basic_vector_mempool::free(void* mem, unsigned int xTicksToWait) {
    if(mem == NULL) return false;
    bool _ret = false;

    for(std::vector<chunk_t*>::iterator it = m_vChunks.begin(); it != m_vChunks.end(); it++) {
        chunk_t* entry = *it; 

        if(entry->theBuffer == mem && entry->state != chunk_state::Blocked) {
            entry->wasCurropted = 
                (entry->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
                 entry->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START);

            mem = NULL;
            entry->state = chunk_state::Free;

            if(entry->wasCurropted) {
                std::cout << "[basic_vector_mempool] entry was corrupted" << std::endl;

                entry->theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                entry->theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;
            }
            _ret = true;
            break;
        } 
    }
    return _ret;
}

bool basic_vector_mempool::internal_create_chunks() {
   for(unsigned int i = 0; i < m_uiElements; i++) {
        void* buffer = malloc(m_uiItemSize);
        m_vChunks.push_back( new chunk_t(buffer) );
    }
}