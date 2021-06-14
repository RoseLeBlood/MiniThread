/**
 * @file
 * @brief Basic tuple
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

#ifndef __MINILIB_BASIC_TUPLE_H__
#define __MINILIB_BASIC_TUPLE_H__

#include "../mn_config.hpp"
#include "../mn_algorithm.hpp"

namespace mn {
	namespace container {

		template <size_t N, typename T>
		class basic_tuple {
		public:
			using self_type = basic_tuple<N, T>;
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;
			using size_type = size_t;
			using iterator = pointer;
			using const_iterator = const pointer;
			using reverse_iterator = mn::reverse_iterator<iterator>;
			using const_reverse_iterator = const mn::reverse_iterator<iterator>;

			basic_tuple() {
				mn::fill_n (m_dDate, N, T());
			}

			basic_tuple(const self_type& t)
				: m_dDate(t.m_dDate) { }

			basic_tuple (mn::initializer_list<value_type> v) { assign (v); }
			basic_tuple(const_reference a)
				: basic_tuple(a, T(), T(), T()) { }
			basic_tuple(const_reference a, const_reference b)
				: basic_tuple(a, b, T(), T()) { }
			basic_tuple(const_reference a, const_reference b, const_reference c)
				 : basic_tuple(a, b, c, T()) { }
			basic_tuple(const_reference a, const_reference b, const_reference c, const_reference d) {
				m_dDate[0] = a; m_dDate[1] = b; m_dDate[2] = c;

				fill_n (m_dDate+3, N-3, d);
			}

			template <typename U>
			basic_tuple(const basic_tuple<N, U>& src) {
				mn::copy_n ( static_cast<T*>(src.m_dDate), N, m_dDate);
			}

			iterator 		begin ()			{ return m_dDate; }
			const_iterator	begin () const		{ return m_dDate; }

			iterator		end ()				{ return m_dDate + N; }
			const_iterator	end () const		{ return m_dDate + N; }

			size_type		size () const		{ return N; }
			size_type		max_size () const	{ return N; }

			bool			empty (void) const	{ return N == 0; }

			const_reference	at (size_type i) const	{ return m_dDate[i]; }
			reference		at (size_type i)		{ return m_dDate[i]; }


			void swap (self_type& v) {
				 for (uint32_t i = 0; i < N; ++ i)
				 	mn::swap (m_dDate[i], v.m_dDate[i]);
			}

			self_type& assign (mn::initializer_list<value_type> v) {
				const size_t _is_set = min (v.size(), N);
				mn::copy_n (v.begin(), _is_set, begin());
				mn::fill_n (begin()+_is_set, N-_is_set, T());
				return *this;
			}

			self_type& assign (const self_type& src) {
				mn::copy_n (src.begin(), N, begin());
				return *this;
			}


			template <typename U>
			self_type&	operator = (const tuple<N, U>& src) {
				mn::copy_n ( static_cast<T*>(src.m_dDate), N, m_dDate);

				return *this;
			}
			self_type&	operator = (const self_type& src) {
				return assign(src);
			}

			self_type&	operator = (mn::initializer_list<value_type> v) {
				return assign(v);
			}

			self_type&	operator += (mn::initializer_list<value_type> v) {
				for (uint32_t i = 0; i < min(N, v.size()); ++ i)
					m_dDate[i] += v.begin()[i];
				return *this;
			}

			self_type&	operator -= (mn::initializer_list<value_type> v) {
				for (uint32_t i = 0; i < min(N, v.size()); ++ i)
					_v[i] -= v.begin()[i];
				return *this;
			}

			self_type&	operator *= (mn::initializer_list<value_type> v) {
				for (uint32_t i = 0; i < min(N, v.size()); ++ i)
					m_dDate[i] *= v.begin()[i];
				return *this;
			}

			self_type&	operator /= (mn::initializer_list<value_type> v) {
				for (uint32_t i = 0; i < min(N, v.size()); ++ i)
					m_dDate[i] /= v.begin()[i];
				return *this;
			}


			self_type&	operator += (const_reference v) {
				for (uint32_t i = 0; i < N; ++ i)
					m_dDate[i] += v;
				return *this;
			}

			self_type&	operator -= (const_reference v) {
				for (uint32_t i = 0; i < N; ++ i)
					m_dDate[i] -= v;
				return *this;
			}

			self_type&	operator *= (const_reference v) {
				for (uint32_t i = 0; i < N; ++ i)
					m_dDate[i] *= v;
				return *this;
			}

			self_type&	operator /= (const_reference v) {
				for (uint32_t i = 0; i < N; ++ i)
					m_dDate[i] /= v;
				return *this;
			}

			self_type	operator + (const_reference v) {
				self_type _result;
				for (uint32_t i = 0; i < N; ++ i)
					_result[i] = m_dDate[i] + v;
				return _result;
			}

			self_type	operator - (const_reference v) {
				self_type _result;
				for (uint32_t i = 0; i < N; ++ i)
					_result[i] = m_dDate[i] - v;
				return _result;
			}

			self_type	operator * (const_reference v) {
				self_type _result;
				for (uint32_t i = 0; i < N; ++ i)
					_result[i] = m_dDate[i] * v;
				return _result;
			}

			self_type	operator / (const_reference v) {
				self_type _result;
				for (uint32_t i = 0; i < N; ++ i)
					_result[i] = m_dDate[i] / v;
				return _result;
			}

			const_reference	operator[] (size_type i) const	{ return m_dDate[i]; }
			reference		operator[] (size_type i)		{ return m_dDate[i]; }
		private:
			T	m_dDate[N];
		};

		template <size_t N, typename T, typename U>
		inline bool operator == (const basic_basic_tuple<N,T>& a, const basic_basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] != b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline bool operator != (const basic_basic_tuple<N,T>& a, const basic_basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] == b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline bool operator < (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] >= b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline bool operator > (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] <= b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline bool operator <= (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] > b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline bool operator >= (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				if (a[i] < b[i]) return false;
			return true;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T>& operator += (basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				a[i] = T(a[i] + b[i]);
			return a;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T>& operator -= (basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				a[i] = T(a[i] - b[i]);
			return a;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T>& operator *= (basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				a[i] = T(a[i] * b[i]);
			return a;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T>& operator /= (basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			for (uint32_t i = 0; i < N; ++ i)
				a[i] = T(a[i] / b[i]);
			return a;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T> operator + (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			basic_tuple<N,T> result;
			for (uint32_t i = 0; i < N; ++ i)
				result[i] = T(a[i] + b[i]);
			return result;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T> operator - (const basic_tuple<N, T>& a, const basic_tuple<N, U>& b) {
			basic_tuple<N,T> result;
			for (uint32_t i = 0; i < N; ++ i)
				result[i] = T(a[i] - b[i]);
			return result;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T> operator * (const basic_tuple<N, T>& a, const basic_tuple<N, U>& b) {
			basic_tuple<N,T> result;
			for (uint32_t i = 0; i < N; ++ i)
				result[i] = T(a[i] * b[i]);
			return result;
		}

		template <size_t N, typename T, typename U>
		inline basic_tuple<N,T> operator / (const basic_tuple<N,T>& a, const basic_tuple<N, U>& b) {
			basic_tuple<N,T> result;
			for (uint32_t i = 0; i < N; ++ i)
				result[i] = T(a[i] / b[i]);
			return result;
		}

		template <size_t N, typename T>
		using tuple = basic_tuple;
	}
}



#endif // __MINILIB_BASIC_TUPLE_H__
