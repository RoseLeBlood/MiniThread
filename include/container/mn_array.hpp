/**
 * @file
 * @brief Class for a dynamic array.
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
#ifndef __MINILIB_CONTAINER_DYNAMIC_ARRAY_H__
#define __MINILIB_CONTAINER_DYNAMIC_ARRAY_H__

#include "../mn_config.hpp"

# include <cassert>
# include <typeinfo>

#include "mn_algorithm.hpp"
#include "../mn_deleter.hpp"
#include "../mn_def.hpp"
#include "../mn_functional.hpp"
#include "../mn_typetraits.hpp"

#include "../utils/mn_utils.hpp"
#include "../container/mn_pair.hpp"

namespace mn {
	namespace container {

		template<typename TArray>
		struct array_traits {
			using array_type = TArray;

			using value_type = typename array_type::value_type;
			using reference = typename array_type::reference;
			using const_reference = typename array_type::const_reference;
			using pointer = typename array_type::pointer;

			using difference_type = typename array_type::difference_type;
			using size_type = typename array_type::size_type;
			using allocator = typename array_type::allocator;
			using deleter = typename array_type::deleter;
			using iterator = typename array_type::iterator;
			using const_iterator = typename array_type::const_iterator;

			static const size_type TypeSize = TArray::TypeSize;
		};
		/**
		 * @brief Helper class for array iterators.
		 */
		template <typename T>
		class basic_array_iterator {
		public:
			using iterator_category = bidirectional_iterator_tag ;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using const_reference = const T&;
			using difference_type = mn::ptrdiff_t;
			using size_type = mn::size_t;

			using self_type = basic_array_iterator<T>;

			basic_array_iterator(pointer _valuePtr, uint32_t pos) noexcept
				: m_valuePtr(_valuePtr), m_iPosition(pos) { }

			basic_array_iterator(const self_type& other) noexcept
				: m_valuePtr(other.m_valuePtr), m_iPosition(other.m_iPosition) { }

			basic_array_iterator(const self_type&& other) noexcept
				: m_valuePtr(mn::move(other.m_valuePtr)), m_iPosition(mn::move(other.m_iPosition)) { }

			bool operator != (const self_type& other) const {
				return !(*this == other);
			}

			bool operator == (const self_type& other) const {
				return m_iPosition == other.m_iPosition;
			}

			self_type& operator++() {
				++m_iPosition; return *this;
			}
			self_type& operator--() {
				--m_iPosition; return *this;
			}

			reference operator*() {
				return *(m_valuePtr + m_iPosition);
			}

			const_reference operator*() const {
				return *(m_valuePtr + m_iPosition);
			}

			self_type& operator = (const self_type& other) noexcept {
				m_valuePtr = other.m_valuePtr;
				m_iPosition = other.m_iPosition;
				return *this;
			}

			reference operator [] (size_type index) {
				return (m_valuePtr + m_iPosition);
			}

			const_reference operator [] (size_type index) const {
				return (m_valuePtr + m_iPosition);
			}
		private:
			pointer m_valuePtr;
			uint32_t m_iPosition;
		};
		/**
		 * @brief Lightweight c++11 array implementation.
		 */
		template <typename T, class TAllocator = memory::default_allocator,
			class TDeleter = mn::memory::default_delete<T, TAllocator> >
		class basic_array {
		public:
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using const_reference = const T&;
			using difference_type = mn::ptrdiff_t;
			using size_type = mn::size_t;
			using allocator_type = TAllocator;
			using deleter_type = TDeleter;
			using iterator = basic_array_iterator<T>;
			using const_iterator = basic_array_iterator<const T>;

			static const size_type TypeSize = sizeof(value_type);

			using self_type = basic_array<T, TAllocator, TDeleter>;

			basic_array()
				: m_arData(nullptr),
				  m_szSize(16),
				  m_szCurPos(0),
				  m_alloCator(),
				  m_dDeleter() {

				m_arData = construct(m_szSize);
			}

			basic_array(pointer _ptrData, unsigned int _sSize)
				: m_arData(_ptrData),
				  m_szSize(_sSize),
				  m_szCurPos(0),
				  m_alloCator(),
				  m_dDeleter(m_alloCator) { }

			basic_array(const self_type& other)
				: m_arData(other.m_arData),
				  m_szSize(other.m_szSize),
				  m_szCurPos(other.m_szCurPos),
				  m_alloCator(other.m_alloCator),
				  m_dDeleter(other.m_dDeleter) { }

			~basic_array() {
				if (m_arData != nullptr) {
					deconstruct(m_arData, m_szSize);
					m_arData = nullptr;
				}
    		}

    		iterator begin() 					{ return iterator(m_arData, 0); }
			const_iterator begin() const 		{ return iterator(m_arData, 0); }

			iterator end() 						{ return iterator(m_arData, m_szSize); }
			const_iterator end() const 			{ return const_iterator(m_arData, m_szSize); }

			reference 		front() noexcept 			{ return (m_arData[0]); }
      		const_reference front() const noexcept		{ return (m_arData[0]); }

      		reference 		back() noexcept 			{ return (m_arData[m_szSize-1]); }
			const_reference back() const noexcept 		{ return (m_arData[m_szSize-1]); }

			size_type 		size() const noexcept 		{ return m_szSize; }

			reference 		at(size_type pos) 			{ return m_arData[pos]; }
			const_reference at(size_type pos) const 	{ return m_arData[pos]; }

			constexpr bool is_empty() const noexcept {
				return m_szSize == 0; }

			constexpr bool is_full() const noexcept {
				return m_szSize == m_szCurPos; }


			int push(value_type entry) {
				if (m_szCurPos >= m_szSize)
					if(!resize(m_szSize + 16)) return -1;

				m_arData[m_szCurPos] = entry; m_szCurPos++;

				return m_szCurPos;
			}

			reference pop() {
				assert(is_full());
				return m_arData[m_szCurPos++];
			}


			bool resize(size_type newSize) {
				pointer _newArData = construct(newSize);
				if(_newArData == nullptr) return false;

				size_type _cpysize = (newSize < m_szSize) ?  newSize : m_szSize;

				for(int i = 0; i < _cpysize; i++)
					_newArData[i] = m_arData[i];

				deconstruct(m_arData, m_szSize);
				m_szSize = newSize;
				m_arData = _newArData;

				m_szCurPos = (m_szCurPos >= m_szSize) ? m_szSize-1 : m_szCurPos;

				return true;
			}

			bool is_equele(const self_type& other, bool bWithPos = false) {
				if(m_szSize != other.m_szSize) return false;
				if(bWithPos && (m_szCurPos != other.m_szCurPos)) return false;

				for(int i = 0; i < m_szSize; i++)
					if(m_arData[i] != other.m_arData[i]) return false;

				return true;
			}

			void swap(self_type& other) noexcept  {
				swap_ranges(begin(), end(), other.begin());
			}

			reference operator[](size_type pos) noexcept {
				assert(pos < m_szSize);
				return m_arData[pos]; }

      		constexpr const_reference operator[](size_type pos) const noexcept {
      			assert(pos < m_szSize);
      			return m_arData[pos]; }

			void fill(const value_type& val) {
				mn::fill_n<value_type>(begin(), size(), val);
			}
		private:
			pointer construct(size_type n) {
				void* _mem = m_alloCator.allocate(n, TypeSize, mn::alignment_for(n * TypeSize) );
				//return (pointer*)_val;

				return new (_mem) value_type[n];
			}
			void deconstruct (pointer ptr, size_type size) {
				m_alloCator.deallocate(ptr, size, TypeSize, mn::alignment_for(size * TypeSize) );
			}
		private:
			pointer 	m_arData;
			size_type 	m_szSize;
			size_type 	m_szCurPos;

			allocator_type  m_alloCator;
			deleter_type	m_dDeleter;
		};

		// Array comparisons.
		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator == (const basic_array<T, TAllocator, TDeleter>& a,
								  const basic_array<T, TAllocator, TDeleter>& b) {
			return a.is_equele(b);
		}

		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator != (const basic_array<T, TAllocator, TDeleter>& a,
								  const basic_array<T, TAllocator, TDeleter>& b) {
			return !(a.is_equele(b));
		}


 		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator < (const basic_array<T, TAllocator, TDeleter>& a,
								 const basic_array<T, TAllocator, TDeleter>& b) {
			for(int i = 0; i < mn::min(a.size(), b.size()); i++)
				if(a[i] >= b[i]) return false;
			return true;
		}

		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator <= (const basic_array<T, TAllocator, TDeleter>& a,
								  const basic_array<T, TAllocator, TDeleter>& b) {
			for(int i = 0; i < mn::min(a.size(), b.size()); i++)
				if(a[i] > b[i]) return false;
			return true;
		}

		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator > (const basic_array<T, TAllocator, TDeleter>& a,
								 const basic_array<T, TAllocator, TDeleter>& b) {
			for(int i = 0; i < mn::min(a.size(), b.size()); i++)
				if(a[i] <= b[i]) return false;
			return true;
		}

		template <typename T, class TAllocator, class TDeleter >
		inline bool  operator >= (const basic_array<T, TAllocator, TDeleter>& a,
								  const basic_array<T, TAllocator, TDeleter>& b) {
			for(int i = 0; i < mn::min(a.size(), b.size()); i++)
				if(a[i] < b[i]) return false;
			return true;
		}

		template <typename T, class TAllocator, class TDeleter >
		inline void swap (basic_array<T, TAllocator, TDeleter>& a,
						  basic_array<T, TAllocator, TDeleter>& b) {
			a.swap(b);
		}

		template <typename T, class TAllocator, class TDeleter >
		inline void fill (basic_array<T, TAllocator, TDeleter>& a, const int val) {
			a.fill(val);
		}

		template <typename T, class TAllocator, class TDeleter >
		inline void zero (basic_array<T, TAllocator, TDeleter>& a) {
			a.fill(0);
		}


		template <typename T,
		          class TAllocator = memory::default_allocator,
				  class TDeleter = mn::memory::default_delete<T, TAllocator> >
		using array = basic_array<T, TAllocator, TDeleter>;
	}
}
#endif
