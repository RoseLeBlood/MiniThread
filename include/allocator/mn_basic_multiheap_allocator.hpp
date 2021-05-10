/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef __MINILIB_BASIC_MULTIHEAP_ALLOCATOR_H__
#define __MINILIB_BASIC_MULTIHEAP_ALLOCATOR_H__

#include "../mn_config.hpp"

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32

#include "mn_basic_allocator.hpp"
#include "mn_allocator_typetraits.hpp"

#include "multi_heap.h"

namespace mn {
	namespace memory {

		template<size_t TBytes>
		class basic_multiheap_allocator_impl {
		public:
			using allocator_category = std_allocator_tag();
			using is_thread_safe = std::false_type  ;


			static void first() noexcept {
				if(m_pHandle != nullptr) return;
				m_pHandle = multi_heap_register(m_buffer, TBytes);
			}
			static void* allocate(size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(alignment);
				return  (void*) multi_heap_malloc(m_pHandle, size );
			}

			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(size);
				MN_UNUSED_VARIABLE(alignment);

				multi_heap_free(m_pHandle, ptr);
			}

			static size_t max_node_size() noexcept {
				return size_t(-1);
			}
			static size_t get_max_alocator_size() const noexcept {
				return TBytes;
			}
		private:
			static char                    m_buffer[TBytes];
            static multi_heap_handle_t     m_pHandle;
		};

		template<size_t TBytes>
		char basic_multiheap_allocator_impl<TBytes>::m_buffer;

		template<size_t TBytes>
		multi_heap_handle_t basic_multiheap_allocator_impl<TBytes>::m_pHandle = nullptr;


		template<size_t TBytes, class TFilter = basic_allocator_filter>
		using multiheap_allocator = basic_allocator<basic_multiheap_allocator_impl<TBytes>, TFilter>;



	}
}

#endif // MN_THREAD_CONFIG_BOARD


#endif // __MINILIB_BASIC_MULTIHEAP_ALLOCATOR_H__
