/**
 * @file
 * @brief
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

#ifndef __MINILIB_BASIC_VALUE_PTR_H__
#define __MINILIB_BASIC_VALUE_PTR_H__

#include "../mn_config.hpp"
#include "../mn_hash.hpp"

#include "mn_clone_ptr.hpp"


namespace mn {
    namespace pointer {

		template <typename T, class TCloner = basic_default_clone<T>, class TDeleter = memory::default_delete<T>>
		class basic_value_ptr {
		public:
			using value_type = T;
			using const_type = const T;
			using reference = T&;
			using const_reference = const T&;
			using pointer = T*;
			using const_pointer = const T*;

			using cloner_type = TCloner;
            using deleter_type = TDeleter;

			using self_type = basic_value_ptr<value_type, cloner_type, deleter_type>;

			basic_value_ptr() noexcept
    			: m_ptr( cloner_type(), deleter_type() ) {}

			basic_value_ptr(mn::nullptr_t) noexcept
    			: m_ptr( cloner_type(), deleter_type() ) {}

			basic_value_ptr( pointer p ) noexcept
    			: m_ptr( p ) { }

			basic_value_ptr( const cloner_type& cloner )
    			: m_ptr( cloner ) { }

			basic_value_ptr(const deleter_type& deleter )
    			: m_ptr( deleter ) { }

			basic_value_ptr(const cloner_type& cloner, const deleter_type& deleter )
    			: m_ptr( cloner, deleter ) { }

			basic_value_ptr(const cloner_type&& cloner )
    			: m_ptr( mn::move( cloner ) ) { }

			basic_value_ptr(const deleter_type&& deleter )
    			: m_ptr( mn::move( deleter ) ) { }

			basic_value_ptr(const cloner_type&& cloner, const deleter_type&& deleter )
    			: m_ptr( mn::move( cloner ), mn::move( deleter ) ) { }


    		basic_value_ptr(const self_type& other )
    			: m_ptr( other.m_ptr ) {}

			basic_value_ptr(const_reference value )
    			: m_ptr( value ) { }

			basic_value_ptr(const self_type&& other )
    			: m_ptr( mn::move(other.m_ptr) ) {}

			basic_value_ptr(const value_type&& value )
    			: m_ptr( mn::move(value )) { }

			template< class... Args>
			basic_value_ptr(Args&&... args)
				: m_ptr( mn::forward<Args>(args)...) { }

			template< typename U, class... Args>
			basic_value_ptr(mn::initializer_list<U> il, Args&&... args)
				: m_ptr(il, std::forward<Args>(args)...) { }

			template <typename V, class VClonerTDeleter>
    		basic_value_ptr( const V& value, const VClonerTDeleter & obj )
    			: m_ptr( value, obj ) {}

			template< typename V, class VCloner, class VDeleter >
			basic_value_ptr(const V& value, const VCloner& cloner, const VDeleter& deleter )
				: m_ptr(value, cloner, deleter) { }

			~basic_value_ptr() = default;

			pointer 		get() const noexcept 		{ return m_ptr.get();  }
			cloner_type& 	get_cloner() noexcept 		{ return m_ptr.get_cloner(); }
			deleter_type& 	get_deleter() noexcept		{ return m_ptr.get_deleter(); }

			bool 			has_value() const noexcept 	{ return !! get(); }
			const_reference	value() const				{ assert(has_value()); return *get(); }
			reference 		value()      				{ assert(has_value()); return *get(); }

			pointer 		release() noexcept			{ return m_ptr.release(); }

			void 			reset(pointer p = pointer() ) noexcept {
				m_ptr.reset( p );
			}

			void swap( basic_value_ptr & other ) noexcept {
				m_ptr.swap( other.m_ptr );
			}

			template< class U >
			value_type		value_or(U && v) {
				return has_value() ? *(m_ptr.get()) : static_cast<value_type>(mn::forward<U>( v ) );
			}


			template< class... Args >
    		void emplace( Args&&... args ) {
				m_ptr.reset( T( mn::forward<Args>(args)...) );
    		}

			template< class U, class... Args >
			void emplace( mn::initializer_list<U> il, Args&&... args ) {
				m_ptr.reset( T( il, std::forward<Args>(args)...) );
			}

			reference operator*()  const 			{ assert( get() != nullptr ); return *get(); }
			pointer   operator->() const noexcept	{ assert( get() != nullptr ); return  get(); }

			explicit operator bool() const noexcept { return has_value(); }

			self_type& operator = ( mn::nullptr_t ) noexcept {
				m_ptr.reset(); return *this;
			}

			self_type& operator = ( const_type value ) noexcept {
				m_ptr.reset(value); return *this;
			}

			self_type& operator = (const self_type& rhs ) {
				if ( this == &rhs )  return *this;

				if ( rhs ) { m_ptr.reset( *rhs ); }
				else { m_ptr.reset( nullptr ); }

				return *this;
			}

			self_type& operator = (self_type&& rhs ) noexcept {
				if ( this == &rhs )  return *this;

				swap(rhs);

				return *this;
			}

			template< class U,
				typename = typename mn::enable_if<mn::is_same< typename mn::decay<U>::type, T>::value >::type >
			self_type& operator = ( U && value ) {
				m_ptr.reset( mn::forward<U>( value ) ); return *this;
			}
		private:
			mn::pointer::clone_ptr<value_type, cloner_type, deleter_type> m_ptr;
		};

		template< class T >
		inline basic_value_ptr< typename mn::decay<T>::type > make_value( T && v ) {
			return basic_value_ptr< typename mn::decay<T>::type >( mn::forward<T>( v ) );
		}

		template< class T, class... Args >
		inline basic_value_ptr<T> make_value( Args&&... args ) {
			return basic_value_ptr<T>(mn::forward<Args>(args)...);
		}

		template< class T, class U, class... Args >
		inline basic_value_ptr<T> make_value( mn::initializer_list<U> il, Args&&... args ) {
			return basic_value_ptr<T>(il, mn::forward<Args>(args)...);
		}

		template< class T, class D, class C >
		inline void swap( basic_value_ptr<T, D, C> & a, basic_value_ptr<T, D, C> & b ) noexcept {
			a.swap( b );
		}

		template <typename T, class TCloner = basic_default_clone<T>, class TDeleter = memory::default_delete<T>>
		using value_ptr = basic_value_ptr<T, TCloner, TDeleter>;
	}
}

namespace mn {

	template< class T, class D, class C >
	struct hash< mn::pointer::value_ptr<T, D, C> > {
		using argument_type = mn::pointer::value_ptr<T, D, C>;
		using pointer = typename argument_type::pointer;

		result_type operator()( const argument_type& p ) const noexcept {
			return hash<pointer>()( p.get() );
		}
	};

} // namespace std


#endif // __MINILIB_BASIC_VALUE_PTR_H__
