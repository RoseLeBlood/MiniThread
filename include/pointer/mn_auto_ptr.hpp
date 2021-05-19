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
#ifndef _MINILIB_a66be7cc_ab50_11eb_b276_b7700b99df9f_H_
#define _MINILIB_a66be7cc_ab50_11eb_b276_b7700b99df9f_H_

#include "../mn_config.hpp"

#include "../mn_algorithm.hpp"
#include "../mn_def.hpp"

namespace mn {
	 namespace pointer {

	 	/**
	 	 * @brief AutoPtr is a "smart" pointer for classes implementing
	 	 * reference counting based garbage collection.
	 	 *
	 	 */
		template <typename T>
		class basic_auto_ptr {
		public:
			using value_type = T;
			using element_type = T;
			using const_value_type = const value_type;
            using pointer = T*;
            using reference = T&;
            using self_type = basic_auto_ptr<T>;

			basic_auto_ptr()
				: m_ptr(nullptr) { }
			basic_auto_ptr(pointer ptr)
				: m_ptr(ptr) { }
			basic_auto_ptr(pointer ptr, bool shared)
				: m_ptr(ptr) { }
			basic_auto_ptr(const self_type& other )
				: m_ptr(other.m_ptr) { }
			basic_auto_ptr(basic_auto_ptr&& other) noexcept
				: m_ptr(mn::move(other.m_ptr)) { other.m_ptr = nullptr; }

			template <class TO>
			basic_auto_ptr(const basic_auto_ptr<TO>& other)
				: m_ptr(const_cast<TO*>(other.get())) { if (m_ptr) m_ptr->duplicate(); }

			~basic_auto_ptr() { if (m_ptr) m_ptr->release(); }

			template <class TO>
			void reset(const basic_auto_ptr<TO>& p)					{ assign<TO>(p); }
			void reset() 											{ if (m_ptr) { m_ptr->release(); m_ptr = nullptr; }	}
			void reset(pointer ptr) 								{ assign(ptr); }
			void reset(pointer ptr, bool shared)					{ assign(ptr, shared); }
			void reset(const self_type& ptr)						{ assign(ptr); }


			void 	swap(self_type& ptr)							{ mn::swap(m_ptr, ptr.m_ptr); }
			pointer get()											{ return m_ptr; }
			bool 	isNull() const									{ return m_ptr == nullptr; }

			pointer duplicate()										{ if (m_ptr) m_ptr->duplicate(); return m_ptr; }


			operator pointer() 										{ return m_ptr; }
			pointer operator ->()									{ return m_ptr; }
			bool operator ! () const								{ return m_ptr == nullptr; }

			template <class TO>
			self_type& operator = (const basic_auto_ptr<TO>& ptr) 	{ return assign<TO>(ptr); }
			self_type& operator = (pointer ptr)			 			{ return assign(ptr); }
			self_type& operator = (const self_type& ptr) 			{ return assign(ptr); }

			self_type& operator = (self_type&& other) noexcept {
				if (m_ptr) m_ptr->release();
				m_ptr = other.m_ptr;
				other.m_ptr = nullptr;
				return *this;
			}

			bool operator == (const self_type& other) const {
				return m_ptr == other.m_ptr;
			}
			bool operator == (const pointer other) const {
				return m_ptr == other;
			}
			bool operator != (const self_type& other) const {
				return m_ptr != other.m_ptr;
			}
			bool operator != (const pointer other) const {
				return m_ptr != other;
			}
			bool operator <= (const self_type& other) const {
				return m_ptr <= other.m_ptr;
			}
			bool operator <= (const pointer other) const {
				return m_ptr <= other;
			}
			bool operator >= (const self_type& other) const {
				return m_ptr >= other.m_ptr;
			}
			bool operator >= (const pointer other) const {
				return m_ptr >= other;
			}
			bool operator < (const self_type& other) const {
				return m_ptr < other.m_ptr;
			}
			bool operator < (const pointer other) const {
				return m_ptr < other;
			}
			bool operator > (const self_type& other) const {
				return m_ptr > other.m_ptr;
			}
			bool operator > (const pointer other) const {
				return m_ptr > other;
			}

			template <class TO>
			basic_auto_ptr<TO> dycast() const {

				TO* pOther = dynamic_cast<TO*>(m_ptr);
				return basic_auto_ptr<TO>(pOther, true);

			}
			template <class TO>
			basic_auto_ptr<TO> scast() const {

				TO* pOther = static_cast<TO*>(m_ptr);
				return basic_auto_ptr<TO>(pOther, true);

			}
		private:
			self_type& assign(pointer other) {
				if (m_ptr != other) {
					if (m_ptr) m_ptr->release();
					m_ptr = other;
				}
				return *this;
			}

			self_type& assign(pointer other, bool shared) {

				if (m_ptr != other) {
					if (m_ptr) m_ptr->release();
					m_ptr = other;
					if (shared && m_ptr) m_ptr->duplicate();
				}
				return *this;
			}

			self_type& assign(const self_type& other) {

				if (&other != this) {
					if (m_ptr) m_ptr->release();
					m_ptr = other.m_ptr;
					if (m_ptr) m_ptr->duplicate();
				}
				return *this;
			}

			template <class TO>
			self_type& assign(const basic_auto_ptr<TO>& other) {

				if (other.get() != m_ptr) {
					if (m_ptr) m_ptr->release();
					m_ptr = const_cast<TO*>(other.get());
					if (m_ptr) m_ptr->duplicate();
				}
				return *this;
			}

		private:
			pointer m_ptr;
		};

	 }
}

#endif // _MINILIB_a66be7cc_ab50_11eb_b276_b7700b99df9f_H_
