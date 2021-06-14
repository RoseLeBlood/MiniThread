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

#ifndef __MINILIB_BASIC_SHARED_LIST_H__
#define __MINILIB_BASIC_SHARED_LIST_H__

#include "../mn_config.hpp"

#include "mn_list.hpp"

#include "../mn_autolock.hpp"
#include "../pointer/mn_shared_ptr.hpp"

#include <list>
namespace mn {
	namespace container {
		/**
		 * @brief A simple list of shared_ptr items.
		 * @tparam T The type for an element.
		 * @tparam TLockType Type of the lock object.
		 *
		 * @note Only thread safe when you use all functions with end with _save. For example:
		 * insert_save(..) or push_back_save(..)
		 */
		template <typename T, typename TLockType = mn::mutex_t>
		class basic_shared_list : public list< pointer::shared_atomic_ptr<T> > {
			/**
			 * @brief The base type of this object.
			 */
			using base_type = list< pointer::shared_atomic_ptr<T> > ;
		public:
			using self_type = basic_shared_list<T, TLockType>;

			using lock_type = TLockType;
			using pointer_type = pointer::shared_atomic_ptr<T>;

			using value_type = typename base_type::value_type;
			using reference = typename base_type::value_type&;
			using const_reference = const typename base_type::value_type&;
			using pointer = typename base_type::value_type*;
			using const_pointer = const typename base_type::value_type*;
			using allocator_type = typename base_type::allocator_type;
            using size_type = typename base_type::size_type;
            using node_type = typename base_type::node_type;
			using deleter = typename base_type::deleter;

			using iterator = typename base_type::iterator;
			using const_iterator = typename base_type::const_iterator;

			/**
			 * @brief  Creates a basic_shared_list with no elements.
		     */
			basic_shared_list(const allocator_type& allocator = allocator_type())
				: base_type(allocator), m_lockObject() { }

			/**
			 * @brief  Creates a basic_shared_list with no elements.
			 * @param lLock Reference of the lock object.
		     */
			basic_shared_list(lock_type& lLock, const allocator_type& allocator = allocator_type()) noexcept
				: base_type(allocator), m_lockObject(lLock) { }

			/**
			 *  @brief  Builds a basic_shared_list from a range.
			 *  @param  first  An input iterator.
			 *  @param  last  An input iterator.
			 *  @param  allocator  An allocator object.
			 *
			 *  Create a basic_shared_list consisting of copies of the elements from
			 *  [@afirst,@alast).
			 */
            template<class InputIterator>
            basic_shared_list(InputIterator first, InputIterator last,
							  const allocator_type& allocator = allocator_type()) noexcept
                : base_type(first, last, allocator_type), m_lockObject() { }
             /**
             * @brief Construct a new basic  shared list object
             */
            basic_shared_list(const basic_shared_list& rhs) noexcept
                : base_type(rhs), m_lockObject(rhs.m_lockObject) { }

			/**
			 * @brief Get the read/write iterator that points to the first element from the list.
			 * @return The read/write iterator that points to the first element from the list.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
			iterator begin_save(unsigned long xTicksToWait = portMAX_DELAY) {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return base_type::begin();
			}

			/**
			 * @brief Get the read/write iterator that points to the first element from the list.
			 * @return The read/write iterator that points to the first element from the list.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            const_iterator begin_save() const  {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::begin();
			}

			/**
			 * @brief Get the read/write iterator that points to the last element from the list.
			 * @return The read/write iterator that points to the last element from the list.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            iterator end_save(unsigned long xTicksToWait = portMAX_DELAY) {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::end();
			}

			/**
			 * @brief Get the read/write iterator that points to the last element from the list.
			 * @return The read/write iterator that points to the last element from the list.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            const_iterator end_save(unsigned long xTicksToWait = portMAX_DELAY) const {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::end();
			}

			/**
			 * @brief Get a read/write reference to the data at the first element.
			 * @return A read/write reference to the data at the first element.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            const_reference front_save(unsigned long xTicksToWait = portMAX_DELAY) const {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::front();
			}

			/**
			 * @brief Get a read/write reference to the data at the first element .
			 * @return A read/write reference to the data at the first element.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            reference front_save() {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::front();
			}

			/**
			 * @brief Get a read/write reference to the data at the last element.
			 * @return A read/write reference to the data at the last element.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            const_reference back_save(unsigned long xTicksToWait = portMAX_DELAY) const {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::back();
			}

			/**
			 * @brief Get a read/write reference to the data at the last element.
			 * @return A read/write reference to the data at the last element.
			 *
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 */
            reference back_save(unsigned long xTicksToWait = portMAX_DELAY) {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
            	return base_type::back();
			}

			/**
		   	 * @brief Add data to the front of the list.
		  	 * @param pValue Data to be added.
		  	 * @param xTicksToWait How long to wait to get the lock until giving up.
		  	 */
			void push_front_save(pointer pValue, unsigned long xTicksToWait = portMAX_DELAY) {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				base_type::push_front(pointer_type(pValue));
            }

            /**
		   	 * @brief Add data to the back of the list.
		  	 * @param pValue Data to be added.
		  	 * @param xTicksToWait How long to wait to get the lock until giving up.
		  	 */
            void push_back_save(pointer pValue, unsigned long xTicksToWait = portMAX_DELAY) {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);

				base_type::push_back(pointer_type(pValue));
            }

            /**
		   	 * @brief Add data to the front of the list.
		  	 * @param pValue Data to be added.
		  	 * @param xTicksToWait How long to wait to get the lock until giving up.
		  	 */
			void push_front_save(pointer_type pValue, unsigned long xTicksToWait = portMAX_DELAY) {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);

				base_type::push_front(pointer_type);
            }

            /**
		   	 * @brief Add data to the back of the list.
		  	 * @param pValue Data to be added.
		  	 * @param xTicksToWait How long to wait to get the lock until giving up.
		  	 */
            void push_back_save(pointer_type pValue, unsigned long xTicksToWait = portMAX_DELAY) {
            	basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);

				base_type::push_back(pointer_type);
            }

            /**
             * @brief Removes last element.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             */
            inline void pop_back_save(unsigned long xTicksToWait = portMAX_DELAY) {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				base_type::pop_back();
            }

            /**
             * @brief Removes first element.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             */
			inline void pop_front_save(unsigned long xTicksToWait = portMAX_DELAY) {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				base_type::pop_front();
            }

            /**
             * @brief Inserts given value into the list before specified iterator.
             * @param  pos  An iterator into the list.
             * @param  value  Data to be inserted.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return An iterator that points to the inserted data.
             */
            iterator insert(iterator pos, pointer value, unsigned long xTicksToWait = portMAX_DELAY) {
                basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return base_type::insert(pos, pointer_type(value) );
            }

            /**
             * @brief Inserts given value into the list before specified iterator.
             * @param  pos  An iterator into the list.
             * @param  value  Data to be inserted.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return An iterator that points to the inserted data.
             */
            iterator insert_save(iterator pos, pointer_type value, unsigned long xTicksToWait = portMAX_DELAY) {
                basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return base_type::insert(pos, (value) );
            }

             /**
             * @brief remove given value into the list before specified iterator.
             * @param  it  An iterator into the list.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return  An iterator pointing to the next element (or end()).
             */
            iterator erase_save(iterator it, unsigned long xTicksToWait = portMAX_DELAY) {
                basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return base_type::erase(it);
            }


            /**
             * @brief Remove a range of elements.
             * @param first  Iterator pointing to the first element to be erased.
       		 * @param last  Iterator pointing to one past the last element to be
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return An iterator pointing to the element pointed to by @a last prior to erasing (or end()).
             */
            iterator erase_save(iterator first, iterator last, unsigned long xTicksToWait = portMAX_DELAY) {
                basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return base_type::erase(first, last);
            }

            /**
             * @brief Get the used list of this object.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return The list.
             */
			base_type& list(unsigned long xTicksToWait = portMAX_DELAY) noexcept {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return *(static_cast<base_type*>(this));
			}

			/**
             * @brief Get the used list of this object.
             * @param xTicksToWait How long to wait to get the lock until giving up.
             * @return The list.
             */
			const base_type& list(unsigned long xTicksToWait = portMAX_DELAY) const noexcept {
				basic_autolock<TLockType> _lock(m_lockObject, xTicksToWait);
				return *(static_cast<base_type*>(this));
			}

			/**
			 * @brief Lock this object.
			 * @param xTicksToWait How long to wait to get the lock until giving up.
			 * @return The error code from the using lock object.
			 */
			int lock(unsigned long xTicksToWait = portMAX_DELAY) {
				return m_lockObject.lock(xTicksToWait);
			}
			/**
			 * @brief Unlock this object.
			 * @return The error code from the using lock object.
			 */
			int unlock() {
				return m_lockObject.unlock();
			}
		protected:
			/**
			 * @brief The lock object.
			 */
			lock_type m_lockObject;
		};

		template <typename T, typename TLockType = mn::mutex_t>
		using shared_ptr_list =  basic_shared_list<T, TLockType>;
	}
}


#endif // __MINILIB_BASIC_SHARED_LIST_H__
