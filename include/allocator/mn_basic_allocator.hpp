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

#include "../mn_functional.hpp"
#include "../mn_typetraits.hpp"
#include "../utils/mn_alignment.hpp"

#include "mn_basic_allocator_maximal_filter.hpp"

namespace mn {
	namespace memory {

		/**
		 * A a basic filter for a allocater
		 */
		class basic_allocator_filter {
		public:
			bool on_pre_alloc(size_t size, size_t alignment) { return true; }
			void on_alloc(size_t size, size_t alignment) { }

			bool on_pre_dealloc(size_t size, size_t alignment) { return true; }
			void on_dealloc(size_t size, size_t alignment) { }
		};

		/**
		 * The basic allocater for all allocator impl in this library.
		 */
		template <class TAllocator, class TFilter = basic_allocator_filter >
		class basic_allocator  {
		public:
			using allocator_category = typename TAllocator::allocator_category ;
			using is_thread_safe = typename TAllocator::is_thread_safe ;
			using filter_type = TFilter;

			using pointer = void*;

			basic_allocator() noexcept  { TAllocator::first();  }

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size, size_t alignment) {
				pointer _mem = nullptr;

				if(m_fFilter.on_pre_alloc(size)) {
					_mem = TAllocator::allocate(size, alignment);
					m_fFilter.on_alloc(size);
				}
				return _mem;
			}

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size) {
				return allocate(size, mn::alignment_for(size));
			}

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size The size of the Type
			 * @param count The count of the array
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t count, size_t size, size_t alignment = 0) {
				return allocate(count * size, (alignment == 0) ? mn::alignment_for(size) : alignment);
			}


			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free.
			 * @param alignment
			 * @param size The size of the Type
			 */
			void deallocate(pointer address, size_t size, size_t alignment) noexcept {
				if(m_fFilter.on_pre_dealloc(size)) {
					TAllocator::deallocate(address, size, alignment);
					m_fFilter.on_dealloc(size);
				}
			}

			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free.
			 * @param size The size of the Type
			 */
			void deallocate(pointer address, size_t size) noexcept {
				deallocate(address, size, mn::alignment_for(size));
			}

			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free
			 * @param count The count of the array
			 * @param size The size of the Type
			 * @param alignment
			 */
			void deallocate(pointer address, size_t count, size_t size, size_t alignment) noexcept {
				size = size * count;
				if(m_fFilter.on_pre_dealloc(size)) {
					TAllocator::deallocate(address, size, alignment);
					m_fFilter.on_dealloc(size);
				}
			}

			/**
			 * @brief Construct a object from allocated impl.
			 * @tparam Type The type of the object.
			 * @param Args The arguments for the constructer of the object.
			 */
			template <class Type, typename... Args>
			Type* construct(Args&&... args) {
				auto _size = sizeof(Type);

				pointer _mem = allocate(_size, mn::alignment_for(_size) );
				return new (_mem) Type(mn::forward<Args>(args)...);
			}

			/**
			 * @brief Deconstruct a object (call deconstructor) and free the memory
			 * @tparam Type The type of the object.
			 * @param address The pointer of the object to be deconstruct.
			 */
			template <class Type>
			void destroy(Type* address) noexcept {
				if(address == nullptr) return;

				auto _size = sizeof(Type);

				if(mn::is_class<Type>::value) address->~Type();
				deallocate(address, _size, mn::alignment_for(_size));
			}

			/**
			 * @brief Get the maximal size to allocate.
			 * @return The maximal size to allocate.
			 */
			size_t get_max_alocator_size() const noexcept {
				return TAllocator::get_max_alocator_size();
			}
		private:
			filter_type m_fFilter;
		};


	}
}

#endif // __MINILIB_BASIC_ALLOCATOR_H__
