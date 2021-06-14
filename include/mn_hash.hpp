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
#ifndef __MINILIB_BASIC_HASH_H__
#define __MINILIB_BASIC_HASH_H__

#include "mn_config.hpp"

#include <stdint.h>

#include "mn_def.hpp"


namespace mn {
	using result_type = mn::size_t;

	/**
	 * @brief Default implementations, just casts to hash_value.
	 */
	template<typename T>
	result_type extract_int_key_value(const T& t) {
		return (result_type)t;
	}

	namespace internal {
		/**
		 * @brief Implementation of hasher.
		 * Works for keys that can be converted to 32-bit integer with extract_int_key_value.
		 * @note Algorithm by Robert Jenkins. (see http://www.cris.com/~Ttwang/tech/inthash.htm).
		 */
		template<typename T>
		result_type rjenkins_hash(const T& t) {
			result_type _hash = extract_int_key_value(t);

	 		_hash = (_hash + 0x7ed55d16) + (_hash << 12);
			_hash = (_hash ^ 0xc761c23c) ^ (_hash >> 19);
			_hash = (_hash + 0x165667b1) + (_hash << 5);
			_hash = (_hash + 0xd3a2646c) ^ (_hash << 9);
			_hash = (_hash + 0xfd7046c5) + (_hash << 3);
			_hash = (_hash ^ 0xb55a4f09) ^ (_hash >> 16);
			return static_cast<result_type>(_hash);
		}

		inline result_type rjenkins_hash_string(const char* strValue) {
			unsigned long _iRet = 0;
			while(*strValue) {
				_iRet += rjenkins_hash<char>(*strValue);
				++strValue;
			}
			return static_cast<result_type>(_iRet);
		}
	}
	/**
	 * @brief Default implementation of hasher.
	 */
	template<typename T>
	struct hash {
		const result_type operator()(T& t) const noexcept {
			return internal::rjenkins_hash(t);
		}
	};

	template<typename T>
    struct hash<T*>{
		result_type operator () (T* pPtr) const noexcept {
			assert(pPtr);
			auto _value = *pPtr;
			return internal::rjenkins_hash(_value);
		}
    };

  	template<>
    struct hash<char*>{
		const result_type operator()(const char* t) const noexcept {
			return internal::rjenkins_hash_string(t);
		}
    };

    template<>
    struct hash<const char*>{
		const result_type operator()(const char* t) const noexcept {
			return internal::rjenkins_hash_string(t);
		}
    };

    template<>
    struct hash<int8_t>{
		result_type operator () (int8_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };


	template<>
    struct hash<uint8_t>{
		result_type operator () (uint8_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };


	template<>
    struct hash<int16_t>{
		result_type operator () (int16_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };

	template<>
    struct hash<uint16_t>{
		result_type operator () (uint16_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };

	template<>
    struct hash<int32_t>{
		result_type operator () (int32_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };

	template<>
    struct hash<uint32_t>{
		result_type operator () (uint32_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };


	template<>
    struct hash<int64_t>{
		result_type operator () (int64_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };


	template<>
    struct hash<uint64_t>{
		result_type operator () (const uint64_t n) const noexcept {
			return static_cast<result_type>(n) * MN_THREAD_CONFIG_BASIC_HASHMUL_VAL;
		}
    };

    template <class T>
	struct hash_function {
		result_type operator () (T key, size_t maxValue) const noexcept {
			return mn::hash<T>{}(key) % maxValue;
		}
	};


	template <>
	struct hash_function<const char*> {
		result_type operator () (const char* key, size_t maxValue) const noexcept {
			return mn::hash<const char*>{}(key) % maxValue;
		}
	};
}

#endif // __MINILIB_BASIC_HASH_H__
