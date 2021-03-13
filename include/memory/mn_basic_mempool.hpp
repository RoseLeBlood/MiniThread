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
#ifndef _MINLIB_VECTOR_MEMPOOL_H_
#define _MINLIB_VECTOR_MEMPOOL_H_


#include <malloc.h>
#include <iostream>
#include "../mn_autolock.hpp"
#include "../mn_allocator.hpp"
#include <string.h>
#include <vector>

#include "../mn_task.hpp"
#include "mn_basic_mempool_chunk.hpp"

#define _MEMPOOL_CLASS_LOCK(Mutex, xTicksRemaining) if(Mutex.lock(xTicksRemaining) != NO_ERROR) break;
#define _MEMPOOL_CLASS_UNLOCK(Mutex) { Mutex.unlock(); }
#define _MEMPOOL_CLASS_UNLOCK_BREAK(Mutex) { Mutex.unlock(); break; }


namespace mn {
    namespace memory {
        /**
        * A very extendeble mempool for debug and more (timed version)
        * 
        * \ingroup memory
        */ 
        template <typename TType, int nElements, typename TMUTEX = basic_mutex, class TALLOCATOR = default_allocator_t>
        class basic_mempool_vector {
        public:
            using chunk_type = vmempool_chunk<sizeof(TType), TALLOCATOR>;
            using chunk_reference = chunk_type&;
            using chunk_pointer = chunk_type*;

            using value_type = TType;
            using pointer = TType*;

            using allocator_type = TALLOCATOR;
            using vector_type = std::vector<chunk_pointer>;
            using self_type = basic_mempool_vector<TType, nElements, TMUTEX, TALLOCATOR>;
            using lock_type = TMUTEX;
            using lock_gurd_type = basic_autolock<TMUTEX>;
            /**
            * Ctor 
            */ 
            basic_mempool_vector() {  }
            basic_mempool_vector(const basic_mempool_vector&) = delete; ///< no copyable 

            /**
            * Create the mempool 
            * @param[in] xTicksToWait How long to wait to get until giving up.
            * @return - ERR_MEMPOOL_OK: Mempool are created
            *         - ERR_MEMPOOL_CREATE: Can't create the mempool 
            *         - ERR_MEMPOOL_MIN: Can not create the pool witd all elements, 
            *          call size for cheak the real size of pool
            */ 
            virtual int create(unsigned long xTicksToWait) {  
                lock_gurd_type lock(m_mutex); 

                int _retError = ERR_MEMPOOL_OK;
            
                for(int i = 0; i < nElements; i++) {
                    m_vChunks.push_back(new chunk_type() );
                }
            
                return m_vChunks.size() == nElements ? _retError : ERR_MEMPOOL_CREATE;
            }

            virtual bool add_memory(unsigned int elements) {
                lock_gurd_type lock(m_mutex); 

                size_t _oldEle = m_vChunks.size();

                for(int i = 0; i < elements; i++) {
                    m_vChunks.push_back(new chunk_type() );
                }

                return _oldEle < m_vChunks.size();
            }
            
            /**
            * Allocate an item from the pool.
            * @param[in] oFreedSelf Can the allocated chunk only deallocated from allocated task?
            * @param[in] xTicksToWait How long to wait to get until giving up.
            * @return Pointer of the memory or NULL if the pool is empty.
            */ 
            virtual pointer allocate(bool oFreedSelf = false, unsigned long xTicksToWait = portMAX_DELAY) {
                if(m_vChunks.size() == 0) { return NULL; }

                TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
                TickType_t xTicksRemaining = xTicksToWait;

                pointer buffer = nullptr;

                for(auto it = m_vChunks.begin(); 
                    (it != m_vChunks.end() && (xTicksRemaining <= xTicksToWait) ); it++) {

                    _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
                    {
                        chunk_type* entry = *it;

                        basic_task* task = basic_task::get_self();

                        if(task)
                            buffer = static_cast<pointer>( entry->construct(oFreedSelf, task->get_id() ) );
                        else 
                            buffer = static_cast<pointer>( entry->construct(false, 0 ) );

                        if(buffer != nullptr) {
                            _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutex);
                        }

                    }
                    if (xTicksToWait != portMAX_DELAY) {
                        xTicksRemaining = xTicksEnd - xTaskGetTickCount();
                    }
                    _MEMPOOL_CLASS_UNLOCK(m_mutex);
                }
                
                return buffer;
            }

            /**
            * Returns the item back to the pool.
            * @param[in]  mem The allocated memory to given back te the pool
            * @param[in]  xTicksToWait How long to wait to get until giving up.
            * @param[out] wasCurropted A pointer of a bool var 
            * @return if true ther the item back to it's pool, false If not
            */ 
            virtual bool  free(pointer mem, bool* wasCurropted = NULL, unsigned long xTicksToWait = portMAX_DELAY) {
                if(m_vChunks.size() == 0) { return false; }
                if(mem == NULL) { return false; }

                TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
                TickType_t xTicksRemaining = xTicksToWait;
                bool _ret = false, _wasCurropted = false;

                basic_task* task = NULL;

                for(auto it = m_vChunks.begin(); 
                    (it != m_vChunks.end() ) && (xTicksRemaining <= xTicksToWait); it++) {

                    _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);
                    task = basic_task::get_self();

                   
                    if( (*it)->deconstruct(mem, task->get_id(), _wasCurropted) ) {
                        if(wasCurropted) *wasCurropted = _wasCurropted;
                        _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutex);
                        _ret = true;
                    }

                    if (xTicksToWait != portMAX_DELAY) {
                        xTicksRemaining = xTicksEnd - xTaskGetTickCount();
                    }
                    _MEMPOOL_CLASS_UNLOCK(m_mutex);
                } 
                return _ret;
            }

            /**
            * Return the number of chunks in the mempool
            * @return The number of chunks in the mempool
            */ 
            unsigned int size() {
                lock_gurd_type lock(m_mutex); 
                return m_vChunks.size(); 
            }
            /**
            * How many elements are marked as used
            * @return The number of elements / chunks are marked as use
            */ 
            unsigned int get_used() { return get_num_of_state<vmempool_chunk_state::Used>(); }
            /**
            * How many elements are marked as free
            * @return The number of elements / chunks are marked as free
            */ 
            unsigned int get_free() { return get_num_of_state<vmempool_chunk_state::Free>(); }
            /**
            * How many elements are marked as blocked
            * @return The number of elements / chunks are marked as blocked
            */ 
            unsigned int get_blocked() { return get_num_of_state<vmempool_chunk_state::Blocked>(); }

            template<vmempool_chunk_state state> 
            int get_num_of_state() {
                lock_gurd_type lock(m_mutex); 
                unsigned int _ret = 0;

                for(auto it = m_vChunks.begin(); (it != m_vChunks.end()) ; it++ ) {
                    if((*it)->state == state) _ret++;
                }
                return _ret;
            }

            /**
            * Is the mempool empty?
            * @return True when all chunks are used (empty) and false when not
            */ 
            virtual bool is_empty() { return ( size() ) == ( get_used() + get_blocked() ); }
            /**
            * block a chunk 
            * @param[in] id The id of the chunk
            * @param[in] blocked If True than block the chunk and if false then unblock the chunk
            * @param[in] xTicksToWait How long to wait to get until giving up.
            * @return False the chunk is in use and can not block or release. If true then 
            * was block or unblock the chunk 
            */ 
            bool set_blocked(const int id, const bool blocked, unsigned long xTicksToWait = 512) {
                TickType_t xTicksEnd = xTaskGetTickCount() + xTicksToWait;
                TickType_t xTicksRemaining = xTicksToWait;
                bool _ret = false;

                while((xTicksRemaining <= xTicksToWait)) {
                    _MEMPOOL_CLASS_LOCK(m_mutex, xTicksRemaining);

                    if( (m_vChunks.at(id)->get_state() != vmempool_chunk_state::Used) ) {
                        
                        m_vChunks.at(id)->set_state(blocked ? 
                            vmempool_chunk_state::Blocked : 
                            vmempool_chunk_state::Free); 
                         
                        _ret = true;
                        _MEMPOOL_CLASS_UNLOCK_BREAK(m_mutex);
                    }

                    if (xTicksToWait != portMAX_DELAY) {
                        xTicksRemaining = xTicksEnd - xTaskGetTickCount();
                    }
                    _MEMPOOL_CLASS_UNLOCK(m_mutex);
                }
                return _ret;
            }

            /**
            * Get the state of the chunk
            * @param[in] id The id of the chunk
            * @return The state of the chunk
            */ 
            vmempool_chunk_state get_state(const int id) {
                lock_gurd_type lock(m_mutex);
                return (id < size) ?  m_vChunks.at(id)->state : vmempool_chunk_state::NotHandle;
            }

            /**
            * Get the chunk from a given chunk id
            * @param[in] id The id of the chunk
            * 
            * @return The chunk from a given chunk id
            */ 
            chunk_type* get_chunk(const int id) {
                lock_gurd_type lock(m_mutex);
                return (id < m_vChunks.size()) ? m_vChunks.at(id) : NULL;
            }
            vector_type get_chunks() {
                vector_type ret;
                m_vChunks.swap(ret);
                return ret;
            }
            /**
            * No copyble
            */ 
            basic_mempool_vector& operator=(const basic_mempool_vector&) = delete;
        public:
            inline void print_chunk(chunk_type& chunk) {
                chunk.print();      
            }
        private:
            vector_type    m_vChunks;
            lock_type      m_mutex;
            allocator_type m_allocator;
        };
    }
}


#endif
