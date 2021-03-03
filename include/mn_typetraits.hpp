/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>. -
*/
#ifndef _MINLIB_TYPE_TRAITS_H_
#define _MINLIB_TYPE_TRAITS_H_

/**
 * @author Amber-Sophia Schröck
 * @date 2021.02.14  
 */
namespace mn {
    template <class ...ts> struct make_void { typedef void type; };
    template <class ...ts> using void_t = typename make_void<ts ...>::type;
        
    template<class T, T v> struct integral_constant {
        enum { value = v  };
    };
    
    using  true_type = integral_constant<bool, true>;
    using false_type =  integral_constant<bool, false> ;
        
    template<typename T> 
    struct is_enum : public integral_constant<bool, __is_enum(T)> { };

    template<typename T> 
    struct is_union : public integral_constant<bool, __is_union(T)> { };

    template<typename T> struct is_class : public integral_constant<bool, __is_class(T)> { };
    template<typename> struct is_function : public false_type { };
    
    template<typename T> 
    struct is_empty : public integral_constant<bool, __is_empty(T)> { };
    
    template<typename T> 
    struct is_abstract : public integral_constant<bool, __is_abstract(T)> { };
    
    template<typename T> 
    struct is_polymorphic : public integral_constant<bool, __is_polymorphic(T)> { };
        
    template<typename T> 
    struct is_literal_type : public integral_constant<bool, __is_literal_type(T)> { };
        
    template<typename T> 
    struct is_pod : public integral_constant<bool, __is_pod(T)> { };
        
    

	template<typename T> 
    struct is_integral : public integral_constant<bool, false> { };

	template<class T> 
    struct is_integral<const T> : public is_integral<T> { };

    template<class T> 
    struct is_integral<volatile T> : public is_integral<T> { };

    template<class T> 
    struct is_integral<const volatile T> : public is_integral<T> { };
    
    

    template<typename T> 
    struct is_rational : public integral_constant<bool, false> { };

	template<class T> 
    struct is_rational<const T> : public is_rational<T> { };

    template<class T> 
    struct is_rational<volatile T> : public is_rational<T> { };

    template<class T> 
    struct is_rational<const volatile T> : public is_rational<T> { };

    template<typename T> 
    struct is_void : public integral_constant<bool, false> { };

    template<class T> 
    struct is_void<const T> : public is_void<T> { };

    template<class T> 
    struct is_void<volatile T> : public is_void<T> { };

    template<class T> 
    struct is_void<const volatile T> : public is_void<T> { };

    template<typename T> 
    struct is_floating_point : public integral_constant<bool, false> { };

    template<> 
    struct is_floating_point<float> : public integral_constant<bool, true> { };

    template<> 
    struct is_floating_point<double> : public integral_constant<bool, true> { };

    template<typename T> 
    struct is_pointer : public integral_constant<bool, false> { };

    template<typename T> 
    struct is_pointer<T*> : public integral_constant<bool, true> { };

    template<typename T> 
    struct is_fundamental : public integral_constant<bool, is_integral<T>::value || is_rational<T>::value> { };

    template<typename T> 
    struct has_trivial_constructor : public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T> 
    struct has_trivial_copy : public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T> 
    struct has_trivial_assign : public integral_constant<bool, is_fundamental<T>::value | is_pointer<T>::value | is_pod<T>::value> { };

    template<typename T> 
    struct has_trivial_destructor : public integral_constant<bool, is_fundamental<T>::value || is_pointer<T>::value || is_pod<T>::value> { };

    template<typename T> 
    struct has_cheap_compare : public integral_constant<bool, has_trivial_copy<T>::value && sizeof(T) <= 4 > { };

    #define MN_INTEGRAL(T) template<> \
    struct is_integral<T> : public integral_constant<bool, true> { };

    #define MN_RATIONAL(T) template<> \
    struct is_rational<T> : public integral_constant<bool, true> { };

    #define MN_VOIDTYPE(T) template<> \
    struct is_void<T> : public integral_constant<bool, true> { };

	MN_INTEGRAL(char);
	MN_INTEGRAL(unsigned char);
	MN_INTEGRAL(short);
	MN_INTEGRAL(unsigned short);
	MN_INTEGRAL(int);
	MN_INTEGRAL(unsigned int);
	MN_INTEGRAL(long);
	MN_INTEGRAL(unsigned long);
	MN_INTEGRAL(wchar_t);
    MN_INTEGRAL(bool);

    MN_VOIDTYPE(void);
    MN_VOIDTYPE(const void);
    MN_VOIDTYPE(volatile void );
    MN_VOIDTYPE(const volatile void );

    MN_RATIONAL(float);
    MN_RATIONAL(double);
    MN_RATIONAL(long double);

}


#endif