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
#ifndef __MINILIB_MEM_BASIC_MALLOC_ALLOCATOR_H__
#define __MINILIB_MEM_BASIC_MALLOC_ALLOCATOR_H__

#include "../mn_config.hpp"

#include <cstdlib>
#include <memory>

#include "mn_basic_allocator.hpp"
#include "mn_allocator_typetraits.hpp"

namespace mn {
	namespace memory {

		class basic_malloc_allocator_impl {
		public:
			using allocator_category = std_allocator_tag();
			using is_thread_safe = std::false_type ;

			static void first() noexcept { }

			static void* allocate(size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(alignment);
				return malloc(size);
			}

			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(size);
				MN_UNUSED_VARIABLE(alignment);
				free(ptr);
			}

			static size_t max_node_size()  {
				return size_t(-1);
			}

			static size_t get_max_alocator_size()  {
				return __SIZE_MAX__;
			}
		};

		template <class TFilter = basic_allocator_filter>
		using malloc_allocator = basic_allocator<basic_malloc_allocator_impl, TFilter>;
	}
}

#endif // __MINILIB_MEM_BASIC_MALLOC_ALLOCATOR_H__
