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
#ifndef __MINILIB_BASIC_ALLOCATOR_MAXIMAL_FILTER_H__
#define __MINILIB_BASIC_ALLOCATOR_MAXIMAL_FILTER_H__

#include "../mn_config.hpp"


namespace mn {
	namespace memory {

		template <size_t TMaxAlloc>
		class basic_allocator_maximal_filter {
		public:
			bool on_preAlloc(size_t size) 	{ return get_left() >= size; }
			bool on_preDealloc(size_t size) { return true; }

			void on_alloc(size_t size) 		{ m_sCurrentAlloc += size; }
			void on_dealloc(size_t size) 	{ m_sCurrentAlloc -= size; }

			size_t get_left() 				{ return TMaxAlloc - m_sCurrentAlloc; }
			size_t get_current()			{ return m_sCurrentAlloc; }

			bool is_full()					{ return TMaxAlloc <= m_sCurrentAlloc; }
			bool is_empty()					{ return m_sCurrentAlloc == 0; }

		private:
			size_t m_sCurrentAlloc;
		};
	}
}

#endif // __MINILIB_BASIC_ALLOCATOR_MAXIMAL_FILTER_H__
