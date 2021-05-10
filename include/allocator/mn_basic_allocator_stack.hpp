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
#ifndef __MINILIB_ALLOCATOR_STACK_H__
#define __MINILIB_ALLOCATOR_STACK_H__

#include "../mn_config.hpp"

#include "mn_basic_allocator.hpp"
#include "mn_allocator_typetraits.hpp"

namespace mn {
	namespace memory {

		/**
         * @brief Stack based allocator.
         * @note - operates on buffer of TBUFFERSIZE bytes of stack memory
         * @note - never frees memory
         * @note - cannot be copied
         *
         * @author RoseLeBlood
         * @date 2021.02.21
         * @version 1.0
         */
        template <int TBUFFERSIZE>
		class basic_allocator_stack_impl {
		public:
			using allocator_category = std_allocator_tag();
			using is_thread_safe = std::false_type  ;

			static void first() noexcept { }

			static void* allocate(size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(alignment);

				if(m_bufferTop < TBUFFERSIZE) {
					if( (m_bufferTop+size) <= TBUFFERSIZE) {
						char* ret = &m_aBuffer[0] + m_bufferTop;
						m_bufferTop += size;

						return (void*)ret;
					}
				}
				return nullptr;
			}

			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {
				MN_UNUSED_VARIABLE(size);
				MN_UNUSED_VARIABLE(alignment);
				MN_UNUSED_VARIABLE(ptr);
			}

			static size_t max_node_size()  {
				return size_t(-1);
			}
			static size_t get_max_alocator_size()  {
				return __SIZE_MAX__;
			}
		private:
           	static size_t          m_bufferTop;
            static char*           m_aBuffer[TBUFFERSIZE];
		};

		template <int TBUFFERSIZE>
		size_t basic_allocator_stack_impl<TBUFFERSIZE>::m_bufferTop = 0;
		template <int TBUFFERSIZE>
		char* basic_allocator_stack_impl<TBUFFERSIZE>::m_aBuffer[TBUFFERSIZE];

		template <int TBUFFERSIZE, class TFilter = basic_allocator_filter>
		using stack_allocator = basic_allocator<basic_allocator_stack_impl<TBUFFERSIZE>, TFilter>;

	}
}



#endif // __MINILIB_ALLOCATOR_STACK_H__
