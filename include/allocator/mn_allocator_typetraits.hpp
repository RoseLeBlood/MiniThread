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
#ifndef __MINILIB_ALLOCATOR_TYPETRAITS_H__
#define __MINILIB_ALLOCATOR_TYPETRAITS_H__

#include "../mn_config.hpp"
#include "../mn_typetraits.hpp"
#include "../utils/mn_alignment.hpp"

namespace mn {
	namespace memory {
		struct std_allocator_tag { };
		struct nodeleter_allocator_tag { };

		template<typename TAlloC>
		struct allocator_traits {
			using allocator_category = typename TAlloC::allocator_category ;
			using is_thread_safe = typename TAlloC::is_thread_safe ;
		};

		template <class TAlloC>
        struct is_thread_safe_allocator
        	: mn::integral_constant<bool, allocator_traits<TAlloC>::is_thread_safe::value> { };

		namespace internal {
			template <class TAlloC>
			inline void* allocate(const TAlloC& alloc, size_t size, size_t alignment,
								  mn::memory::std_allocator_tag) {
				return alloc.allocate(size, alignment);
			}

			template <class TAlloC>
			inline void* allocate(const TAlloC& alloc, size_t size, size_t alignment,
								  mn::memory::nodeleter_allocator_tag) {
				return alloc.allocate(size, alignment);
			}
			template <class TAlloC>
			inline void* deallocate(const TAlloC& alloc, void* address, size_t size, size_t alignment,
								   mn::memory::std_allocator_tag)  {
				alloc.deallocate(address, size, alignment); return address;
			}

			template <class TAlloC>
			inline void* deallocate(const TAlloC& alloc, void* address, size_t size, size_t alignment,
								   mn::memory::nodeleter_allocator_tag)  {
				return address;
			}

		} // internal

		template <class TAlloC>
		inline void* allocate(const TAlloC& alloc, size_t size, size_t alignment) {
			return internal::allocate(alloc, size, alignment, typename allocator_traits<TAlloC>::allocator_category() );
		}

		template <class TAlloC>
		inline void* deallocate(const TAlloC& alloc, void* address, size_t size, size_t alignment) {
			return internal::deallocate(alloc, address, size, alignment,
										typename allocator_traits<TAlloC>::allocator_category() );
		}

		template <class TAlloC>
		inline size_t max_alocator_size(const TAlloC& alloc) {
			return alloc.get_max_alocator_size();
		}

	}
}


#endif // __MINILIB_ALLOCATOR_TYPETRAITS_H__
