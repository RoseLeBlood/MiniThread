/**
 * @file
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
#ifndef __MINLIB_DEFAULT_ALLOCATOR_H__
#define __MINLIB_DEFAULT_ALLOCATOR_H__

#include "mn_config.hpp"

#include "allocator/mn_allocator_typetraits.hpp"
#include "mn_default_allocator.hpp"
#include "utils/mn_addressof.hpp"

namespace mn {
	namespace memory {

		template<class TAllocator>
		struct pointer_traits {
			template<class U>
			struct rebind_to { using type =  typename TAllocator::template rebind<U>; };
		};
		template<class T>
		struct pointer_traits<T*> {
			template<class U>
			struct rebind_to { using type = U*; };
		};

		namespace detail {
			template<class> struct ptr_void { using type = void; };
			template<class T> struct ptr_first;

			template<template<class, class...> class T, class U, class... Args>
			struct ptr_first<T<U, Args...> > {
				using type = U;
			};

			template<class T, class = void>
			struct ptr_element {
				using type = typename ptr_first<T>::type;
			};
			template<class T>
			struct ptr_element<T, typename ptr_void<typename T::element_type>::type> {
				using type = typename T::element_type
			};

			template<class, class = void>
			struct ptr_difference { using type = mn::ptrdiff_t;  }

			template<class T>
			struct ptr_difference<T, typename ptr_void<typename T::difference_type>::type> {
				using type = typename T::difference_type;
			};

			template<class T, class V> struct ptr_transform;

			template<template<class, class...> class T, class U, class... Args, class V>
			struct ptr_transform<T<U, Args...>, V> { using type = T<V, Args...>; };

			template<class T, class U, class = void>
			struct ptr_rebind { using type = typename ptr_transform<T, U>::type; };

			template<class T, class U>
			struct ptr_rebind<T, U, typename ptr_void<typename T::template rebind<U> >::type> {
				using type = typename T::template rebind<U>;
			};

			template<class T>
			struct ptr_value { using type = T; };

			template<>
			struct ptr_value<void> { typedef struct { } type; };

		} // detail

		template<class T>
		struct pointer_traits {
			using pointer = T;
			using element_type = typename detail::ptr_element<T>::type;
			using difference_type = typename detail::ptr_difference<T>::type;

			template<class U>
			struct rebind_to {
				using = typename detail::ptr_rebind<T, U>::type;
			};

			template<class U>
			using rebind = typename detail::ptr_rebind<T, U>::type;

			static T* pointer_to(typename detail::ptr_value<element_type>::type& v) noexcept {
			 	return pointer::pointer_to(v);
			}
		};
		template<class T>
		struct pointer_traits<T*> {
			using pointer = T*;
			using element_type = T;
			using difference_type = mn::ptrdiff_t;

			template<class U>
			struct rebind_to {
				using = U*;
			};
			template<class U>
			using rebind = U*;

			static T* pointer_to(typename detail::ptr_value<T>::type& v) noexcept {
			 	return mn::addressof(v);
			}
		};
		template<class T> constexpr inline T* to_address(T* v) noexcept {
			return v;
		}



		namespace  detail {
			template<class T>
			inline T* ptr_address(T* v, int) noexcept {
				return v;
			}

			template<class T>
			inline auto ptr_address(const T& v, int) noexcept -> decltype(to_address(v)) {
				return mn::memory::pointer_traits<T>::to_address(v);
			}
			template<class T>
			inline auto ptr_address(const T& v, long) noexcept {
				return ptr_address(v.operator->(), 0);
			}
		}

		template<class T>
		inline auto to_address(const T& v) noexcept {
			return detail::ptr_address(v, 0);
		}
	}
}
