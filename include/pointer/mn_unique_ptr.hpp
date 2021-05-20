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
#ifndef __MINILIB_UNIQUE_PTR_H__
#define __MINILIB_UNIQUE_PTR_H__

#include "../mn_config.hpp"

#include "../mn_copyable.hpp"
#include "../mn_deleter.hpp"
#include "../mn_functional.hpp"

#include "../utils/mn_utils.hpp"


namespace mn {
	namespace pointer {
		/**
		 * @brief A simple  unique_ptr, for a single object.
		 * @tparam T The type of the stored pointer.
		 * @tparam TAllocator The type of the using allocator, for the deleter.
		 * @tparam TDeleter The type of the using deleter.
		 */
		template <typename T, class TAllocator = memory::default_allocator,
				 class TDeleter = memory::default_delete<T, TAllocator> >
		class basic_unique_ptr : public mn::only_move_tag<void> {
		public:
			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using deleter_type = TDeleter;
			using allotor_type = TAllocator;

			using self_type = basic_unique_ptr<value_type, allotor_type, deleter_type>;

			/**
			 * @brief Constructor a basic_unique_ptr that owns nothing.
		     */
      		constexpr basic_unique_ptr() noexcept
      			: m_ptr(nullptr), m_refDeleter(deleter_type()) { }

      		/**
			 * @brief Creates a unique_ptr that owns nothing.
		     */
			constexpr basic_unique_ptr(nullptr_t) noexcept
				: m_ptr(nullptr), m_refDeleter(deleter_type()) { }

			/**
			 * @brief Takes ownership of a pointer.
		     */
			explicit basic_unique_ptr(pointer pPointer) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter_type()) { }

			/**
			 * @brief Takes ownership of a pointer.
		     *
		     * @param pPointer A pointer to an object of @c element_type
		     * @param deleter  A reference to a deleter.
		     */
			basic_unique_ptr(pointer pPointer, deleter_type& deleter) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter) { }

			/**
			 * @brief Takes ownership of a pointer.
		     *
		     * @param pPointer A pointer to an object of @c element_type
		     * @param deleter  A reference to a deleter.
		     */
			basic_unique_ptr(pointer pPointer, const deleter_type& deleter) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter) { }

			/**
			 * @brief Takes ownership of a pointer.
			 * @param pPointer A pointer to an object of @c element_type
			 * @param deleter  An rvalue reference to a deleter.
			 */
			basic_unique_ptr(pointer pPointer, deleter_type&& deleter) noexcept
				: m_ptr(std::move(pPointer)), m_refDeleter(std::move(deleter)) { }

			/**
			 * @brief Move constructor.
			 */
			basic_unique_ptr(self_type&& other)
			 	: m_ptr(other.release()), m_refDeleter(mn::forward<deleter_type>(other.get_deleter()) ) { }

			/**
			 * @brief Converting constructor from another type.
			 */
			template<typename U, class UAllocator = memory::default_allocator,
					 class UDeleter = memory::default_delete<U, UAllocator>>
			basic_unique_ptr(basic_unique_ptr<U, UAllocator, UDeleter>&& other) noexcept
				: m_ptr(other.release()), m_refDeleter(mn::forward<deleter_type>(other.get_deleter()) ) { }

			/**
			 * @brief Destructor, invokes the deleter if the stored pointer is not null.
			 */
			~basic_unique_ptr() noexcept {
				//deleter_type _deleter = m_mPair.second();

				if (m_ptr != nullptr) get_deleter()(m_ptr);
				m_ptr = nullptr;
			}


			/**
			 * @brief Move assignment operator.
			 * @param other The object to transfer ownership from.
			 * @return this
			 */
			self_type&  operator=(self_type&& other) noexcept {
				reset(other.release());
				m_refDeleter = mn::forward<deleter_type>(other.get_deleter());
				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param other The object to transfer ownership from.
			 * @return this
			 */
			template<typename U, class UAllocator = memory::default_allocator,
					 class UDeleter = memory::default_delete<U, UAllocator>>
			self_type&  operator=(basic_unique_ptr<U, UAllocator, UDeleter>&& other) noexcept {
				reset(other.release());
				m_refDeleter = mn::forward<deleter_type>(other.get_deleter());
				return *this;
			}

			/**
			 * @brief Release ownership of any stored pointer.
			 * @return The stored pointer, bevor it set to null.
			 */
			pointer release() noexcept {
				pointer _ptr = get();
				m_ptr = pointer();
				return *this;
			}

			/**
			 * @brief Replace the stored pointer.
			 */
			void reset(pointer pPtr) noexcept {
				mn::swap(m_ptr, pPtr);
				if (pPtr != pointer())
	  				get_deleter()(pPtr);
			}
			/**
			 * @brief Exchange the pointer and deleter with another object.
			 */
			void swap(self_type& other) noexcept {
				mn::swap(m_ptr, other.m_ptr);
				mn::swap(m_refDeleter, other.m_refDeleter);
			}
			/**
			 * @brief Get the stored pointer.
			 * @return The stored pointer.
			 */
			pointer get() const noexcept {
				return m_ptr;
			}

			/**
			 * @brief Get a reference to the stored deleter.
			 * @return The reference to the stored deleter.
			 */
			deleter_type& get_deleter() noexcept {
				return m_refDeleter;
			}
			/**
			 * @brief Get a reference to the stored deleter.
			 * @return The reference to the stored deleter.
			 */
			const deleter_type& get_deleter() const noexcept {
				return m_refDeleter;
			}


			/**
			 * @brief Return the stored pointer as reference.
			 */
			reference operator*() const {
				return *m_ptr;
			}
			/**
			 * @brief Return the stored pointer.
			 */
			pointer operator->() const noexcept {
				return m_ptr;
			}
			/**
			 * @brief Is the stored pointer not null?
			 * @return true if the stored pointer is not null.
			 */
			explicit operator bool() const noexcept {
				return m_ptr == nullptr ? false : true;
			}
		private:
			pointer m_ptr;
			deleter_type& m_refDeleter;
		};

		/**
		 * @brief A simple  unique_ptr, for a array object.
		 * @tparam T The type of the stored pointer.
		 * @tparam TAllocator The type of the using allocator, for the deleter.
		 * @tparam TDeleter The type of the using deleter.
		 */
		template <typename T, class TAllocator, class TDeleter >
		class basic_unique_ptr<T[], TAllocator, TDeleter> : public mn::only_move_tag<void> {
		public:
			using value_type = T;
			using pointer = T*;
			using const_pointer = const T*;
			using reference = T&;
			using const_reference = const T&;
			using deleter_type = TDeleter;
			using allotor_type = TAllocator;

			using self_type = basic_unique_ptr<value_type, allotor_type, deleter_type>;

			/**
			 * @brief Constructor a basic_unique_ptr that owns nothing.
		     */
      		constexpr basic_unique_ptr() noexcept
      			: m_ptr(nullptr), m_refDeleter(deleter_type()) { }

      		/**
			 * @brief Creates a unique_ptr that owns nothing.
		     */
			constexpr basic_unique_ptr(nullptr_t) noexcept
				: m_ptr(nullptr), m_refDeleter(deleter_type()) { }

			/**
			 * @brief Takes ownership of a pointer.
		     */
			explicit basic_unique_ptr(pointer pPointer) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter_type()) { }

			/**
			 * @brief Takes ownership of a pointer.
		     *
		     * @param pPointer A pointer to an object of @c element_type
		     * @param deleter  A reference to a deleter.
		     */
			basic_unique_ptr(pointer pPointer, deleter_type& deleter) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter) { }

			/**
			 * @brief Takes ownership of a pointer.
		     *
		     * @param pPointer A pointer to an object of @c element_type
		     * @param deleter  A reference to a deleter.
		     */
			basic_unique_ptr(pointer pPointer, const deleter_type& deleter) noexcept
				: m_ptr(pPointer), m_refDeleter(deleter) { }

			/**
			 * @brief Takes ownership of a pointer.
			 * @param pPointer A pointer to an object of @c element_type
			 * @param deleter  An rvalue reference to a deleter.
			 */
			basic_unique_ptr(pointer pPointer, deleter_type&& deleter) noexcept
				: m_ptr(std::move(pPointer)), m_refDeleter(std::move(deleter)) { }

			/**
			 * @brief Move constructor.
			 */
			basic_unique_ptr(self_type&& other)
			 	: m_ptr(other.release()), m_refDeleter(mn::forward<deleter_type>(other.get_deleter()) ) { }

			/**
			 * @brief Converting constructor from another type.
			 */
			template<typename U, class UAllocator = memory::default_allocator,
					 class UDeleter = memory::default_delete<U, UAllocator>>
			basic_unique_ptr(basic_unique_ptr<U, UAllocator, UDeleter>&& other) noexcept
				: m_ptr(other.release()), m_refDeleter(mn::forward<deleter_type>(other.get_deleter()) ) { }

			template<typename U>
			explicit basic_unique_ptr(U* p) noexcept = delete;
			/**
			 * @brief Destructor, invokes the deleter if the stored pointer is not null.
			 */
			~basic_unique_ptr() noexcept {
				//deleter_type _deleter = m_mPair.second();

				if (m_ptr != nullptr) get_deleter()(m_ptr);
				m_ptr = nullptr;
			}


			/**
			 * @brief Move assignment operator.
			 * @param other The object to transfer ownership from.
			 * @return this
			 */
			self_type&  operator=(self_type&& other) noexcept {
				reset(other.release());
				m_refDeleter = mn::forward<deleter_type>(other.get_deleter());
				return *this;
			}

			/**
			 * @brief Move assignment operator.
			 * @param other The object to transfer ownership from.
			 * @return this
			 */
			template<typename U, class UAllocator = memory::default_allocator,
					 class UDeleter = memory::default_delete<U, UAllocator>>
			self_type&  operator=(basic_unique_ptr<U, UAllocator, UDeleter>&& other) noexcept {
				reset(other.release());
				m_refDeleter = mn::forward<deleter_type>(other.get_deleter());
				return *this;
			}

			/**
			 * @brief Release ownership of any stored pointer.
			 * @return The stored pointer, bevor it set to null.
			 */
			pointer release() noexcept {
				pointer _ptr = get();
				m_ptr = pointer();
				return *this;
			}

			/**
			 * @brief Replace the stored pointer.
			 */
			void reset(pointer pPtr) noexcept {
				mn::swap(m_ptr, pPtr);
				if (pPtr != pointer())
	  				get_deleter()(pPtr);
			}
			/**
			 * @brief Exchange the pointer and deleter with another object.
			 */
			void swap(self_type& other) noexcept {
				mn::swap(m_ptr, other.m_ptr);
				mn::swap(m_refDeleter, other.m_refDeleter);
			}

			/**
			 * @brief Get the stored pointer.
			 * @return The stored pointer.
			 */
			pointer get() const noexcept {
				return m_ptr;
			}

			/**
			 * @brief Get a reference to the stored deleter.
			 * @return The reference to the stored deleter.
			 */
			deleter_type& get_deleter() noexcept {
				return m_refDeleter;
			}
			/**
			 * @brief Get a reference to the stored deleter.
			 * @return The reference to the stored deleter.
			 */
			const deleter_type& get_deleter() const noexcept {
				return m_refDeleter;
			}

			/**
			 * @brief Return the stored pointer as reference.
			 */
			reference operator*() const {
				return *m_ptr;
			}
			/**
			 * @brief Return the stored pointer.
			 */
			pointer operator->() const noexcept {
				return m_ptr;
			}

			/**
			 * @brief Access an element of owned array.
			 */
      		reference operator[] (size_t pos) const {
				assert(m_ptr != nullptr);
				return m_ptr[pos];
      		}
			/**
			 * @brief Is the stored pointer not null?
			 * @return true if the stored pointer is not null.
			 */
			explicit operator bool() const noexcept {
				return m_ptr == nullptr ? false : true;
			}

			// Disable construction from convertible pointer types.
      		template<typename U>
			basic_unique_ptr(U*, const deleter_type) = delete;

			// Disable construction from convertible pointer types.
			template<typename U>
			basic_unique_ptr(U*, deleter_type&&) = delete;
		private:
			pointer m_ptr;
			deleter_type& m_refDeleter;
		};

		template<typename T, class TA, class TD>
		void swap(basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			a.swap(b);
		}

		template<typename T, class TA, class TD>
		inline bool operator == (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			return a.get() == b.get();
		}


		template<typename T, class TA, class TD>
		inline bool operator != (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			return a.get() != b.get();
		}

		template<typename T, class TA, class TD>
		inline bool operator < (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			using pointer = typename basic_unique_ptr<T, TA, TD>::pointer;
			return mn::less<pointer>(a.get(), b.get());
		}
		template<typename T, class TA, class TD>
		inline bool operator > (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			using pointer = typename basic_unique_ptr<T, TA, TD>::pointer;
			return mn::greater<pointer>(a.get(), b.get());
		}

		template<typename T, class TA, class TD>
		inline bool operator <= (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			return a.get() <= b.get();
		}

		template<typename T, class TA, class TD>
		inline bool operator >= (basic_unique_ptr<T, TA, TD>& a, basic_unique_ptr<T, TA, TD>& b) {
			return a.get() >= b.get();
		}


		template<typename T, typename... TArgs>
    	inline basic_unique_ptr<T> make_unique(TArgs&&... args) {
    		return basic_unique_ptr<T>(new T(mn::forward<TArgs>(args)...));
		}

		template<typename T>
		inline basic_unique_ptr<T[]> make_unique(size_t siNums) {
			return basic_unique_ptr<T>(new T[siNums]() );
		}

		template<typename T, typename... TArgs>
		inline basic_unique_ptr<T[]> make_unique(TArgs&&...) = delete;

		template<typename T,
		        class TAllocator = memory::default_allocator,
				class TDeleter = memory::default_delete<T, TAllocator>>
		using unique_ptr = basic_unique_ptr<T, TAllocator, TDeleter>;
	}
}

#endif // __MINILIB_UNIQUE_PTR_H__
