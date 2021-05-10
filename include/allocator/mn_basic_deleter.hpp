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

namespace mn {
	namespace memory {
		 template<typename Type, class TAllocator>
        class basic_deleter  {
        public:
			using value_type  = Type;
			using allocator = TAllocator;
			using pointer = TAllocator*;
			using reference = TAllocator&;

			constexpr basic_deleter() : m_refAllocator(nullptr) {}
			constexpr basic_deleter(reference alloc) noexcept
            	: m_refAllocator(&alloc) { }

            reference get_allocator() {
				return *m_refAllocator;
            }

            bool is_valid() const noexcept {
				return m_refAllocator != nullptr;
			}

			void operator()(value_type* pointer) noexcept {
                m_refAllocator.deallocate(pointer, sizeof(value_type), alignof(value_type));
            }
		private:
			pointer m_refAllocator;
        };

        template<typename Type, class TAllocator>
        class basic_deleter<Type[], TAllocator> {
        public:
			using value_type  = Type;
			using allocator = TAllocator;
			using pointer = TAllocator*;
			using reference = TAllocator&;

			constexpr basic_deleter() : m_refAllocator(nullptr), m_sArraySize(0u) {}
			constexpr basic_deleter(reference alloc, size_t size) noexcept
            	: m_refAllocator(&alloc), m_sArraySize(size) { }

			bool is_valid() const noexcept {
				return m_refAllocator != nullptr;
			}

			reference get_allocator() {
				return *m_refAllocator;
            }

            size_t get_size() const {
            	return m_sArraySize;
            }

			void operator()(value_type* pointer) noexcept {
                m_refAllocator.deallocate(pointer, m_sArraySize, sizeof(value_type), alignof(value_type));
            }
		private:
			pointer m_refAllocator;
			size_t m_sArraySize;
        };
	}
}


#endif // __MINLIB_BASIC_DELETER_H__
