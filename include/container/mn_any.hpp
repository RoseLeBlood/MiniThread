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
#ifndef __MINILIB_CONTAINER_ANY_H__
#define __MINILIB_CONTAINER_ANY_H__

#include "../mn_config.hpp"

# include <cassert>
# include <typeinfo>

#include "mn_algorithm.hpp"
#include "../mn_allocator.hpp"
#include "../mn_def.hpp"
#include "../mn_functional.hpp"
#include "../mn_typetraits.hpp"

#include "../utils/mn_utils.hpp"



namespace mn {
	namespace container {
		/**
		 * @brief An Any class represents a general type and is capable of storing any type.
		 * @note Code taken from https://github.com/martinmoene/any-lite. Original copyright by Martin Moene.
		 * Distributed under the Boost Software License, Version 1.0.
		 *
		 * @note Modified for MiniThread
		 */
		class basic_any {
			/**
			 * @brief Interface class for the holder.
			 */
			class basic_any_holder {
			public:
				virtual std::type_info const & type() const = 0;
				virtual iholder * clone() const = 0;
			};

			/**
			 * @brief The real impl of the basic_any holder.
			 */
			template< typename T >
			class basic_any_holder_impl : public basic_any_holder {
			public:
				static const int TYPE_SIZE() { return sizeof(T); }

				basic_any_holder_impl( T const & value ) : m_typeHold( value ) {}
				basic_any_holder_impl( T && value ) : m_typeHold( mn::move( value ) ) {}


				/**
				 * @brief Get the type info of the given type.
				 * @return the Type info of the given type.
				 */
				virtual std::type_info const & type() const override {
					return typeid( T );
				}

				/**
				 * @brief Get a clone from this.
				 * @return A clone from this.
				 */
				virtual basic_any_holder* clone() const override {
					return new basic_any_holder_impl( m_typeHold );
				}

				/**
				 * @brief Get the stored value.
				 * @return The stored value.
				 */
				T get() { return m_typeHold; }
				/**
				 * @brief Get the stored value.
				 * @return The stored value.
				 */
				const T get() const { return m_typeHold; }

				/**
				 * @brief Convert this to the type.
				 */
				operator T() { return get(); }
			private:
				/**
				 * The stored value.
				 */
				T m_typeHold;
			};

		public:
			using self_type = basic_any<TAllocator>;
			using holder_type = basic_any_holder;

			/**
			 * @brief Construct an empty basic_any type.
			 */
			constexpr basic_any() noexcept
    			: m_asyContent( nullptr ) {}

			/**
			 * @brief Construct an any which stores the init parameter inside.
			 */
			template< class T >
			basic_any(const T & other ) : m_asyContent( nullptr ) {
				construct(other);
			}

			/**
			 * @brief Copy constructor, works with both empty and initialized Any values.
			 */
			basic_any( const self_type& other )
			 	: m_asyContent(nullptr )  {
					if ((this != &other) && !other.empty()) construct(other);
			}

			/**
			 * @brief Copy constructor, works with both empty and initialized Any values.
			 */
			basic_any( self_type && other ) noexcept
				: m_asyContent.content( mn::move( other.m_asyContent ) ) {
					other.m_asyContent = nullptr;
			}

			/**
			 * @brief Deconstructor of this basic_any object
			 */
			~basic_any() { deconstruct(true); }

			/**
			 * @brief Reset the basic_any
			 */
			void reset() noexcept {
				deconstruct(false);
			}

			/**
			 * @brief Swaps the content of the two Anys.
			 */
			self_type& swap( any & other ) noexcept {
				if (this == &other) return *this;

        		mn::swap( m_asyContent, other.m_asyContent );
        		return *this;
    		}

    		/**
			 * @brief Has the basic_any any value?
			 * @return True The basic_any is has any value and if false then not.
			 */
			bool has_value() const noexcept {
        		return m_asyContent != nullptr;
   			}

   			/**
			 * @brief if the basic_any  empty?
			 * @return True The basic_any is empty and if false then not.
			 */
   			bool is_empty() const noexcept {
        		return m_asyContent == nullptr;
   			}

   			/**
			 * @brief Returns the type information of the stored content.
			 * @return the type information of the stored content or when empty then the typeid from void.
			 */
   			const std::type_info & type() const noexcept {
				return has_value() ? m_asyContent->type() : typeid( void );
			}

			/**
			 * @brief Get the content as pointer.
			 * @return The content as pointer.
			 */
			template<class T>
			const T * to_pointer() const {
				auto* _content = static_cast<basic_any_holder_impl<T> *>( m_asyContent );
				return &( _content->get() );
			}

			/**
			 * @brief Get the content as pointer.
			 * @return The content as pointer.
			 */
			template< class T >
			T * to_pointer() {
				auto* _content = static_cast<basic_any_holder_impl<T> *>( m_asyContent );
				return &( _content->get() );
			}

			/**
			 * @brief Assignment operator for basic_any.
			 */
			self_type& operator=( const self_type& other ) noexcept {
        		self_type( other ).swap( *this );
        		return *this;
    		}

    		/**
			 * @brief Assignment operator for basic_any.
			 */
    		self_type& operator=( self_type && other ) noexcept {
				self_type( mn::move( other ) ).swap( *this );
				return *this;
			}

			/**
			 * @brief Assignment operator for all types.
			 */
			template<typename T>
			self_type& operator=(const T& other ) noexcept {
				construct(other);
				return *this;
			}

			/**
			 * @brief emplace
			 */
			template< class T, class... Args >
			void emplace( Args && ... args ) {
				self_type( T( mn::forward<Args>(args)... ) ).swap( *this );
			}
		private:
			/**
			 * @brief Helper function to construct the basic_any from other basic_any.
			 */
			void construct(const self_type& other) {
				if(other.has_value())
					m_asyContent = other.m_asyContent->clone() ;
				else
					m_asyContent = nullptr;
			}

			/**
			 * @brief Helper function to construct the basic_any from value.
			 */
			template< class T >
			void construct(const T & other) {
				m_asyContent = new  basic_any_holder_impl<T>(other) ;
			}

			/**
			 * @brief Helper function to deconstruct.
			 * @param bdelete Then free the content.
			 * @note only call with true in deconstructor.
			 */
			void deconstruct(bool bdelete = false) {
				mn::destruct<content_type>(m_asyContent);
				if(bdelete) delete m_asyContent;
			}
		private:
			/**
			 * @brief Holder of the value.
			 */
			holder_type	m_asyContent;
		};

		inline void swap( basic_any& x, basic_any& y ) noexcept {
			x.swap( y );
		}

		/**
		 * @brief Make a basic_any object.
		 * @return The moded basic_any object.
		 * @example basic_any x = make_any<int>(3);
		 */
		template< class T, class ...Args >
		inline basic_any make_any( Args&& ...args ) {
			return basic_any( mn::in_place_type<T>, mn::forward<Args>(args)...);
		}

		/**
		 * @brief Cast the extractet value from a basic_any* and return it as pointer.
		 * @return The extractet value from a basic_any* as pointer.
		 * @example int* pTmp = any_cast<int*>(_basicAnyObj).
		 */
		template< class T >
		inline const T* any_cast( const basic_any* other ) noexcept {
			if(other == nullptr) return nullptr;
			return operand->type() == typeid(T) ? operand->to_pointer<T>() : nullptr;
		}

		/**
		 * @brief Cast the extractet value from a basic_any* and return it as pointer.
		 * @return The extractet value from a basic_any* as pointer.
		 * @example int* pTmp = any_cast<int*>(_basicAnyObj).
		 */
		template<class T >
		inline T* any_cast(basic_any* other ) noexcept {
			if(other == nullptr) return nullptr;
			return operand->type() == typeid(T) ? operand->to_pointer<T>() : nullptr;
		}
	}
}
#endif // __MINILIB_CONTAINER_ANY_H__
