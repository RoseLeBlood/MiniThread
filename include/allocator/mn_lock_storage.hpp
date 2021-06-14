/**
 * @file
 * @brief
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
 * The Mini Thread Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef __MN_BASIC_LOCK_STORAGE_H__
#define __MN_BASIC_LOCK_STORAGE_H__

#include "../mn_config.hpp"

#include "mn_allocator_typetraits.hpp"

#include "../mn_autolock.hpp"
#include "../mn_null_lock.hpp"

#include "mn_basic_allocator.hpp"

namespace mn {
	namespace memory {

		/**
		 * The basic allocater for all allocator impl in this library, with thread-safed version.
		 */
		template <class TMutex, class TAllocator, class TFilter = basic_allocator_filter>
		class basic_threadsafed_allocator {
		public:
			using allocator_category = typename allocator::allocator_category ;

			using value_type = void;
			using pointer = void*;
			using const_pointer = const void*;
			using difference_type = mn::ptrdiff_t;
			using size_type = size_t;

			using is_thread_safe = mn::true_type;

			using lock_type = TMutex;
			using allocator_impl = TAllocator;
			using filter_type = TFilter;
			using self_type = basic_threadsafed_allocator<TMutex, TAllocator, TFilter>;
			using lock_guard = mn::basic_autolock<lock_type>;


			basic_threadsafed_allocator() noexcept
				: m_lockObjct(), m_fFilter(), m_xTicksToWait(portMAX_DELAY) { allocator::first();  }

			basic_threadsafed_allocator(const lock_type& lckObject) noexcept
				: m_lockObjct(lckObject), m_fFilter(), m_xTicksToWait(portMAX_DELAY)  { allocator::first();  }

			basic_threadsafed_allocator(const lock_type& lckObject, const filter_type& _fFilter ) noexcept
				: m_lockObjct(lckObject), m_fFilter(_fFilter), m_xTicksToWait(portMAX_DELAY)  { allocator::first();  }

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size, size_t alignment) {
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				pointer _mem = nullptr;

				if(m_fFilter.on_pre_alloc(size)) {
					_mem = allocator_impl::allocate(size, alignment);
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
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				if(m_fFilter.on_pre_dealloc(size)) {
					allocator_impl::deallocate(address, size, alignment);
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
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				size = size * count;
				if(m_fFilter.on_pre_dealloc(size)) {
					allocator_impl::deallocate(address, size, (alignment == 0) ? mn::alignment_for(size) : alignment);
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

				void* _mem = allocate(_size, mn::alignment_for(_size) );

				return ::new (_mem) Type(mn::forward<Args>(args)...);
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

				mn::destruct<Type>(address);
				deallocate(address, _size, mn::alignment_for(_size));
			}

			/**
			 * @brief Get the maximal size to allocate.
			 * @return The maximal size to allocate.
			 */
			size_t get_max_alocator_size() const noexcept {
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				return allocator_impl::get_max_alocator_size();
			}

			void lock(unsigned long xTicksToWait = 0) const {

				m_lockObjct.lock(xTicksToWait == 0  ? m_xTicksToWait : xTicksToWait);
			}

            void unlock() const noexcept {
				m_lockObjct.unlock();
			}

			void set_ticksToWait(unsigned long xTicksToWait) {
				m_lockObjct.lock(m_xTicksToWait);
					m_xTicksToWait = xTicksToWait;
				m_lockObjct.unlock();
			}
			bool is_locked() {
				return m_lockObjct.is_locked();
			}

			basic_threadsafed_allocator(const self_type& other) noexcept = delete;
			self_type& operator = (const basic_threadsafed_allocator& other) noexcept  = delete;
		private:
			lock_type	m_lockObjct;
			filter_type m_fFilter;
			unsigned long m_xTicksToWait;
		};

	}
}


#endif // __MN_BASIC_LOCK_STORAGE_H__
