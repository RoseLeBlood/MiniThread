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
#ifndef __MINLIB_BASIC_DELETER_H__
#define __MINLIB_BASIC_DELETER_H__


#include "../mn_config.hpp"

#include "mn_basic_allocator.hpp"
#include "mn_allocator_typetraits.hpp"

#include "../mn_functional.hpp"

namespace mn {
	namespace memory {
		/**
		 * @brief A Simple template for a deleter.
		 * @tparam Type The type of pointer to delete.
		 * @tparam TAllocator The using allocator for call deallocate
		 */
		template<typename Type, class TAllocator>
        class basic_deleter  {
        public:
			using value_type  = Type;
			using allocator = TAllocator;
			using pointer = TAllocator*;
			using reference = TAllocator&;

			/**
			 * @brief Construct a basic deleter
			 */
			constexpr basic_deleter() : m_refAllocator() {}
			/**
			 * @brief Construt a basic deleter
			 * @param alloc The using allocator class
			 */
			constexpr basic_deleter(reference alloc, size_t) noexcept
            	: m_refAllocator(&alloc) { }

			/**
			 * @brief Converting constructor.
			 * Allows conversion from a deleter for arrays of another type, @p U,
		 	 * only if @p U* is convertible to @p value_type*.
			 */
			template<typename U, typename = typename enable_if<is_convertible<U*,value_type*>::value>::type>
			basic_deleter(const basic_deleter<U, TAllocator>& other)  noexcept
				 : m_refAllocator(other.m_refAllocator)  { }

            /**
			 * @brief Is the deleter valid - have a allocator
			 * @return True when the deleter valid is and false if not.
			 */
			bool is_valid() const noexcept {
				return m_refAllocator != nullptr;
			}

			/**
			 * @brief Get a reference from the using allocator.
			 * @return The reference from the using allocator.
			 */
			reference get_allocator() {
				return *m_refAllocator;
            }

            /**
			 * @brief Set the using allocator for delete.
			 * @param alloc The reference of the using allocator.
			 */
            void set_allocator(reference alloc) noexcept {
				m_refAllocator = &alloc;
            }

            /**
			 * @brief Calls @c deallocate @p pArray
			 */
			void operator()(value_type* pP) noexcept {
				static_assert(!mn::is_void<value_type>::value, "can't delete pP to incomplete type");
				static_assert(sizeof(value_type) > 0, "can't delete pP to incomplete type");

                m_refAllocator->deallocate(pP, sizeof(value_type), alignof(value_type));
            }
		private:
			pointer m_refAllocator;
        };

        /**
		 * @brief A Simple template for a deleter.
		 * @tparam Type The type of pointer to delete.
		 * @tparam TAllocator The using allocator for call deallocate
		 * @note Specialization for arrays
		 */
        template<typename Type, class TAllocator>
        class basic_deleter<Type[], TAllocator> {
        public:
			using value_type  = Type;
			using allocator = TAllocator;
			using pointer = TAllocator*;
			using reference = TAllocator&;

			template<typename U>
			using remove_cv_type = typename mn::remove_cv<U>::type;

			/**
			 * @brief Like is_base_of<_Tp, _Up> but false if unqualified types are the same
			 */
			template<typename U>
			using is_derived_Tp = integral_constant< bool, is_base_of<value_type, U>::value &
			 						!(is_same<remove_cv_type<value_type>, remove_cv_type<U>>::value )>;

			/**
			 * @brief Construct a basic deleter.
			 */
			constexpr basic_deleter() : m_refAllocator(), m_sArraySize(0u) {}
			/**
			 * @brief Construt a basic deleter
			 * @param alloc The using allocator class
			 * @param size The size of the array
			 */
			constexpr basic_deleter(reference alloc, size_t size) noexcept
            	: m_refAllocator(&alloc), m_sArraySize(size) { }

			/**
			 * @brief Converting constructor.
			 * Allows conversion from a deleter for arrays of another type, @p U,
		 	 * only if @p U* is convertible to @p value_type*.
			 */
			template<typename U, typename = typename enable_if<is_convertible<U*, value_type*>::value>::type>
			basic_deleter(const basic_deleter<U[], TAllocator>& other)  noexcept
				 : m_refAllocator(other.m_refAllocator), m_sArraySize(other.m_sArraySize)  { }

			/**
			 * @brief Is the deleter valid - have a allocator
			 * @return True when the deleter valid is and false if not.
			 */
			bool is_valid() const noexcept {
				return m_refAllocator != nullptr;
			}

			/**
			 * @brief Get a reference from the using allocator.
			 * @return The reference from the using allocator.
			 */
			reference get_allocator() {
				return *m_refAllocator;
            }

            /**
			 * @brief Set the using allocator for delete.
			 * @param alloc The reference of the using allocator.
			 */
            void set_allocator(reference alloc) noexcept {
				m_refAllocator = &alloc;
            }

            /**
			 * @brief Get the size of the array.
			 * @return The size of the array.
			 */
            size_t get_size() const {
            	return m_sArraySize;
            }

            /**
			 * @brief Calls @c deallocate @p pArray
			 */
			void operator()(value_type* pArray) noexcept {
				static_assert(sizeof(value_type) > 0, "can't delete pArray to incomplete type");
                m_refAllocator->deallocate(pArray, m_sArraySize, sizeof(value_type), alignof(value_type));
            }


            template<typename U>
			typename enable_if<is_derived_Tp<U>::value>::type operator()(value_type*) const = delete;

		private:
			pointer m_refAllocator;
			size_t m_sArraySize;
        };
	}
}


#endif // __MINLIB_BASIC_DELETER_H__
