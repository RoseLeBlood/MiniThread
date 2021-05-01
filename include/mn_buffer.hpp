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
#ifndef __MINILIB_BASIC_BUFFER_H__
#define __MINILIB_BASIC_BUFFER_H__

#include "mn_config.hpp"

#include <cassert>

#include "mn_allocator.hpp"
#include "mn_typetraits.hpp"
#include "mn_algorithm.hpp"

namespace mn {

	/**
	 * @brief A buffer class that allocates a buffer of a given type and size.
	 * @note Useful for temporery buffering data.
	 */
	template <typename TVALUE, class TALLOCATOR = mn::memory::allocator_system_t>
	class buffer {
	public:
		using self_type = buffer<TVALUE, TALLOCATOR>;
		using value_type = TVALUE;
		using allocator = TALLOCATOR;
		using pointer = value_type*;
		using reference = value_type&;
		using size_type = mn::size_t;

		using const_reference = const value_type&;
		using const_pointer = const value_type*;

		using iterator_category = random_access_iterator_tag ;
		using difference_type = ptrdiff_t;
		using iterator = pointer;
		using const_iterator = const value_type*;

		/**
		 * @brief Consructs and allocates the Buffer.
		 * @param size The size of the buffer
		 */
		buffer(const size_type& size)
			: m_sSize(size), m_sUsed(size), m_pRawBuffer(0), m_bOwnMem(true), m_allocator()  {
			init_internal_buffer(size);
		}

		/**
		 * @brief Consructs the buffer from a given  memory pointer
		 * @param size The size of the buffer
		 * @param buffer The using pointer for this buffer
		 * @note This pointer not deallocated, from buffer on deconstruction
		 */
		buffer(pointer buffer, size_type size)
			: m_sSize(size), m_sUsed(size), m_pRawBuffer(buffer), m_bOwnMem(false), m_allocator()  { }


		/**
		 * @brief Consructs and allocates the Buffer. copies the contents of
		 * the supplied memory into the buffer
		 * @param size The size of the buffer
		 * @param buffer The using pointer for this buffer
		 */
		buffer(const_pointer buffer, size_type size)
			: m_sSize(size), m_sUsed(size), m_pRawBuffer(0), m_bOwnMem(true), m_allocator()  {
			init_internal_buffer(buffer);
		}


		/**
		 * @brief Move constructor.
		 */
		buffer(const self_type& other)
			: m_sSize(other.m_sSize), m_sUsed(other.m_sUsed), m_pRawBuffer(0), m_bOwnMem(true), m_allocator() {
			init_internal_buffer(other.m_pRawBuffer);
		}

		/**
		 * @brief Deconstrut the buffer. Is allocated the memory by buffer, then deallocated it
		 */
		~buffer() {
			destroy_internal_buffer();
		}

		/**
		 * @brief Get the iterator to the beginning of the buffer.
		 * @return The iterator to the beginning of the buffer.
		 */
		iterator begin() 			{ return m_pRawBuffer; }

		/**
		 * @brief Get the iterator to the beginning of the buffer.
		 * @return The iterator to the beginning of the buffer.
		 */
		const_iterator begin() const { return m_pRawBuffer; }

		/**
		 * @brief Get the iterator to end of the buffer.
		 * @return The iterator to end of the buffer.
		 */
		iterator end() 				{ return m_pRawBuffer; }

		/**
		 * @brief Get the iterator to end of the buffer.
		 * @return The iterator to end of the buffer.
		 */
		const_iterator end() const 	{ return m_pRawBuffer; }

		/**
		 * @brief Resizes this buffer and appends the given data.
		 * @param pBuffer The array of data to append.
		 * @param sBufferSize The number elements in the array.
		 */
		void append(const_pointer pBuffer, std::size_t sBufferSize) {
			if (sBufferSize == 0) return;
			if(resize(m_sUsed + sBufferSize, true))
				memcpy(m_pRawBuffer + m_sUsed - sBufferSize, pBuffer, sBufferSize * sizeof(value_type));
		}

		/**
		 * @brief Resizes this buffer and appends the given data.
		 * @param value The value to append.
		 */
		void append(value_type value) {
			if(resize(m_sUsed + 1, true))
				m_pRawBuffer[m_sUsed - 1] = value;
		}

		/**
		 * @brief Resizes this buffer and appends the given data.
		 * @param refBuffer The other buffer to append.
		 */
		void append(const self_type& refBuffer) {
			append(refBuffer.begin(), refBuffer.used());
		}

		/**
		 * @brief Resizes the buffer capacity and size.
		 * @note Externally memory can not resize.
		 *
		 * @param newSize The new size for this buffer.
		 * @param bReserve If true then the content of the old buffer is copied over to the new buffer.
		 */
		bool resize(size_type newSize, bool bReserve = true) {
			if(!m_bOwnMem) return false;
			if(newSize > m_sSize) {
				pointer __pNewRawBuffer = m_allocator.alloc(newSize * sizeof(value_type));
				if(__pNewRawBuffer == 0) return false;

				if(bReserve) memcpy(__pNewRawBuffer, m_pRawBuffer, m_sUsed * sizeof(value_type));

				m_allocator.free(m_pRawBuffer);
				m_pRawBuffer = __pNewRawBuffer;
				m_sSize = newSize;
			}
			m_sUsed = newSize;

			return true;
		}

		/**
		 * @brief Change the buffer size.
		 * @note Externally memory can not change.
		 *
		 * @param newSize The new size for this buffer.
		 * @param bReserve If true then the content of the old buffer is copied over to the new buffer.
		 */
		bool change_size(size_type newSize, bool bReserve = true) {
			if(!m_bOwnMem) return false;
			if (newSize == m_sSize) return true;

			pointer __pNewRawBuffer = NULL;
			if(newSize > 0) {
				__pNewRawBuffer = m_allocator.alloc(newSize * sizeof(value_type));
				if(bReserve)  memcpy(__pNewRawBuffer, m_pRawBuffer,
									 (m_sUsed < newSize ? m_sUsed : newSize) * sizeof(value_type));
			}

			m_allocator.free(m_pRawBuffer);
			m_pRawBuffer = __pNewRawBuffer;
			m_sSize = newSize;

			if (newSize < m_sUsed) m_sUsed = newSize;

		}

		/**
		 * @brief Assigns the argument buffer to this buffer.
		 * @note If necessary, resizes the buffer.
		 */
		void assign(const_pointer pBuffer, size_type size) {
			if (0 == size) return;
			if (size > m_sSize) resize(size, false);

			memcpy(m_pRawBuffer, pBuffer, size * sizeof(value_type));
			m_sUsed = size;
		}

		/**
		 * @brief Clear the used content.
		 */
		void clear() {
			memset(m_pRawBuffer, 0, m_sUsed * sizeof(value_type));
		}
		/**
		 * @brief Is the given buffer equel with this?
		 * @param other Reference to the comparing buffer
		 * @return true The given buffer is equel with this, false when not.
		 */
		bool is_equel(const buffer& other) const {
			if(m_sSize != other.m_sSize) return false;
			if(m_sUsed != other.m_sUsed) return false;

			return (memcmp(m_pRawBuffer, other.m_pRawBuffer, m_sUsed * sizeof(value_type)) == 0);
		}

		/**
		 * @brief is the buffer  empty?
		 * @return If true then is the buffer empty and if false then not.
		 */
		bool is_empty() const				{ return m_sUsed == 0; }

		/**
		 * @brief is the buffer  full?
		 * @return If true then is the buffer full and if false then not.
		 */
		bool is_full() const 				{ return m_sUsed == m_sSize; }
		/**
		 * @brief Get the allocated memory size in elements.
		 * @return The allocated memory size in elements.
		 */
		size_type get_size() const	 		{ return m_sSize; }

		/**
		 * @brief Get the allocated memory size in bytes.
		 * @return The allocated memory size in bytes.
		 */
		size_type get_size_bytes() const 	{ return m_sSize * sizeof(value_type); }

		/**
		 * @brief Get the used size of the buffer in elements.
		 * @return The used size of the buffer in elements.
		 */
		size_type get_used() const 			{ return m_sUsed; }

		/**
		 * @brief Get the used size of the buffer in bytes.
		 * @return The used size of the buffer in bytes.
		 */
		size_type get_used_bytes() const 	{ return m_sUsed * sizeof(value_type); }

		/**
		 * @brief Compare operator.
		 */
		bool operator == (const buffer& other) const {
			return is_equel(other);
		}
		/**
		 * @brief Not compare operator.
		 */
		bool operator != (const buffer& other) const {
			return !(is_equel(other));
		}

		reference operator [] (size_type index) {
			assert (index < m_sUsed);
			return m_pRawBuffer[index];
		}

		const_reference operator [] (size_type index) const {
			assert (index < m_sUsed);
			return m_pRawBuffer[index];
		}
	private:
		/**
		 *
		 *
		 */
		void init_internal_buffer() {
			if(m_sSize > 0) m_allocator.calloc(m_sSize, sizeof(value_type), &m_pRawBuffer);
		}

		/**
		 *
		 *
		 */
		void init_internal_buffer(const pointer buffer) {
			if(m_sSize > 0) {
				m_pRawBuffer = m_allocator.alloc(m_sSize * sizeof(value_type), &m_pRawBuffer);
				memcpy(m_pRawBuffer, buffer, m_sUsed * sizeof(value_type));
			}
		}

		void destroy_internal_buffer() {
			if(m_bOwnMem && (m_pRawBuffer != 0))
				m_allocator.free(m_pRawBuffer);
		}
	private:

		size_type m_sSize;
		size_type m_sUsed;
		pointer   m_pRawBuffer;
		bool      m_bOwnMem;
		allocator m_allocator;
	};

}

#endif // __MINILIB_BASIC_BUFFER_H__
