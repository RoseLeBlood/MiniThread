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
#ifndef __MINILIB_BASIC_LIGHT_MAP_H__
#define __MINILIB_BASIC_LIGHT_MAP_H__

#include "../mn_config.hpp"

#include "../mn_algorithm.hpp"
#include "../mn_iterator.hpp"
#include "../mn_typetraits.hpp"

#include "mn_pair.hpp"
#include "mn_vector.hpp"


namespace mn {
	namespace container {

		/**
		 * @brief  Lightweight c++11 dictionary map implementation.
		 *
		 * @tparam TKey The type for the key.
		 * @tparam TValue The type for the value.
		 * @tparam TPairType The using type for the Key:Value-pair.
		 * @tparam TArrayType The using type for holding the pairs.
		 */
		template <class TKey, class TValue,
				  class TALLOCATOR = memory::default_allocator,
				  class TPairType = mn::container::pair<TKey, TValue>,
				  class TContainer = mn::container::vector<TPairType, TALLOCATOR> >
		class basic_light_map {
		public:
			using mapped_type = TValue;
			using key_type = TKey;

			using value_type = TPairType;
			using reference = TPairType&;
			using pointer = TPairType*;
			using const_reference = const TPairType&;
			using const_pointer = const TPairType*;

			using difference_type = mn::ptrdiff_t;
			using size_type = mn::size_t;

			using iterator = TValue*;
			using const_iterator = const TValue*;

			using self_type = basic_light_map < TKey, TValue, TALLOCATOR, TPairType, TContainer>;

			basic_light_map(const size_type start_size = 32) noexcept
				:  m_ayKeyValue(start_size) { }

			~basic_light_map() {
				m_ayKeyValue.clear();
			}


			void clear() {
				m_ayKeyValue.clear();
			}

			template< class... Args >
			mn::container::pair<iterator, bool> assign(const key_type& key, Args && ... args) {
				value_type _value(key, mn::forward<Args>(args)...);

				return assign(_value);
			}

			mn::container::pair<iterator, bool> assign(const value_type& vValue) {
				mn::container::pair<iterator, bool> _ret(vValue.seconde, false);

				for(typename TContainer::iterator it = m_ayKeyValue.begin();
							it != m_ayKeyValue.end(); it++) {
					value_type _pair = *it;

					if(_pair.first == vValue.first) {
						m_ayKeyValue.erase(it);
						m_ayKeyValue.insert(vValue);
						_ret->second = true;
					}
				}
				return _ret;
			}

			/**
			 * @brief Inserts a new element into the container constructed in-place with the given args if there is no
			 * 		element with the key in the container.
			 * @param key The key to emplace.
			 * @param args arguments to forward to the constructor of the element.
			 *
			 * @return Returns a pair consisting of an iterator to the inserted element,
			 *	and a bool denoting whether the insertion took place (true if insertion happened and false if not).
			 */
			template< class... Args >
			mn::container::pair<iterator, bool> emplace(const key_type& key, Args && ... args) {
				value_type _value = value_type(key, mn::forward<Args>(args)...);

				return insert(_value);
			}

			/**
			 * @brief Inserts value.
			 * @param value The element value to insert.
			 * @return  Returns a pair consisting of an iterator to the inserted element (or to the element that
						prevented the insertion) and a bool denoting whether the insertion took place.
			 */
			mn::container::pair<iterator, bool> insert( const value_type& value ) {
				mn::container::pair<iterator, bool> result(&value, false);

				if(find(value.first) == nullptr) {
					m_ayKeyValue.push_back(value);
					result.second = find(value.first) != nullptr;
				}

				return result;
			}

			template< class... Args >
			void insert_or_assign(const key_type& key, Args && ... args) {
				value_type _value(key, mn::forward<Args>(args)...);

				auto iter = find(key);
				if(iter == nullptr)
					insert(_value);
				else
					assign(_value);

			}

			/**
			 * @brief insert key_type key with mapped_type value.
			 * @return
			 *	 - True: The key doesn't exist, the data is added to the map
			 *	 - False: The key already exists, no change is made
			 */
			bool insert(const key_type& key, const mapped_type& value) {
				return insert(value_type(key, value)).second;
			}

			/**
			 * @brief insert key_type key with mapped_type value.
			 * @return
			 *	 - True: The key doesn't exist, the data is added to the map
			 *	 - False: The key already exists, no change is made
			 */
			bool insert(key_type&& key, mapped_type&& value) {
				return insert(value_type( mn::forward(key), mn::forward(value)) ).second;
			}




			/**
			 * @brief Removes specified elements from the container.
			 * @param pos Iterator to the element to remove.
			 * @return Return the the last removed iterator.
			 */
			iterator erase( const_iterator pos ) {
				return m_ayKeyValue.erase(pos);
			}

			/**
			 * @brief Removes specified elements from the container.
			 * @param first The start of the range of elements to remove.
			 * @param last The end of the range of elements to remove.
			 * @return Return the the last removed iterator.
			 */
			iterator erase( const_iterator first, const_iterator last ) {
				return m_ayKeyValue.erase(first, last);
			}
			/**
			 * @brief Removes the element with the key equivalent to tKey.
			 * @param tKey value of the elements to remove.
			 * @return Number of elements removed (0 or 1).
			 */
			size_type erase( const key_type& tKey ) {
				if(empty()) return 0;

				size_type _ret  = 0;

				for(typename TContainer::iterator it = m_ayKeyValue.begin();
							it != m_ayKeyValue.end(); it++) {
					value_type _pair = *it;

					if(_pair.first == tKey) {
						m_ayKeyValue.erase(it); _ret =  1;
					}
				}

				return _ret;
			}
			/**
			 * @brief Finds an element with key equivalent to key.
			 * @param tKey 	The key value of the element to search for.
			 * @return The associerte value with this key, when not exist then return the end();
			 */
			iterator find(const key_type& tKey) noexcept {
				if(empty()) return nullptr;

				for(typename TContainer::iterator it = m_ayKeyValue.begin();
							it != m_ayKeyValue.end(); it++) {
					value_type _pair = *it;

					if(_pair.first == tKey) {
						return &(_pair.second);
					}
				}

				return nullptr;
			}
			/**
			 * @brief Finds an element with key equivalent to key.
			 * @param tKey 	The key value of the element to search for.
			 * @return The associerte value with this key, when not exist then return the
			 * setted alternative value.
			 */
			const_iterator find(const key_type& tKey) const noexcept {
				if(empty()) return nullptr;

				for(typename TContainer::iterator it = m_ayKeyValue.begin();
							it != m_ayKeyValue.end(); it++) {
					value_type _pair = *it;

					if(_pair.first == tKey) {
						return &(_pair.second);
					}
				}

				return nullptr;
			}

			/**
			 * @brief Is the map empty.
			 * @return If true then is the map empty and if false then not.
			 */
			constexpr bool empty() const noexcept {
				return m_ayKeyValue.is_empty();
			}

			/**
			 * @brief Get the number of map-members.
			 * @return The number of map entries.
			 */
			constexpr size_type size() const noexcept {
				return m_ayKeyValue.get_used();
			}

			/**
			 * @brief Returns the number of elements with key key.
			 * @param key Value of the elements to count.
			 * @return Number of elements with key that compares equivalent to key, which is either 1 or 0.
			 */
			size_type count( const key_type& key ) const {
				return (find(key) != nullptr) ? 1 : 0;
			}


			/**
			 * @brief Exchanges the contents of the container with those of other.
			 * @param other Container to exchange the contents with.
			 */
			void swap( self_type& other ) {
				swap(m_ayKeyValue, other.m_ayKeyValue);
			}

			/**
			 * @brief Read value of map for given key.
			 * @param tKey The key.
			 * @return The associerte value with this key, when not exist then return the
			 * setted alternative value.
			 */
			iterator operator[](const key_type& tKey) noexcept {
				return find(tKey);
			}
			/**
			 * @brief Read value of map for given key.
			 * @param tKey The key.
			 * @return The associerte value with this key, when not exist then return the
			 * setted alternative value.
			 */
			const_iterator operator[](const key_type& tKey) const noexcept {
				return find(tKey);
			}
		private:
			TContainer m_ayKeyValue;
		};

		/**
		 * @brief A standard map container for storing a fixed size pairs.
		 * @tparam TKey The type for the key.
		 * @tparam TValue The type for the value.
		 * @tparam  N  Number of elements.
		 *
		 * @ingroup container
		 */
		template <class TKey, class TValue>
		using map = basic_light_map< TKey, TValue >;

	}
}

#endif // __MINILIB_BASIC_LIGHT_MAP_H__
