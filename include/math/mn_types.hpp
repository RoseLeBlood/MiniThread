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

#ifndef __MINILIB_MATH_VECXN_HPP__
#define __MINILIB_MATH_VECXN_HPP__

#include "../mn_config.hpp"
#include "../mn_functional.hpp"

#include <mn_algorithm.hpp>


namespace mn {
	namespace math {
		/**
		 * @brief Basic template for all mn::math using vector types.
		 * @tparam TTYPE type for the vector
		 * @tparam TSIZE The dimension of the vector type.
		 */
		template <typename TTYPE, size_t TSIZE >
		struct vecnx {
			using value_type = TTYPE;
			using size_type = size_t;
			using reference = TTYPE&;
			using const_reference = const TTYPE&;

			static size_type n_elemenst() { return TSIZE; }
		};

		/**
		 * @brief Basic template for 1-Dimonsen mn::math using vector types.
		 * @tparam TTYPE type for the vector.
		 */
		template <typename TTYPE>
		struct vec1x : public vecnx<TTYPE, 1> {
			using base_type = vecnx<TTYPE, 1>;
			using self_type = vec1x<TTYPE>;

			using value_type = typename base_type::value_type;
			using size_type = typename base_type::size_type;
			using reference = typename base_type::reference;
			using const_reference = typename base_type::const_reference;

			using base_type::n_elemenst;

			union {
				struct {
					value_type x;
				};
				struct {
					value_type X;
				};
				value_type narray[1];
			};

			vec1x() : x(TTYPE(0)) { }
			vec1x(value_type _x) : x(_x) { }
			vec1x(value_type* comp) : x(comp[0]) { }

			vec1x(const self_type& other) : x(other.x) { }
			vec1x(const self_type&& other) : x(mn::move(other.x)) { }

			/**
			 * @brief Swaps the content of the two Anys.
			 */
			self_type& swap( self_type& other ) noexcept {
				if (this == &other) return *this;

        		mn::swap( x, other.x );
        		return *this;
    		}

			reference operator[](size_type pos) noexcept {
				assert(pos < 1);
				return narray[pos];
			}

      		constexpr const_reference operator[](size_type pos) const noexcept {
      			assert(pos < 1);
      			return narray[pos];
			}

            self_type& operator += (const self_type& c)	{x += c.x; return *this;}
	        self_type& operator -= (const self_type& c)	{x -= c.x; return *this;}
	        self_type& operator *= (const self_type& c)	{x *= c.x; return *this;}
	        self_type& operator *= (const value_type f)	{x *= x;   return *this;}
	        self_type& operator /= (const self_type& c)	{x /= c.x; return *this;}
	        self_type& operator /= (const value_type f)	{x /= x;   return *this;}

	        operator value_type* ()			{return (TTYPE*)(narray);}

	        /**
			 * @brief Assignment operator for vector 1D.
			 */
			self_type& operator = ( const self_type& other ) noexcept {
        		self_type( other ).swap( *this );
        		return *this;
    		}
    		/**
			 * @brief Assignment operator for 1D vector.
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
				x = other;
				return *this;
			}
		};

		template <typename TTYPE>
        inline vec1x<TTYPE> operator + (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a.x + b.x);}

		template <typename TTYPE>
        inline vec1x<TTYPE> operator - (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a.x - b.x);}

		template <typename TTYPE>
        inline vec1x<TTYPE> operator * (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a.x * b.x);}

		template <typename TTYPE>
        inline vec1x<TTYPE> operator / (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a.x / b.x);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator + (const vec1x<TTYPE>& a, const T b)	{
			return vec1x<TTYPE>(a.x + b);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator - (const vec1x<TTYPE>& a, const T b)	{
			return vec1x<TTYPE>(a.x - b);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator - (const T a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a - b.x);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator * (const vec1x<TTYPE>& a, const T b)	{
			return vec1x<TTYPE>(a.x * b);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator / (const vec1x<TTYPE>& a, const T b)	{
			return vec1x<TTYPE>(a.x / b);}

		template <typename TTYPE, typename T>
        inline vec1x<TTYPE> operator / (const T a, const vec1x<TTYPE>& b)	{
			return vec1x<TTYPE>(a / b.x);}

		template <typename TTYPE>
        inline bool operator == (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x == b.x; }

		template <typename TTYPE>
        inline bool operator != (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x != b.x; }

		template <typename TTYPE>
        inline bool operator <= (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x <= b.x; }

		template <typename TTYPE>
        inline bool operator >= (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x >= b.x; }

		template <typename TTYPE>
        inline bool operator < (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x < b.x; }

		template <typename TTYPE>
        inline bool operator > (const vec1x<TTYPE>& a, const vec1x<TTYPE>& b)	{
			return a.x > b.x; }

		using vec1b = vec1x<int8_t>;
		using vec1s = vec1x<int16_t>;
		using vec1i = vec1x<int32_t>;
		using vec1f = vec1x<float>;
		using vec1d = vec1x<double>;

		/**
		 * @brief Basic template for 2D mn::math using vector types.
		 * @tparam TTYPE type for the vector.
		 */
		template <typename TTYPE>
		struct vec2x : public vecnx<TTYPE, 2> {
			using base_type = vecnx<TTYPE, 2>;
			using self_type = vec2x<TTYPE>;

			using value_type = typename base_type::value_type;
			using size_type = typename base_type::size_type;
			using reference = typename base_type::reference;
			using const_reference = typename base_type::const_reference;

			using base_type::n_elemenst;

			union {
				struct {
					value_type x;
					value_type y;
				};
				struct {
					value_type X;
					value_type Y;
				};
				value_type narray[2];
			};

			vec2x() : x(TTYPE(0)), y(TTYPE(0)) { }
			vec2x(value_type _x, value_type _y) : x(_x), y(_y) { }
			vec2x(value_type* comp) : x(comp[0]), y(comp[1]) { }

			vec2x(const self_type& other) : x(other.x), y(other.y) { }
			vec2x(const self_type&& other) : x(mn::move(other.x)), y(mn::move(other.y)) { }

			/**
			 * @brief Swaps the content of the two Anys.
			 */
			self_type& swap( self_type& other ) noexcept {
				if (this == &other) return *this;

        		mn::swap( x, other.x );
        		mn::swap( y, other.y );
        		return *this;
    		}

			reference operator[](size_type pos) noexcept {
				assert(pos < 2);
				return narray[pos];
			}

      		constexpr const_reference operator[](size_type pos) const noexcept {
      			assert(pos < 2);
      			return narray[pos];
			}

            self_type& operator += (const self_type& c)	{x += c.x; y += c.y; return *this;}
	        self_type& operator -= (const self_type& c)	{x -= c.x; y -= c.y; return *this;}
	        self_type& operator *= (const self_type& c)	{x *= c.x; y *= c.y; return *this;}
	        self_type& operator *= (const value_type f)	{x *= f;   y *= f;   return *this;}
	        self_type& operator /= (const self_type& c)	{x /= c.x; y /= c.y; return *this;}
	        self_type& operator /= (const value_type f)	{x /= x;   y /= f;   return *this;}

	        operator value_type* ()			{return (TTYPE*)(narray);}

	        /**
			 * @brief Assignment operator for vector 1D.
			 */
			self_type& operator = ( const self_type& other ) noexcept {
        		self_type( other ).swap( *this );
        		return *this;
    		}
    		/**
			 * @brief Assignment operator for 1D vector.
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
				x = other; y = other;
				return *this;
			}
		};

		template <typename TTYPE>
        inline vec2x<TTYPE> operator + (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			return vec2x<TTYPE>(a.x + b.x, a.y + b.y);}

		template <typename TTYPE>
        inline vec2x<TTYPE> operator - (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			return vec2x<TTYPE>(a.x - b.x, a.y - b.y);}

		template <typename TTYPE>
        inline vec2x<TTYPE> operator * (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			return vec2x<TTYPE>(a.x * b.x, a.y * b.y);}

		template <typename TTYPE>
        inline vec2x<TTYPE> operator / (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			return vec2x<TTYPE>(a.x / b.x, a.y / b.y);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator + (const vec2x<TTYPE>& a, const T b)	{
			return vec2x<TTYPE>(a.x + b, a.y + b);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator - (const vec2x<TTYPE>& a, const T b)	{
			return vec2x<TTYPE>(a.x - b, a.y - b);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator - (const T a, const vec2x<TTYPE>& b)	{
			return vec2x<TTYPE>(a - b.x, a - b.y);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator * (const vec2x<TTYPE>& a, const T b)	{
			return vec2x<TTYPE>(a.x * b, a.y * b);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator / (const vec2x<TTYPE>& a, const T b)	{
			return vec1x<TTYPE>(a.x / b, a.y / b);}

		template <typename TTYPE, typename T>
        inline vec2x<TTYPE> operator / (const T a, const vec2x<TTYPE>& b)	{
			return vec1x<TTYPE>(a / b.x, a / b.y);}

		template <typename TTYPE>
        inline bool operator == (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x != b.x) return false; return a.y == b.y; }

		template <typename TTYPE>
        inline bool operator != (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x == b.x) return false; return a.y != b.y; }

		template <typename TTYPE>
        inline bool operator <= (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x > b.x) return false; return a.y <= b.y; }

		template <typename TTYPE>
        inline bool operator >= (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x < b.x) return false; return a.y >= b.y; }

		template <typename TTYPE>
        inline bool operator < (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x >= b.x) return false; return a.y < b.y; }

		template <typename TTYPE>
        inline bool operator > (const vec2x<TTYPE>& a, const vec2x<TTYPE>& b)	{
			if(a.x <= b.x) return false; return a.y > b.y; }

		using vec2b = vec2x<int8_t>;
		using vec2s = vec2x<int16_t>;
		using vec2i = vec2x<int32_t>;
		using vec2f = vec2x<float>;
		using vec2d = vec2x<double>;

		/**
		 * @brief Basic template for 3D mn::math using vector types.
		 * @tparam TTYPE type for the vector.
		 */
		template <typename TTYPE>
		struct vec3x : public vecnx<TTYPE, 3> {
			using base_type = vecnx<TTYPE, 3>;
			using self_type = vec3x<TTYPE>;

			using value_type = typename base_type::value_type;
			using size_type = typename base_type::size_type;
			using reference = typename base_type::reference;
			using const_reference = typename base_type::const_reference;

			using base_type::n_elemenst;

			union {
				struct {
					value_type x;
					value_type y;
					value_type z;
				};
				struct {
					value_type X;
					value_type Y;
					value_type Z;
				};
				value_type narray[3];
			};

			vec3x() : x(TTYPE(0)), y(TTYPE(0)), z(TTYPE(0)) { }
			vec3x(value_type _x, value_type _y,  value_type _z) : x(_x), y(_y), z(_z) { }
			vec3x(value_type* comp) : x(comp[0]), y(comp[1]), z(comp[2]) { }

			vec3x(const self_type& other) : x(other.x), y(other.y), z(other.z) { }
			vec3x(const self_type&& other) : x(mn::move(other.x)), y(mn::move(other.y)), z(mn::move(other.z)) { }

			/**
			 * @brief Swaps the content of the two Anys.
			 */
			self_type& swap( self_type& other ) noexcept {
				if (this == &other) return *this;

        		mn::swap( x, other.x );
        		mn::swap( y, other.y );
        		mn::swap( z, other.z );
        		return *this;
    		}

			reference operator[](size_type pos) noexcept {
				assert(pos < 3);
				return narray[pos];
			}

      		constexpr const_reference operator[](size_type pos) const noexcept {
      			assert(pos < 3);
      			return narray[pos];
			}

            self_type& operator += (const self_type& c)	{x += c.x; y += c.y; z += c.z; return *this;}
	        self_type& operator -= (const self_type& c)	{x -= c.x; y -= c.y; z -= c.z; return *this;}
	        self_type& operator *= (const self_type& c)	{x *= c.x; y *= c.y; z *= c.z; return *this;}
	        self_type& operator *= (const value_type f)	{x *= f;   y *= f;   z *= f;   return *this;}
	        self_type& operator /= (const self_type& c)	{x /= c.x; y /= c.y; z /= c.z; return *this;}
	        self_type& operator /= (const value_type f)	{x /= x;   y /= f;	 z /= f;   return *this;}

	        operator value_type* ()			{return (TTYPE*)(narray);}

	        /**
			 * @brief Assignment operator for vector 1D.
			 */
			self_type& operator = ( const self_type& other ) noexcept {
        		self_type( other ).swap( *this );
        		return *this;
    		}
    		/**
			 * @brief Assignment operator for 1D vector.
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
				x = other; y = other; z = other;
				return *this;
			}
		};

		template <typename TTYPE>
        inline vec3x<TTYPE> operator + (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a.x + b.x, a.y + b.y, a.z + b.z);}

		template <typename TTYPE>
        inline vec3x<TTYPE> operator - (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a.x - b.x, a.y - b.y, a.z - b.z);}

		template <typename TTYPE>
        inline vec3x<TTYPE> operator * (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a.x * b.x, a.y * b.y, a.z * b.z);}

		template <typename TTYPE>
        inline vec3x<TTYPE> operator / (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a.x / b.x, a.y / b.y, a.z / b.z);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator + (const vec3x<TTYPE>& a, const T b)	{
			return vec3x<TTYPE>(a.x + b, a.y + b, a.z + b);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator - (const vec3x<TTYPE>& a, const T b)	{
			return vec3x<TTYPE>(a.x - b, a.y - b, a.z - b);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator - (const T a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a - b.x, a - b.y, a - b.z);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator * (const vec3x<TTYPE>& a, const T b)	{
			return vec3x<TTYPE>(a.x * b, a.y * b, a.z * b);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator / (const vec3x<TTYPE>& a, const T b)	{
			return vec3x<TTYPE>(a.x / b, a.y / b, a.z / b);}

		template <typename TTYPE, typename T>
        inline vec3x<TTYPE> operator / (const T a, const vec3x<TTYPE>& b)	{
			return vec3x<TTYPE>(a / b.x, a / b.y, a / b.z);}

		template <typename TTYPE>
        inline bool operator == (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x != b.x) return false; if(a.y != b.y) return a.z == b.z;  }

		template <typename TTYPE>
        inline bool operator != (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x == b.x) return false; if(a.y == b.y) return a.z != b.z; }

		template <typename TTYPE>
        inline bool operator <= (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x > b.x) return false; if(a.y > b.y) return a.z <= b.z; }

		template <typename TTYPE>
        inline bool operator >= (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x < b.x) return false; if(a.y < b.y) return a.z >= b.z; }

		template <typename TTYPE>
        inline bool operator < (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x >= b.x) return false; if(a.y >= b.y) return a.z < b.z; }

		template <typename TTYPE>
        inline bool operator > (const vec3x<TTYPE>& a, const vec3x<TTYPE>& b)	{
			if(a.x <= b.x) return false; if(a.y <= b.y) return a.z > b.z; }

		using vec3b = vec3x<int8_t>;
		using vec3s = vec3x<int16_t>;
		using vec3i = vec3x<int32_t>;
		using vec3f = vec3x<float>;
		using vec3d = vec3x<double>;

		/**
		 * @brief Basic template for 4D mn::math using vector types.
		 * @tparam TTYPE type for the vector.
		 */
		template <typename TTYPE>
		struct vec4x : public vecnx<TTYPE, 4> {
			using base_type = vecnx<TTYPE, 4>;
			using self_type = vec4x<TTYPE>;

			using value_type = typename base_type::value_type;
			using size_type = typename base_type::size_type;
			using reference = typename base_type::reference;
			using const_reference = typename base_type::const_reference;

			using base_type::n_elemenst;

			union {
				struct {
					value_type x;
					value_type y;
					value_type z;
					value_type w;
				};
				struct {
					value_type X;
					value_type Y;
					value_type Z;
					value_type W;
				};
				value_type narray[4];
			};

			vec4x() : x(TTYPE(0)), y(TTYPE(0)), z(TTYPE(0)), w(TTYPE(0)) { }
			vec4x(value_type _x, value_type _y, value_type _z, value_type _w) : x(_x), y(_y), z(_z), w(_w) { }
			vec4x(value_type* comp) : x(comp[0]), y(comp[1]), z(comp[2]), w(comp[3]) { }

			vec4x(const self_type& other) : x(other.x), y(other.y), z(other.z) { }
			vec4x(const self_type&& other) : x(mn::move(other.x)), y(mn::move(other.y)),
											 z(mn::move(other.z)), w(mn::move(other.w)) { }

			/**
			 * @brief Swaps the content of the two Anys.
			 */
			self_type& swap( self_type& other ) noexcept {
				if (this == &other) return *this;

        		mn::swap( x, other.x );
        		mn::swap( y, other.y );
        		mn::swap( z, other.z );
        		mn::swap( w, other.w );
        		return *this;
    		}

			reference operator[](size_type pos) noexcept {
				assert(pos < 3);
				return narray[pos];
			}

      		constexpr const_reference operator[](size_type pos) const noexcept {
      			assert(pos < 3);
      			return narray[pos];
			}

            self_type& operator += (const self_type& c)	{x += c.x; y += c.y; z += c.z; w += c.w; return *this;}
	        self_type& operator -= (const self_type& c)	{x -= c.x; y -= c.y; z -= c.z; w -= c.w; return *this;}
	        self_type& operator *= (const self_type& c)	{x *= c.x; y *= c.y; z *= c.z; w *= c.w; return *this;}
	        self_type& operator *= (const value_type f)	{x *= f;   y *= f;   z *= f;   w *= f;   return *this;}
	        self_type& operator /= (const self_type& c)	{x /= c.x; y /= c.y; z /= c.z; w /= c.w; return *this;}
	        self_type& operator /= (const value_type f)	{x /= x;   y /= f;	 z /= f;   w /= f;   return *this;}

	        operator value_type* ()			{return (TTYPE*)(narray);}

	        /**
			 * @brief Assignment operator for vector 1D.
			 */
			self_type& operator = ( const self_type& other ) noexcept {
        		self_type( other ).swap( *this );
        		return *this;
    		}
    		/**
			 * @brief Assignment operator for 1D vector.
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
				x = other; y = other; z = other; w = other;
				return *this;
			}
		};

		template <typename TTYPE>
        inline vec4x<TTYPE> operator + (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w );}

		template <typename TTYPE>
        inline vec4x<TTYPE> operator - (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);}

		template <typename TTYPE>
        inline vec4x<TTYPE> operator * (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);}

		template <typename TTYPE>
        inline vec4x<TTYPE> operator / (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator + (const vec4x<TTYPE>& a, const T b)	{
			return vec4x<TTYPE>(a.x + b, a.y + b, a.z + b, a.w + b);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator - (const vec4x<TTYPE>& a, const T b)	{
			return vec4x<TTYPE>(a.x - b, a.y - b, a.z - b, a.w - b);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator - (const T a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a - b.x, a - b.y, a - b.z, a - b.w);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator * (const vec4x<TTYPE>& a, const T b)	{
			return vec4x<TTYPE>(a.x * b, a.y * b, a.z * b, a.w * b);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator / (const vec4x<TTYPE>& a, const T b)	{
			return vec4x<TTYPE>(a.x / b, a.y / b, a.z / b, a.w / b);}

		template <typename TTYPE, typename T>
        inline vec4x<TTYPE> operator / (const T a, const vec4x<TTYPE>& b)	{
			return vec4x<TTYPE>(a / b.x, a / b.y, a / b.z, a / b.w);}

		template <typename TTYPE>
        inline bool operator == (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x != b.x) return false; if(a.y != b.y) return false; if(a.z != b.z) return false; return a.w == b.w;  }

		template <typename TTYPE>
        inline bool operator != (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x == b.x) return false; if(a.y == b.y) return false; if(a.z == b.z) return false; return a.w != b.w;  }

		template <typename TTYPE>
        inline bool operator <= (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x > b.x) return false; if(a.y > b.y) return false; if(a.z > b.z) return false; return a.w <= b.w;  }

		template <typename TTYPE>
        inline bool operator >= (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x < b.x) return false; if(a.y < b.y) return false; if(a.z < b.z) return false; return a.w >= b.w;  }

		template <typename TTYPE>
        inline bool operator < (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x >= b.x) return false; if(a.y >= b.y) return false; if(a.z >= b.z) return false; return a.w < b.w;  }

		template <typename TTYPE>
        inline bool operator > (const vec4x<TTYPE>& a, const vec4x<TTYPE>& b)	{
			if(a.x <= b.x) return false; if(a.y <= b.y) return false; if(a.z <= b.z) return false; return a.w > b.w;  }

		using vec4b = vec4x<int8_t>;
		using vec4s = vec4x<int16_t>;
		using vec4i = vec4x<int32_t>;
		using vec4f = vec4x<float>;
		using vec4d = vec4x<double>;

	}
}

#endif // __MN_MATH_VECXN_HPP__
