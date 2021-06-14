/**
 * @file
 * @brief The template and inlines for the numeric_limits classes. -*- C++ -*-
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

#ifndef __MINILIB_NUMERIC_LIMITS_H__
#define __MINILIB_NUMERIC_LIMITS_H__

#include "mn_config.hpp"

#include "mn_functional.hpp"
#include "mn_void.hpp"



namespace mn {
	/**
	 *  @brief Describes the rounding style for floating-point types.
	 */
	enum class float_round_style {
		indeterminate       = -1,    /// Intermediate.
		toward_zero         = 0,     /// To zero.
		to_nearest          = 1,     /// To the nearest representable value.
		toward_infinity     = 2,     /// To infinity.
		toward_neg_infinity = 3      /// To negative infinity.
	};

	/**
	 *  @brief Describes the denormalization for floating-point types.
	 */
	enum class float_denorm_style {
		/// Indeterminate at compile time whether denormalized values are allowed.
		indeterminate = -1,
		/// The type does not allow denormalized values.
		absent        = 0,
		/// The type allows denormalized values.
		present       = 1
	};

	template <typename T>
	class numeric_limits {
	public:
		using value_type = T;

  		static constexpr value_type bits() 			{ return 8 * (sizeof(T) / sizeof(char)); }

  		static constexpr value_type min()        	{ return value_type(); }
		static constexpr value_type max()        	{ return value_type(); }

		static constexpr value_type lowest() 		{ return value_type(); }
		static constexpr value_type epsilon() 		{ return value_type(); }
		static constexpr value_type round_error() 	{ return value_type(); }
		static constexpr value_type denorm_min() 	{ return value_type(); }
		static constexpr value_type infinity() 		{ return value_type(); }
		static constexpr value_type quiet_NaN() 	{ return value_type(); }
		static constexpr value_type signaling_NaN() { return value_type(); }

		static constexpr 	int  digits    			= 0;
		static constexpr 	int  digits10  			= 0;
		static constexpr 	bool is_signed 			= false;
  		static constexpr 	bool is_specialized    	= true;
		static constexpr 	bool is_integer        	= true;
		static constexpr 	bool is_exact          	= true;
		static constexpr 	int  max_digits10      	= 0;
		static constexpr 	int  radix             	= 2;
		static constexpr 	int  min_exponent      	= 0;
		static constexpr 	int  min_exponent10    	= 0;
		static constexpr 	int  max_exponent      	= 0;
		static constexpr 	int  max_exponent10    	= 0;
		static constexpr 	bool has_infinity      	= false;
		static constexpr 	bool has_quiet_NaN     	= false;
		static constexpr 	bool has_signaling_NaN 	= false;
		static constexpr 	bool has_denorm_loss   	= false;
		static constexpr 	bool is_iec559         	= false;
		static constexpr 	bool is_bounded        	= true;
		static constexpr 	bool traps             	= false;
		static constexpr 	bool tinyness_before   	= false;
		static constexpr 	float_denorm_style has_denorm = float_denorm_style::absent;
		static constexpr 	float_round_style round_style = float_round_style::toward_zero;
	};

	template <typename T>
	class numeric_limits<const T> : public numeric_limits<T> { };

	template <typename T>
	class numeric_limits<volatile T> : public numeric_limits<T> { };

	template <typename T>
	class numeric_limits<const volatile T> : public numeric_limits<T> { };


	/**
	 * @brief Spezial version for bool
	 */
	template<>
  	class numeric_limits<bool>  {
	public:
  		using value_type = bool;

		static constexpr value_type min()        	{ return false; }
		static constexpr value_type max()        	{ return true; }

		static constexpr value_type lowest() 		{ return false; }
		static constexpr value_type epsilon() 		{ return false; }
		static constexpr value_type round_error() 	{ return false; }
		static constexpr value_type denorm_min() 	{ return false; }
		static constexpr value_type infinity() 		{ return false; }
		static constexpr value_type quiet_NaN() 	{ return false; }
		static constexpr value_type signaling_NaN() { return false; }

		static constexpr 	int  digits    			= 1;
		static constexpr 	int  digits10  			= 0;
    	static constexpr 	bool is_signed 			= false;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for char
	 */
	template<>
  	class numeric_limits<char>  {
  		using value_type = char;

		static constexpr value_type min()        	{ return value_type(-128); }
		static constexpr value_type max()        	{ return value_type(127); }

		static constexpr value_type lowest() 		{ return value_type(-128); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 7;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for uchar
	 */
	template<>
  	class numeric_limits<unsigned char>  {
  		using value_type = unsigned char;

		static constexpr value_type min()        	{ return value_type(0U); }
		static constexpr value_type max()        	{ return value_type(255U); }

		static constexpr value_type lowest() 		{ return value_type(0U); }
		static constexpr value_type epsilon() 		{ return 0U; }
		static constexpr value_type round_error() 	{ return 0U; }
		static constexpr value_type denorm_min() 	{ return 0U; }
		static constexpr value_type infinity() 		{ return 0U; }
		static constexpr value_type quiet_NaN() 	{ return 0U; }
		static constexpr value_type signaling_NaN() { return 0U; }

		static constexpr 	int  digits    			= 8;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= false;
    	static constexpr 	bool is_modulo 			= true;
  	};

  	/**
	 * @brief Spezial version for uchar
	 */
	template<>
  	class numeric_limits<signed char>  {
  		using value_type = signed char;

		static constexpr value_type min()        	{ return value_type(-128); }
		static constexpr value_type max()        	{ return value_type(127); }

		static constexpr value_type lowest() 		{ return value_type(-128); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 7;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for short
	 */
	template<>
  	class numeric_limits<short>  {
  		using value_type = short;

		static constexpr value_type min()        	{ return value_type(-32768); }
		static constexpr value_type max()        	{ return value_type(32767); }

		static constexpr value_type lowest() 		{ return value_type(-32768); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 15;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for unsigned short
	 */
	template<>
  	class numeric_limits<unsigned short>  {
  		using value_type = unsigned short;

		static constexpr value_type min()        	{ return value_type(0U); }
		static constexpr value_type max()        	{ return value_type(65535U); }

		static constexpr value_type lowest() 		{ return value_type(0U); }
		static constexpr value_type epsilon() 		{ return 0U; }
		static constexpr value_type round_error() 	{ return 0U; }
		static constexpr value_type denorm_min() 	{ return 0U; }
		static constexpr value_type infinity() 		{ return 0U; }
		static constexpr value_type quiet_NaN() 	{ return 0U; }
		static constexpr value_type signaling_NaN() { return 0U; }

		static constexpr 	int  digits    			= 16;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= false;
    	static constexpr 	bool is_modulo 			= true;
  	};

  	/**
	 * @brief Spezial version for int
	 */
	template<>
  	class numeric_limits<int>  {
  		using value_type = int;

		static constexpr value_type min()        	{ return value_type(-2147483648); }
		static constexpr value_type max()        	{ return value_type(2147483647); }

		static constexpr value_type lowest() 		{ return value_type(-2147483648); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 31;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for unsigned int
	 */
	template<>
  	class numeric_limits<unsigned int>  {
  		using value_type = unsigned int;

		static constexpr value_type min()        	{ return value_type(0U); }
		static constexpr value_type max()        	{ return value_type(4294967295U); }

		static constexpr value_type lowest() 		{ return value_type(0U); }
		static constexpr value_type epsilon() 		{ return 0U; }
		static constexpr value_type round_error() 	{ return 0U; }
		static constexpr value_type denorm_min() 	{ return 0U; }
		static constexpr value_type infinity() 		{ return 0U; }
		static constexpr value_type quiet_NaN() 	{ return 0U; }
		static constexpr value_type signaling_NaN() { return 0U; }

		static constexpr 	int  digits    			= 32;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= false;
    	static constexpr 	bool is_modulo 			= true;
  	};

  	/**
	 * @brief Spezial version for long
	 */
	template<>
  	class numeric_limits<long>  {
  		using value_type = long;

		static constexpr value_type min()        	{ return value_type(-9223372036854775807 - 1L); }
		static constexpr value_type max()        	{ return value_type(9223372036854775807); }

		static constexpr value_type lowest() 		{ return value_type(-9223372036854775807 - 1L); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 63;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for unsigned long
	 */
	template<>
  	class numeric_limits<unsigned long>  {
  		using value_type = unsigned long;

		static constexpr value_type min()        	{ return value_type(0U); }
		static constexpr value_type max()        	{ return value_type(18446744073709551615U); }

		static constexpr value_type lowest() 		{ return value_type(0U); }
		static constexpr value_type epsilon() 		{ return 0U; }
		static constexpr value_type round_error() 	{ return 0U; }
		static constexpr value_type denorm_min() 	{ return 0U; }
		static constexpr value_type infinity() 		{ return 0U; }
		static constexpr value_type quiet_NaN() 	{ return 0U; }
		static constexpr value_type signaling_NaN() { return 0U; }

		static constexpr 	int  digits    			= 64;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= false;
    	static constexpr 	bool is_modulo 			= true;
  	};

  	/**
	 * @brief Spezial version for long long
	 */
	template<>
  	class numeric_limits<long long>  {
  		using value_type = long long;

		static constexpr value_type min()        	{ return value_type(-9223372036854775807LL - 1LL); }
		static constexpr value_type max()        	{ return value_type(9223372036854775807LL); }

		static constexpr value_type lowest() 		{ return value_type(-9223372036854775807LL - 1LL); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 63;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};

  	/**
	 * @brief Spezial version for unsigned long long
	 */
	template<>
  	class numeric_limits<unsigned long long>  {
  		using value_type = unsigned long long;

		static constexpr value_type min()        	{ return value_type(0U); }
		static constexpr value_type max()        	{ return value_type(18446744073709551615ULL); }

		static constexpr value_type lowest() 		{ return value_type(0U); }
		static constexpr value_type epsilon() 		{ return 0; }
		static constexpr value_type round_error() 	{ return 0; }
		static constexpr value_type denorm_min() 	{ return 0; }
		static constexpr value_type infinity() 		{ return 0; }
		static constexpr value_type quiet_NaN() 	{ return 0; }
		static constexpr value_type signaling_NaN() { return 0; }

		static constexpr 	int  digits    			= 64;
		static constexpr 	int  digits10  			= ((digits * 301) / 1000);
    	static constexpr 	bool is_signed 			= true;
    	static constexpr 	bool is_modulo 			= false;
  	};


  	/**
	 * @brief Spezial version for float
	 */
	template<>
  	class numeric_limits<float> {
  		using value_type = float;

		static constexpr value_type min()        	{ return __FLT_MIN__; }
		static constexpr value_type max()        	{ return __FLT_MAX__; }

		static constexpr value_type lowest() 		{ return -__FLT_MAX__; }
		static constexpr value_type epsilon() 		{ return __FLT_EPSILON__; }
		static constexpr value_type denorm_min() 	{ return __FLT_MIN__; }
		static constexpr value_type infinity() 		{ return __builtin_huge_valf(); }
		static constexpr value_type quiet_NaN() 	{ return __builtin_nanf(""); }
		static constexpr value_type signaling_NaN() { return __builtin_nansf(""); }

		static constexpr 	int  digits    			= __FLT_MANT_DIG__;
		static constexpr 	int  digits10  			= __FLT_DIG__;
		static constexpr 	int  max_digits10  		= ((digits * 301) / 1000) + 2;

    	static constexpr 	int min_exponent   = __FLT_MIN_EXP__;
    	static constexpr 	int min_exponent10 = __FLT_MIN_10_EXP__;
    	static constexpr 	int max_exponent   = __FLT_MAX_EXP__;
    	static constexpr 	int max_exponent10 = __FLT_MAX_10_EXP__;

    	static constexpr 	bool is_specialized    = true;
		static constexpr 	bool is_signed         = true;
		static constexpr 	bool is_integer        = false;
		static constexpr 	bool is_exact          = false;
		static constexpr 	int radix              = 2;
		static constexpr 	bool has_infinity      = true;
		static constexpr 	bool has_quiet_NaN     = true;
		static constexpr 	bool has_signaling_NaN = true;
		static constexpr 	bool has_denorm_loss   = false;
		static constexpr 	bool is_iec559         = false;
		static constexpr 	bool is_bounded        = true;
		static constexpr 	bool is_modulo         = false;
		static constexpr 	bool traps             = false;
		static constexpr 	bool tinyness_before   = false;
		static constexpr 	float_denorm_style has_denorm = float_denorm_style::indeterminate;
		static constexpr 	float_round_style round_style = float_round_style::indeterminate;

		static 				value_type round_error() 	{ return value_type(0.5f); }
  	};


  	/**
	 * @brief Spezial version for double
	 */
	template<>
  	class numeric_limits<double> {
  		using value_type = double;

		static constexpr value_type min()        	{ return __DBL_MIN__; }
		static constexpr value_type max()        	{ return __DBL_MAX__; }

		static constexpr value_type lowest() 		{ return -__DBL_MAX__; }
		static constexpr value_type epsilon() 		{ return __DBL_EPSILON__; }
		static constexpr value_type denorm_min() 	{ return __DBL_MIN__; }
		static constexpr value_type infinity() 		{ return __builtin_huge_val(); }
		static constexpr value_type quiet_NaN() 	{ return __builtin_nan(""); }
		static constexpr value_type signaling_NaN() { return __builtin_nans("");; }

		static constexpr 	int  digits    			= __DBL_MANT_DIG__;
		static constexpr 	int  digits10  			= __DBL_DIG__;
		static constexpr 	int  max_digits10  		= ((digits * 301) / 1000) + 2;

    	static constexpr 	int min_exponent   = __DBL_MIN_EXP__;
    	static constexpr 	int min_exponent10 = __DBL_MIN_10_EXP__;
    	static constexpr 	int max_exponent   = __DBL_MAX_EXP__;
    	static constexpr 	int max_exponent10 = __DBL_MAX_10_EXP__;

    	static constexpr 	bool is_specialized    = true;
		static constexpr 	bool is_signed         = true;
		static constexpr 	bool is_integer        = false;
		static constexpr 	bool is_exact          = false;
		static constexpr 	int radix              = 2;
		static constexpr 	bool has_infinity      = true;
		static constexpr 	bool has_quiet_NaN     = true;
		static constexpr 	bool has_signaling_NaN = true;
		static constexpr 	bool has_denorm_loss   = false;
		static constexpr 	bool is_iec559         = false;
		static constexpr 	bool is_bounded        = true;
		static constexpr 	bool is_modulo         = false;
		static constexpr 	bool traps             = false;
		static constexpr 	bool tinyness_before   = false;
		static constexpr 	float_denorm_style has_denorm = float_denorm_style::indeterminate;
		static constexpr 	float_round_style round_style = float_round_style::indeterminate;

		static 				value_type round_error() 	{ return value_type(0.5); }
  	};

	/**
	 * @brief Spezial version for long double
	 */
	template<>
  	class numeric_limits<long double> {
  		using value_type = long double;

		static constexpr value_type min()        	{ return __LDBL_MIN__; }
		static constexpr value_type max()        	{ return __LDBL_MAX__; }

		static constexpr value_type lowest() 		{ return -__LDBL_MAX__; }
		static constexpr value_type epsilon() 		{ return __LDBL_EPSILON__; }
		static constexpr value_type denorm_min() 	{ return __LDBL_MIN__; }
		static constexpr value_type infinity() 		{ return __builtin_huge_val(); }
		static constexpr value_type quiet_NaN() 	{ return __builtin_nanl(""); }
		static constexpr value_type signaling_NaN() { return __builtin_nansl("");; }

		static constexpr 	int  digits    			= __LDBL_MANT_DIG__;
		static constexpr 	int  digits10  			= __LDBL_DIG__;
		static constexpr 	int  max_digits10  		= ((__LDBL_MANT_DIG__ * 301) / 1000) + 2;

    	static constexpr 	int min_exponent   = __LDBL_MIN_EXP__;
    	static constexpr 	int min_exponent10 = __LDBL_MIN_10_EXP__;
    	static constexpr 	int max_exponent   = __LDBL_MAX_EXP__;
    	static constexpr 	int max_exponent10 = __LDBL_MAX_10_EXP__;

    	static constexpr 	bool is_specialized    = true;
		static constexpr 	bool is_signed         = true;
		static constexpr 	bool is_integer        = false;
		static constexpr 	bool is_exact          = false;
		static constexpr 	int radix              = 2;
		static constexpr 	bool has_infinity      = true;
		static constexpr 	bool has_quiet_NaN     = true;
		static constexpr 	bool has_signaling_NaN = true;
		static constexpr 	bool has_denorm_loss   = false;
		static constexpr 	bool is_iec559         = false;
		static constexpr 	bool is_bounded        = true;
		static constexpr 	bool is_modulo         = false;
		static constexpr 	bool traps             = false;
		static constexpr 	bool tinyness_before   = false;
		static constexpr 	float_denorm_style has_denorm = float_denorm_style::indeterminate;
		static constexpr 	float_round_style round_style = float_round_style::indeterminate;

		static 				value_type round_error() 	{ return value_type(0.5); }
  	};

}

#endif // __MINILIB_NUMERIC_LIMITS_H__
