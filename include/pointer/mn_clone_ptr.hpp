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
#ifndef MINLIB_ASTL_CLONE_PTR_
#define MINLIB_ASTL_CLONE_PTR_

#include "../mn_config.hpp"

#include "../mn_algorithm.hpp"
#include "../mn_deleter.hpp"
#include <stdlib.h>



namespace mn {
    namespace pointer {

        template<typename T>
        struct basic_default_clone {
        protected:
            using pointer = T*;
            using value_type = T;
            using reference = T&;
            using const_reference = const T&;

			basic_default_clone() = default;

            pointer operator()(const_reference x ) const {
            	static_assert( ! mn::is_void<T>::value, "basic_default_clone cannot clone incomplete type");

            	return new T( x );
            }
            pointer operator()(T&& x ) const {
				return new T( mn::move( x ) );
            }
			template< class... Args >
			pointer operator()(Args&&... args ) const {
				return new T( mn::forward<Args>(args)...);
			}
			template< class U, class... Args >
    		pointer operator()(mn::initializer_list<U> il, Args&&... args ) const {
				 return new T( il, mn::forward<Args>(args)...);
    		}
        };

        template<typename T, class TCloner = basic_default_clone<T>, class TDeleter = memory::default_delete<T> >
        class basic_clone_ptr : TCloner, TDeleter {
        public:
            using value_type = T;
            using element_type = T;
            using pointer = T*;
            using reference = T&;
            using self_type = basic_clone_ptr<value_type >;
            using cloner_type = TCloner;
            using deleter_type = TDeleter;

            basic_clone_ptr() noexcept
            	: m_ptr( nullptr ) {}

			basic_clone_ptr( pointer p ) noexcept
				: m_ptr( p ) {}

			basic_clone_ptr(const cloner_type& cloner )
    			: cloner_type( cloner ), m_ptr( nullptr ) {}

			basic_clone_ptr(const deleter_type& deleter )
    			: deleter_type( deleter ), m_ptr( nullptr ) {}

			basic_clone_ptr(const cloner_type& cloner, const deleter_type& deleter )
    			: cloner_type( cloner ), deleter_type( deleter ), m_ptr( nullptr ) { }

			basic_clone_ptr(const cloner_type&& cloner )
    			: cloner_type( mn::move(cloner) ), m_ptr( nullptr ) {}

			basic_clone_ptr(const deleter_type&& deleter )
    			: deleter_type( mn::move(deleter) ), m_ptr( nullptr ) {}

			basic_clone_ptr(const cloner_type&& cloner, const deleter_type&& deleter )
    			: cloner_type( mn::move(cloner) ), deleter_type( mn::move(deleter) ), m_ptr( nullptr ) { }

    		basic_clone_ptr( const self_type& other )
    			: cloner_type ( other ),
    			  deleter_type( other ),
    			  m_ptr( other.m_ptr ? cloner_type()( *other.m_ptr ) : nullptr ) {}

			basic_clone_ptr(const self_type&& other )
				: cloner_type ( mn::move(other) ),
    			  deleter_type( mn::move(other) ),
    			  m_ptr( mn::move( other.m_ptr ) ) { }

			basic_clone_ptr(const value_type& value )
    			: m_ptr( cloner_type()( value ) ) {}

			basic_clone_ptr(const value_type&& value )
    			: m_ptr( cloner_type()( mn::move(value )) ) {}

			template< class... Args >
    		explicit basic_clone_ptr( Args&&... args )
    			: m_ptr( cloner_type()( mn::forward<Args>(args)...) ) {}

			template< class U, class... Args >
			explicit basic_clone_ptr( mn::initializer_list<U> il, Args&&... args )
				: m_ptr( cloner_type()(il, std::forward<Args>(args)...) ) { }

			basic_clone_ptr( const value_type& value, const cloner_type & cloner )
    			: cloner_type ( cloner  ) , m_ptr( cloner_type()( value ) ) {}

			basic_clone_ptr( const value_type&& value, const cloner_type && cloner )
    			: cloner_type ( mn::move(cloner)  ) , m_ptr( cloner_type()( mn::move(value ) ) ) {}

			basic_clone_ptr(const value_type& value, const cloner_type& cloner, const deleter_type& deleter )
    			: cloner_type ( cloner  ), deleter_type( deleter ), m_ptr( cloner_type()( value ) ) {}

			basic_clone_ptr(const value_type&& value, const cloner_type&& cloner, const deleter_type&& deleter )
    			: cloner_type ( mn::move(cloner)  ),
    			  deleter_type( mn::move(deleter) ),
    			  m_ptr( cloner_type()( mn::move(value) ) ) {}



            ~basic_clone_ptr() { deleter_type()( m_ptr ); }

            pointer get()                           { return m_ptr;  }

            cloner_type& get_cloner() noexcept 		{ return *this; }
    		deleter_type& get_deleter() noexcept 	{ return *this; }

            value_type operator *()                 { return *m_ptr; }
            pointer operator->()                    { return m_ptr; }


           	pointer release() noexcept {
        		pointer result = nullptr;
				mn::swap( result, m_ptr );
				return result;
    		}

			void reset(pointer p ) noexcept {
				get_deleter()( m_ptr );
				m_ptr = p;
			}

			void reset(const value_type& v ) {
				reset( get_cloner()( v ) );
			}
			void reset(const value_type&& v ) {
				reset( get_cloner()( mn::move(v) ) );
			}

			void swap(self_type& other ) noexcept {
        		mn::swap( m_ptr, other.m_ptr );
    		}
        private:
            pointer m_ptr;
        };

        template<typename T, class TCloner = basic_default_clone<T>, class TDeleter = memory::default_delete<T> >
        inline void swap(basic_clone_ptr<T, TCloner, TDeleter>& a, basic_clone_ptr<T, TCloner, TDeleter>& b ) {
        	a.swap(b);
        }

        template<typename T, class TCloner = basic_default_clone<T>, class TDeleter = memory::default_delete<T> >
        using clone_ptr = basic_clone_ptr<T, TCloner, TDeleter>;

        template<typename T>
        using default_clone = basic_default_clone<T>;

        /**
		 * @brief Make a clone pointer
		 * @tparam T Value type of the pointer.
		 * @tparam Args Argument for the object.
		 */
		template<typename T, typename... Args >
		inline clone_ptr<T> make_clone(Args&&... args) {
			return clone_ptr<T>(new T (mn::forward<Args>(args)...) );
		}
    }
}

#endif
