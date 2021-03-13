/*
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef _MINLIB_2ff65886_2d7d_47f2_a507_67ea8a480dfd_H_
#define _MINLIB_2ff65886_2d7d_47f2_a507_67ea8a480dfd_H_

#include "freertos/FreeRTOS.h"

namespace mn {
    namespace memory {
        /**
         * The state for a memory chunk
         */ 
        enum class vmempool_chunk_state {
            Free,                   /*!< The chunk is free and can allocated */
            Used,                   /*!< The chunk is used */
            Blocked,                /*!< The chunk is blocked and can not use */
            NotHandle = 99          /*!< Return when the address not handle with this mempool */
        };

        #define VMEM_CHUNK_OWNER_FREE 1
        #define VMEM_CHUNK_ALL_FREE   0

        template <int TBufferSize, class TALLOCATOR >
        class vmempool_chunk {
            MNALLOC_OBJECT(TALLOCATOR);

            static const size_t XUnionSize = mn::value2size_raw<char[2]>::size +  
                                             mn::value2size_raw<vmempool_chunk_state>::size +
                                             TBufferSize;
        public:
            union {
                struct {
                    void* theBuffer;
                    char theMagicGuard[2];              /*!< The magic guard bytes for detect heap memory corruption */
                    vmempool_chunk_state state;          /*!< The state for a memory chunk */
                    
                };
                void* theRealBuffer;
            };

            vmempool_chunk() 
                : theBuffer(0), 
                  theMagicGuard { MN_THREAD_CONFIG_MEMPOOL_MAGIC_START,  MN_THREAD_CONFIG_MEMPOOL_MAGIC_END},
                  state(vmempool_chunk_state::Free),
                  m_ownerTask(0),
                  m_cfree(VMEM_CHUNK_ALL_FREE) {  theBuffer = m_acObject.alloc(TBufferSize, __UINT32_MAX__ ); }

            vmempool_chunk(void* address) 
                :theBuffer(address), 
                 theMagicGuard { MN_THREAD_CONFIG_MEMPOOL_MAGIC_START,  MN_THREAD_CONFIG_MEMPOOL_MAGIC_END},
                 state(vmempool_chunk_state::Free),
                 m_ownerTask(0),
                 m_cfree(VMEM_CHUNK_ALL_FREE) { }
            
            

            void set_owner(uint32_t task) { 
                if(state == vmempool_chunk_state::Free)
                    m_ownerTask = task; 
            }
            bool can_free(int task) { 
                return (m_cfree == 1) ? m_ownerTask == task : true; 
            }
            vmempool_chunk_state get_state() { 
                return state; }

            void set_state(vmempool_chunk_state s) { 
                state = s; }

            void* construct(bool oFreedSelf, int taskID) {
                void* _retBuffer = NULL;

                if(state == vmempool_chunk_state::Free) {
                    set_owner( taskID );
                    set_free( (oFreedSelf) ? VMEM_CHUNK_OWNER_FREE : VMEM_CHUNK_ALL_FREE );

                    _retBuffer = theBuffer;
                }
                return _retBuffer;
            }
            bool deconstruct(void* buffer, int taskID, bool& wasCorrubted) {
                if(m_cfree == VMEM_CHUNK_OWNER_FREE && m_ownerTask != taskID) return false;

                if(buffer == theBuffer) {
                    wasCorrubted = is_corupted();

                    theMagicGuard[0] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_START;
                    theMagicGuard[1] = MN_THREAD_CONFIG_MEMPOOL_MAGIC_END;

                    // maked the state now as free
                    state = vmempool_chunk_state::Free;
                    // and fill the buffer with nulls - arrase the old informations
                    memset(theBuffer, 0, TBufferSize );

                    return true;
                }
                return false;
            }
            

            /**
             * @brief Can only owner task mark free this chunk?
             * 
             * @param t When true then can only the owner mark free the chunk
             */
            void set_free(bool t) { m_cfree = t ? 1 : 0; }

            bool is_corupted() {
                return !(theMagicGuard[0] == MN_THREAD_CONFIG_MEMPOOL_MAGIC_START &&
                         theMagicGuard[1] == MN_THREAD_CONFIG_MEMPOOL_MAGIC_END);
            }

            vmempool_chunk(const vmempool_chunk& other) = delete;
            vmempool_chunk& operator = (const vmempool_chunk& other) = delete;

            inline void print() {
                vmempool_chunk* chunk = this;

                std::cout << "addreoss of chunk @" << chunk 
                    << " with size of: " << sizeof(chunk[0]) <<  std::endl;
                std::cout << "address of theBuffer @ " << chunk->theBuffer
                    << " with size of: " << sizeof(chunk->theBuffer) <<  std::endl;
                std::cout << "address of realBuffer@ " << chunk->theRealBuffer 
                    << " with size of: " << sizeof(chunk->theRealBuffer) <<  std::endl;
                
            
                std::cout << "magic start: " << (int)chunk->theMagicGuard[0] << std::endl;
                std::cout << "magic end: " << (int)chunk->theMagicGuard[1] << std::endl;

                if(chunk->theMagicGuard[1] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_END && 
                chunk->theMagicGuard[0] != MN_THREAD_CONFIG_MEMPOOL_MAGIC_START)
                    std::cout << "chunk is corrupted" << std::endl;
                else
                    std::cout << "chunk is healty" << std::endl;  
            }
        protected:
            /**
             * @brief The task that alloc this chunk 
             */
            uint32_t m_ownerTask; 
            /**
             * @brief Can only owner task free this chunk? 1 = Yes, 0 = all can free
             */
            char     m_cfree : 2; 
        };
        MNALLOC_OBJECT_DTWO(vmempool_chunk, int, TBufferSize, class, TALLOCATOR);
    }
}
#endif