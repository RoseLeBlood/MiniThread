/** 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2021 Amber-Sophia Schroeck
 *
 * The Mini Thread Library is free software; you can redistribute it and/or modify  
 * it under the terms of the GNU Lesser General Public License as published by  
 * the Free Software Foundation, version 3, or (at your option) any later version.

 * The Mini Thread Library is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.  
*/

#ifndef _MINLIB_ALLOCATOR_H_
#define _MINLIB_ALLOCATOR_H_

#include "mn_allocator_system.hpp"
#include "mn_allocator_stack.hpp"

template<typename T, int TBytes> 
using allocator_stack_t = basic_allocator_stack<T, TBytes>;

template <typename T> using allocator_system_t = basic_allocator_system<T>;
template <typename T> using default_allocator_t = allocator_system_t<T>;

#include "mn_allocator_buffer.hpp"

template<typename T> 
using allocator_buffer_t = basic_allocator_buffer<T, default_allocator_t<T> >;

#endif