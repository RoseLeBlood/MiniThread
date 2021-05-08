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
#ifndef __MINILIB_BASIC_ALLOCATOR_H__
#define __MINILIB_BASIC_ALLOCATOR_H__

#include "../mn_config.hpp"
#include "../mn_typetraits.hpp"

namespace mn {
	namespace memory {

		template <class TAllocator>
		class basic_allocator_debugger {
		public:
			virtual void on_alloc(size_t size) { }
			virtual void on_dealloc(size_t size) { }
		};

		template <class TAllocator, class TDebugger = basic_allocator_debugger<TAllocator> >
		class basic_allocator {
		public:
			using allocator_category = typename TAllocator::allocator_category ;
			using is_thread_safe = typename TAllocator::is_thread_safe ;

			using TDebugger::on_alloc;
			using TDebugger::on_dealloc;

			basic_allocator() noexcept  {  }

			void* allocate(size_t size, size_t alignment) {
				auto memory = TAllocator::allocate(size, alignment);
				this->on_alloc(size);
				return memory;
			}
			void deallocate(void* address, size_t size, size_t alignment) noexcept {
				TAllocator::deallocate(address, size, alignment);
				this->on_dealloc(size);
			}
			size_t get_max_alocator_size() const noexcept {
				return TAllocator::get_max_alocator_size();
			}
		};


	}
}

#endif // __MINILIB_BASIC_ALLOCATOR_H__
