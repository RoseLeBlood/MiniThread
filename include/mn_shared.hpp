/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_SHARED_OBJECT_
#define MINLIB_ESP32_SHARED_OBJECT_

#include "mn_config.hpp"
#include "mn_autolock.hpp"

#include <stdint.h>
#include <esp_types.h>
#include <string>

namespace mn {
    /** 
     * Template class used to protect a shared resource with a Mutex.
     * @ingroup preview
     */
    template <class TOBJECT>
    class basic_shared_object {
    public:
        using object_t = TOBJECT;
        using ref_object_t = TOBJECT&;

        /** 
         * Resource constructor.
         * @param value sets the initial value of the resource.
         */
        basic_shared_object (const ref_object_t refValue) 
            : m_refValue(refValue)  { }
        /** 
         * Get the value of the shared resource.
         * @return The value of the shared resource.
         */
        ref_object_t get_object () const {
            automutx_t lock(m_pReadWriteLock);

            return m_refValue;
        }
        /** 
         * Sets the value of the shared resource with the specified new_value.
         * @param refNewValue The new value for this shared resource
         */
        void set_object (const ref_object_t refNewValue) {
            automutx_t lock(m_pReadWriteLock);
            m_refValue = refNewValue;
        }
        /**
         * Operator to set the value of the shared resource.
         * @param refNewValue The new value for this shared resource
         */ 
        void operator = (const ref_object_t refNewValue)  {
            set_object(refNewValue);
        }
        /**
         * Operator to get the value of the shared resource.
         * @return The value of the shared resource.
         */
        operator object_t () const {
            return get_object();
        }

    protected:
        /** 
         * the controll mutex
         */
        mutex_t m_pReadWriteLock;
        /** 
         * reference of the object
         */
        ref_object_t m_refValue;
    };



    template <class TOBJECT>
    using shared_object_t = basic_shared_object<TOBJECT>;

#if MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING == MN_THREAD_CONFIG_YES

    /// A shared object value of @c int8_t
    using shared_int8_t   = basic_shared_object<int8_t>;
    /// A shared object value of @c int16_t
    using shared_int16_t  = basic_shared_object<int16_t>;
    /// A shared object value of @c int32_t
    using shared_int32_t  = basic_shared_object<int32_t>;
    /// A shared object value of @c int64_t
    using shared_int64_t  = basic_shared_object<int64_t>;

    /// A shared object value of @c uint8_t
    using shared_uint8_t   = basic_shared_object<uint8_t>;
    /// A shared object value of @c uint16_t
    using shared_uint16_t  = basic_shared_object<uint16_t>;
    /// A shared object value of @c uint32_t
    using shared_uint32_t  = basic_shared_object<uint32_t>;
    /// A shared object value of @c uint64_t
    using shared_uint64_t  = basic_shared_object<uint64_t>;

    /// A shared object value of @c int8_fast_t
    using shared_fast_int8_t   = basic_shared_object<int_fast8_t>;
    /// A shared object value of @c int16_fast_t
    using shared_fast_int16_t  = basic_shared_object<int_fast16_t>;
    /// A shared object value of @c int32_fast_t
    using shared_fast_int32_t  = basic_shared_object<int_fast32_t>;
    /// A shared object value of @c int64_fast_t
    using shared_fast_int64_t  = basic_shared_object<int_fast64_t>;

    /// A shared object value of @c uint8_fast_t
    using shared_fast_uint8_t   = basic_shared_object<uint_fast8_t>;
    /// A shared object value of @c uint16_fast_t
    using shared_fast_uint16_t  = basic_shared_object<uint_fast16_t>;
    /// A shared object value of @c uint32_fast_t
    using shared_fast_uint32_t  = basic_shared_object<uint_fast32_t>;
    /// A shared object value of @c uint64_fast_t
    using shared_fast_uint64_t  = basic_shared_object<uint_fast64_t>;

    /// A shared object value of @c int_least8_t
    using shared_least_int8_t   = basic_shared_object<int_least8_t>;
    /// A shared object value of @c int_least16_t
    using shared_least_int16_t  = basic_shared_object<int_least16_t>;
    /// A shared object value of @c int_least32_t
    using shared_least_int32_t  = basic_shared_object<int_least32_t>;
    /// A shared object value of @c int_least64_t
    using shared_least_int64_t  = basic_shared_object<int_least64_t>;

    /// A shared object value of @c uint_least8_t
    using shared_least_uint8_t   = basic_shared_object<uint_least8_t>;
    /// A shared object value of @c uint_least16_t
    using shared_least_uint16_t  = basic_shared_object<uint_least16_t>;
    /// A shared object value of @c uint_least32_t
    using shared_least_uint32_t  = basic_shared_object<uint_least32_t>;
    /// A shared object value of @c uint_least64_t
    using shared_least_uint64_t  = basic_shared_object<uint_least64_t>;

    /// A shared object value of @c float
    using shared_float_t         = basic_shared_object<float>;
    /// A shared object value of @c double
    using shared_double_t        = basic_shared_object<double>;
    /// A shared object value of @c bool
    using shared_bool_t          = basic_shared_object<bool>;
    /// A shared object value of @c void*
    using shared_void_t          = basic_shared_object<void*>;
    /// A shared object value of @c size_t
    using shared_size_t          = basic_shared_object<size_t>;

    /// A shared object value of @c char
    using shared_char_t          = basic_shared_object<char>;
    /// A shared object value of @c std::string
    using shared_string_t        = basic_shared_object<std::string>;
    #ifdef _GLIBCXX_USE_WCHAR_T
    /// A shared object value of @c wchar_t
    using shared_wchar_t         = basic_shared_object<wchar_t>;
    /// A shared object value of @c std::wstring
    using shared_wstring_t       = basic_shared_object<std::wstring>;
    #endif //_GLIBCXX_USE_WCHAR_T

    #if ((__cplusplus >= 201103L) && defined(_GLIBCXX_USE_C99_STDINT_TR1))
    /// A shared char of @c char16_t
    using shared_char16_t   = basic_shared_object<char16_t>;
    /// A shared char of @c char32_t
    using shared_char32_t   = basic_shared_object<char32_t>;
    #endif //((__cplusplus >= 201103L) && defined(_GLIBCXX_USE_C99_STDINT_TR1))

}

#endif //MN_THREAD_CONFIG_SHAREDOBJECT_PREUSING == MN_THREAD_CONFIG_YES

#endif //MINLIB_ESP32_SHARED_OBJECT_