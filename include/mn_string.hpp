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
#ifndef _MINLIB_836fa537_efd0_457c_af52_ad4ea8380a71_H_
#define _MINLIB_836fa537_efd0_457c_af52_ad4ea8380a71_H_

#include "mn_allocator.hpp"
#include "string/mn_basic_string.hpp"

namespace mn {
    using string = strings::basic_string<char, memory::default_allocator_t>;

#if MN_THREAD_CONFIG_BOARD ==  MN_THREAD_CONFIG_ESP32 
        /**
         * @brief  List type with allocated in SPI-RAM (8-Bit) 
         */
        using cps_string8_t = strings::basic_string<char, memory::allocator_psram_esp32_t >;
        /**
         * @brief  List type with allocated in SPI-RAM (32-Bit)
         */
        using cps_string32_t = strings::basic_string<char, memory::allocator_psram32_esp32_t >;
        /**
         * @brief  List type with allocated in internal ram (8-Bit) 
         */
        using cin_string8_t = strings::basic_string<char, memory::allocator_internal8_esp32_t >;
        /**
         * @brief  List type with allocated in intarnal ram (32-Bit)
         */
        using cin_string32_t = strings::basic_string<char, memory::allocator_internal32_esp32_t >;
        /**
         * @brief  List type with allocated in DMA section(8-Bit) 
         */
        using cdma_string_t = strings::basic_string<char, memory::allocator_internal_dma_esp32_t >;
#endif

    enum class flags_t {
        boolalpha =	0x0001,
        showbase  =	0x0002,
        showpoint =	0x0004,
        showpos =	0x0008,
        skipws =	0x0010,
        unitbuf =   0x0020, 
        uppercase =	0x0040, 
        dec =	0x0080,
        hex =	0x0100,
        oct =	0x0200,
        bin =	0x0400,
        internal =	0x0800, 
        hexbase = 	0x0100 | 0x0002
	};
    inline bool operator & (flags_t a, flags_t b){
        return ((int)a) & ((int)b);
    }

    namespace internal {

        template<typename E, class TAllocator, typename TStorage = strings::string_storage<E, TAllocator> >
        inline strings::basic_string<E, TAllocator, TStorage> __flagstostring(mn::flags_t f = mn::flags_t::dec ) {
            if (f & flags_t::dec)
                return strings::basic_string<E, TAllocator, TStorage>("d");
            else if (f & flags_t::hex && f & flags_t::showbase)
                return strings::basic_string<E, TAllocator, TStorage>("0x%X");
            else if (f & flags_t::hex)
                return strings::basic_string<E, TAllocator, TStorage>("%X");
            else if (f & flags_t::oct)
                {}//itoa(buf, 8, i);
            else if (f & flags_t::bin)
                return strings::basic_string<E, TAllocator, TStorage>("%b");
            return strings::basic_string<E, TAllocator, TStorage>("%d");
        }
        template<class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
        inline strings::basic_string<char, TAllocator, TStorage> __flagstostring(mn::flags_t f = mn::flags_t::dec ) {
            return __flagstostring<char, TAllocator, TStorage>(f); 
        }

        template<typename TStorage = strings::string_storage<char, memory::default_allocator_t> >
        inline strings::basic_string<char, memory::default_allocator_t, TStorage> __flagstostring(mn::flags_t f = mn::flags_t::dec ) {
            return __flagstostring<char, memory::default_allocator_t, TStorage>(f); 
        }

    }

    /*template<typename E, class TAllocator, typename TStorage>
	struct hash<strings::basic_string<E, TAllocator, TStorage>> {
	    hash_value_t operator()(const basic_string<E, TAllocator, TStorage>& x) const 
	    {
                return hasher<default_hasher>::get().hash(x.c_str() ,x.length());
	    }
	};*/


    template<typename E, class TAllocator = memory::default_allocator_t,
             typename TStorage = strings::string_storage<char, TAllocator> >
    inline size_t frmstring( strings::basic_string<E, TAllocator, TStorage>& dest, const char* format, va_list arg) {
        size_t length;
        va_list apStrLen;
        va_copy(apStrLen, arg);
        length = vsnprintf(NULL, 0, format, apStrLen);
        va_end(apStrLen);
            
            
        if (length > 0) {
            dest.resize(length);
            vsnprintf(dest.data(), dest.length(), format, arg);
        }
            
        return length;
    }

	template<typename E, class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
	inline strings::basic_string<E, TAllocator, TStorage> frmstring( const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        strings::basic_string<E, TAllocator, TStorage> dest;
        frmstring<E, TAllocator, TStorage>(dest, format, ap);
        va_end(ap);
        return dest;
    }

    template<class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
	inline strings::basic_string<char> frmstring( const char* format, ...) {
        va_list ap;
        va_start(ap, format);
        strings::basic_string<char, TAllocator, TStorage> dest;
        frmstring<char, TAllocator, TStorage>(dest, format, ap);
        va_end(ap);
        return dest;
    }
        
       
    template<typename E = char, class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const int32_t v, flags_t f = flags_t::dec  ) { 
        strings::basic_string<E, TAllocator, TStorage> fmt = 
                internal::__flagstostring<E, TAllocator, TStorage >(f);
        return frmstring<E, TAllocator, TStorage>(fmt.c_str(), v); }
    
    template<typename E = char, class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const int64_t v, flags_t f = flags_t::dec  ) { 
        strings::basic_string<E, TAllocator, TStorage> fmt = 
                internal::__flagstostring<E, TAllocator, TStorage >(f);
        return frmstring<E, TAllocator, TStorage>(fmt.c_str(), v); }
    
    template<typename E = char, class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const uint32_t v, flags_t f = flags_t::dec  ) { 
        strings::basic_string<E, TAllocator, TStorage> fmt = 
                internal::__flagstostring<E, TAllocator, TStorage >(f);
        return frmstring<E, TAllocator, TStorage>(fmt.c_str(), v); }
    
    template<typename E = char, class TAllocator = memory::default_allocator_t, typename TStorage = strings::string_storage<char, TAllocator> >
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const uint64_t v, flags_t f = flags_t::dec  ) { 
        strings::basic_string<E, TAllocator, TStorage> fmt = 
                internal::__flagstostring<E, TAllocator, TStorage >(f);
        return frmstring<E, TAllocator, TStorage>(fmt.c_str(), v); }
    
    template<typename E = char, class TAllocator, typename TStorage>
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const float v, flags_t f = flags_t::dec  ) {  
        return frmstring("%f", v); }
    
    template<typename E = char, class TAllocator, typename TStorage>
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const double v, flags_t f = flags_t::dec  ) {  
        return frmstring("%f", v); }
    
    template<typename E = char, class TAllocator, typename TStorage>
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const bool v, flags_t f = flags_t::dec  ) {   
        return frmstring("%s", (v ? "true" : "false") ); }
    
    template<typename E = char, class TAllocator, typename TStorage>
    inline strings::basic_string<E, TAllocator, TStorage> to_string(const void* v, flags_t f = flags_t::dec  ) {   
        return frmstring("%p", v ); }
}

#endif