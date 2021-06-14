/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef _MINLIB_831159bd_3f35_4a00_8d46_f3fd737a5797_H_
#define _MINLIB_831159bd_3f35_4a00_8d46_f3fd737a5797_H_

#include "../mn_config.hpp"

#include "../mn_def.hpp"

namespace mn {
    namespace pointer {

        template < typename T, typename TRefType >
        class basic_shared_ptr   {
        public:
            using value_type = T;
            using element_type = T;
            using reference = T&;
            using const_value_type = const value_type;
            using pointer = value_type*;
            using ref_type = TRefType;

            using self_type = basic_shared_ptr<value_type, ref_type>;


            explicit basic_shared_ptr(pointer ptr )
                : m_ref(1), m_ptr(ptr)  { }

            basic_shared_ptr(const self_type& sp) {
                assert( (++m_ref != 0) );
                m_ptr = sp.m_ptr;
            }

            ~basic_shared_ptr() {
                if (--m_ref == 0) delete m_ptr;
            }

            pointer release() {
                pointer __px = this->get();
                if (--m_ref == 0) delete m_ptr;

                return __px;
            }
            void reset( pointer pValue = 0)
                { self_type(pValue).swap(*this); }

            ref_type ref() {
                return m_ref;
            }
            void swap(self_type& b) {
                mn::swap<pointer>(m_ptr, b.m_ptr);
                mn::swap<ref_type >(m_ref, b.m_ref);
            }

            pointer get() const {
		        return static_cast<T*>(m_ptr);
	        }

            pointer operator->() const {
                assert(get() != 0);
                return this->get();
            }
            const_value_type& operator*() {
                assert(get() != 0);
                return *this->get();
            }
            operator bool() {
                return m_ptr != 0;
            }

            self_type& operator = (self_type& sp) {
                release();
                m_ptr = sp.m_ptr;
                m_ref = sp.m_ref;
                return *this;
            }
        private:
            ref_type m_ref;
            pointer m_ptr;
        };

        template < typename T, typename TRefType >
        void swap(basic_shared_ptr<T, TRefType>& a, basic_shared_ptr<T, TRefType>& b) {
        	a.swap(b);
        }

        template < typename T >
		using shared_ptr = basic_shared_ptr<T, size_t>;

		template < typename T >
		using shared_atomic_ptr = basic_shared_ptr<T, atomic_size_t>;

		/**
		 * @brief Make a shared pointer
		 * @tparam T Value type of the pointer.
		 * @tparam Args Argument for the object.
		 */
		template<typename T, typename... Args >
		inline shared_ptr<T> make_shared(Args&&... args) {
			return shared_ptr<T>(new T (mn::forward<Args>(args)...) );
		}

		/**
		 * @brief Make a shared atomic pointer
		 * @tparam T Value type of the pointer.
		 * @tparam Args Argument for the object.
		 */
		template<typename T, typename... Args >
		inline shared_atomic_ptr<T> make_atomic_shared(Args&&... args) {
			return shared_atomic_ptr<T>(new T (mn::forward<Args>(args)...) );
		}
    }
}

#endif
