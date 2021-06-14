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

		namespace internal {

			template<typename TAlloC>
			struct allocator_traits {
				using allocator_type = TAlloC ;
				using value_type = typename allocator_type::value_type;
				using pointer = typename allocator_type::pointer;
				using const_pointer = typename allocator_type::const_pointer;
				using difference_type = typename allocator_type::difference_type;
				using size_type = typename allocator_type::size_type;


				using allocator_category = typename TAlloC::allocator_category ;
				using is_thread_safe = typename TAlloC::is_thread_safe ;
			};
		}
		template <class TAlloC>
		struct is_thread_safe_allocator
			: public mn::integral_constant<bool, internal::allocator_traits<TAlloC>::is_thread_safe::value> { };


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


		/*
		 * @brief The default specialization of the allocator_traits for a allocator.
		 * Any specialization must provide the same interface.
         */
        template <class TAllocator>
        class allocator_traits {
		public:
			using allocator_type = typename internal::allocator_traits<TAllocator>::allocator_type;
			using pointer = typename internal::allocator_traits<TAllocator>::pointer;
			using size_type = typename internal::allocator_traits<TAllocator>::size_type;

			static pointer allocate_node(allocator_type& state, size_type size, size_type alignment) {
				return state.allocate(size, alignment);
			}

			static pointer allocate_array(allocator_type& state, size_type count, size_type size,
                                          size_type alignment) {
				return state.allocate(count, size, alignment);
			}

			static void deallocate_node(allocator_type& state, pointer node, size_type size,
                                        size_type alignment) noexcept {
            	state.deallocate(node, size, alignment);
			}
			static void deallocate_array(allocator_type& state, pointer node, size_type count,
											size_type size, size_type alignment) noexcept {
            	state.deallocate(node, count, size, alignment);
			}

			static size_type max_node_size(const allocator_type& state) {
				return size_t(-1);
			}
			static size_type max_array_size(const allocator_type& state) {
				return size_t(-1);
			}
			static size_type max_alignment(const allocator_type& state) {
				return mn::max_alignment;
			}
        };

	}
}


#endif // __MINILIB_ALLOCATOR_TYPETRAITS_H__
