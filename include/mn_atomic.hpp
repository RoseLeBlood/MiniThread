/**
 * @file
 * @brief Basic atomics types
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_
#define _MINLIB_08365b25_a93f_4c7e_81f8_38958486f7b2_H_

#include "mn_config.hpp"

#include <stdint.h>

#include "mn_def.hpp"
#include "atomic/mn_atomic_flags.hpp"
#include "atomic/mn_atomic_primary_types.hpp"


namespace mn {


    template<typename T>
    using atomic_ptr            = _atomic_ptr<T*>;


    // Signad basic types
    using atomic_bool           = _atomic<bool>;
    using atomic_char	        = _atomic<char>;
    using atomic_schar	        = _atomic<signed char>;
    using atomic_short	        = _atomic<short>;
    using atomic_int	        = _atomic<int>;
    using atomic_long	        = _atomic<long>;
    using atomic_int_least8_t	= _atomic<int_least8_t>;
    using atomic_int_least16_t	= _atomic<int_least16_t>;
    using atomic_int_least32_t	= _atomic<int_least32_t>;
    using atomic_int_least64_t	= _atomic<int_least64_t>;
    using atomic_int_fast8_t	= _atomic<int_fast8_t>;
    using atomic_int_fast16_t	= _atomic<int_fast16_t>;
    using atomic_int_fast32_t	= _atomic<int_fast32_t>;
    using atomic_int_fast64_t	= _atomic<int_fast64_t>;
    using atomic_int8_t         = _atomic<int8_t>;
    using atomic_int16_t        = _atomic<int16_t>;
    using atomic_int32_t        = _atomic<int32_t>;
    using atomic_int64_t        = _atomic<int64_t>;

    // unsigned types
    using atomic_uchar	        = _atomic<unsigned char>;
    using atomic_ushort	        = _atomic<unsigned short>;
    using atomic_uint	        = _atomic<unsigned int>;
    using atomic_ulong	        = _atomic<unsigned long>;
    using atomic_uint_least8_t	= _atomic<uint_least8_t>;
    using atomic_uint_least16_t	= _atomic<uint_least16_t>;
    using atomic_uint_least32_t	= _atomic<uint_least32_t>;
    using atomic_uint_least64_t	= _atomic<uint_least64_t>;
    using atomic_uint_fast8_t	= _atomic<uint_fast8_t>;
    using atomic_uint_fast16_t	= _atomic<uint_fast16_t>;
    using atomic_uint_fast32_t	= _atomic<uint_fast32_t>;
    using atomic_uint_fast64_t	= _atomic<uint_fast64_t>;
    using atomic_uint8_t        = _atomic<uint8_t>;
    using atomic_uint16_t       = _atomic<uint16_t>;
    using atomic_uint32_t       = _atomic<uint32_t>;
    using atomic_uint64_t       = _atomic<uint64_t>;


    // Spzial types
    using atomic_intptr_t	    = _atomic<intptr_t>;
    using atomic_uintptr_t	    = _atomic<uintptr_t>;
    using atomic_size_t	        = _atomic<size_t>;
    using atomic_ptrdiff_t	    = _atomic<ptrdiff_t>;
    using atomic_intmax_t	    = _atomic<intmax_t>;
    using atomic_uintmax_t	    = _atomic<uintmax_t>;
    using atomic_char16_t   	= _atomic<char16_t>;
    using atomic_char32_t	    = _atomic<char32_t>;
    using atomic_wchar_t	    = _atomic<wchar_t>;
    using atomic_sig_t          = _atomic<int>;

	template <typename TAtomicType, TAtomicType TTrue = true, TAtomicType TFalse = false>
    class basic_atomic_flag  {
	public:
		using flag_type = TAtomicType;
		using reference = TAtomicType&;

		constexpr basic_atomic_flag() noexcept
			: m_bFlag(0) { }

		constexpr basic_atomic_flag(const flag_type& flag) noexcept
			: m_bFlag(flag) { }

		basic_atomic_flag(const basic_atomic_flag&) noexcept = delete;

		reference operator=(const reference)          noexcept = delete;
		reference operator=(const reference) volatile noexcept = delete;

		void clear(memory_order order = memory_order::SeqCst) noexcept {
			m_bFlag.store(TFalse, order);
		}
		flag_type test_and_set(memory_order order = memory_order::SeqCst) noexcept {
			return m_bFlag.exchange(TTrue, order);
		}
		flag_type test(memory_order order = memory_order::SeqCst) const {
			return m_bFlag.load(order);
		}
	private:
		_atomic<flag_type> m_bFlag;
    };

    using atomic_flag = basic_atomic_flag<bool>;
}


#endif
