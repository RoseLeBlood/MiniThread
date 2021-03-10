/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
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

#ifndef __LIBMIN_73d262d0_2534_414e_9be2_0c0c4555de06_H_
#define __LIBMIN_73d262d0_2534_414e_9be2_0c0c4555de06_H_

#include "../mn_string.hpp"
#include "../mn_algorithm.hpp"

namespace mn {
    namespace math {

        /**
         * @brief A class for  RGBA Color handling
         * @tparam T The value of the color value
         */
        template <typename T> 
        class basic_color {
            static constexpr double colorcon = 0.003921568627450980392156862745098;
        public:
            using value_type = T;
            using pointer = T*;
            using self_type = basic_color<T>;

            union {
                struct {
                    /** @brief The red value  */
                    value_type r;
                    /** @brief The green value  */
                    value_type g;
                    /** @brief The blue value  */
                    value_type b;
                    /** @brief The alpha value  */
                    value_type a;
                };
                struct {
                    /** @brief The red value  */
                    value_type red;
                    /** @brief The green value  */
                    value_type green;
                    /** @brief The blue value  */
                    value_type blue;
                    /** @brief The alpha value  */
                    value_type alpha;
                };
                value_type c[4];
            };
            /**
             * @brief Construct a new basic color object
             */
            basic_color(void) 
                : r(0), g(0), b(0), a(1.0f) {}
            /**
             * @brief Construct a new basic color object
             * 
             * @param _r The red vaule
             * @param _g The green vaule
             * @param _b The blue vaule
             * @param _a The alpha vaule
             */
            basic_color(value_type _r, value_type _g, value_type _b, value_type _a)	
                : r(_r), g(_g), b(_b), a(_a) {}
            /**
             * @brief Construct a new basic color object
             * 
             * @param _r The red vaule
             * @param _g The green vaule
             * @param _b The blue vaule
             */
            basic_color(value_type _r, value_type _g, value_type _b) 
                : r(_r), g(_g), b(_b), a(1.0) {}
            /**
             * @brief Construct a new basic color object
             * 
             * @param com a array of the rgba color
             */
            basic_color(value_type *com) 
                : r(com[0]), g(com[1]), b(com[2]), a(com[3]) {}
            /**
             * @brief Construct a new basic color object
             * 
             * @param _r The red value an int 0-255
             * @param _g The green value an int 0-255
             * @param _b The blue value an int 0-255
             */
            basic_color(const int _r, const int _g, const int _b) 
                : r((T)(_r) * colorcon), g((T)(_g) * colorcon), b((T)(_b) * colorcon), a(1.0f)	{}
            /**
             * @brief Construct a new basic color object
             * 
             * @param _r The red value an int 0-255
             * @param _g The green value an int 0-255
             * @param _b The blue value an int 0-255
             * @param _a The alpha value an int 0-255
             */
            basic_color(const int _r, const int _g, const int _b, const int _a) 
                : r((T)(_r) * colorcon), g((T)(_g) * colorcon), b((T)(_b) * colorcon), a((T)(_a) * colorcon) {}
            /**
             * @brief Construct a new basic color object
             * 
             * @param pComponent a array of the rgba color as in [RRRGGGBBBAAA]
             */
            basic_color(const int* pComponent) 
                : r((T)(pComponent[0]) * colorcon), g((T)(pComponent[1]) * colorcon), b((T)(pComponent[2]) * colorcon), a((T)(pComponent[3]) * colorcon) {}
            /**
             * @brief Construct a new basic color object
             */
            basic_color(const int c) 
                : r(colorcon * (T)(c >> 16)), g(colorcon * (T)(c >> 8)), b(colorcon * (T)(c)), a(colorcon * (T)(c >> 24)) {}

    
            self_type& operator = (const self_type& c)	{a = c.a; b = c.b; g = c.g; r = c.r; return *this;} 
            self_type& operator += (const self_type& c)	{r += c.r; g += c.g; b += c.b; a += c.a; return *this;}
	        self_type& operator -= (const self_type& c)	{r -= c.r; g -= c.g; b -= c.b; a -= c.a; return *this;}
	        self_type& operator *= (const self_type& c)	{r *= c.r; g *= c.g; b *= c.b; a *= c.a; return *this;}
	        self_type& operator *= (const value_type f)	{r *= f; g *= f; b *= f; a *= f; return *this;}
	        self_type& operator /= (const self_type& c)	{r /= c.r; g /= c.g; b /= c.b; a /= c.a; return *this;}
	        self_type& operator /= (const value_type f)	{r /= f; g /= f; b /= f; a /= f; return *this;}
       
            virtual mn::string to_string() {
                return mn::frmstring("Color: R:%d G:%d B:%d A:%d", red, green, blue, alpha);
            }

            operator unsigned long ()  {
                return ((a >= 1.0f ? 255 : a <= 0.0f ? 0 : (unsigned long)(a * 255.0f)) << 24) |
                        ((r >= 1.0f ? 255 : r <= 0.0f ? 0 : (unsigned long)(r * 255.0f)) << 16) |
                        ((g >= 1.0f ? 255 : g <= 0.0f ? 0 : (unsigned long)(g * 255.0f)) << 8)  |
                        (b >= 1.0f ? 255 : b <= 0.0f ? 0 : (unsigned long)(b * 255.0f));
            }
            operator T* ()			{return (T*)(c);}
        };
        template <typename T>
        inline basic_color<T> operator + (const basic_color<T>& a, const basic_color<T>& b)	{
            return basic_color<T>(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);}

        template <typename T>
        inline basic_color<T> operator - (const basic_color<T>& a, const basic_color<T>& b)	{
            return basic_color<T>(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);}
        template <typename T>
        inline basic_color<T> operator - (const basic_color<T>& c) {
            return basic_color<T>(-c.r, -c.g, -c.b, c.a);}

        template <typename T>
        inline basic_color<T> operator * (const basic_color<T>& a, const basic_color<T>& b)	{
            return basic_color<T>(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);}

        template <typename T>
        inline basic_color<T> operator * (const basic_color<T>& c, const T f) {
            return basic_color<T>(c.r * f, c.g * f, c.b * f, c.a * f);}

        template <typename T>
        inline basic_color<T> operator * (const T f, const basic_color<T>& c) {
            return basic_color<T>(c.r * f, c.g * f, c.b * f, c.a * f);}

        template <typename T>
        inline basic_color<T> operator / (const basic_color<T>& a, const basic_color<T>& b)	{
            return basic_color<T>(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);}

        template <typename T>
        inline basic_color<T> operator / (const basic_color<T>& c, const T f) {
            return basic_color<T>(c.r / f, c.g / f, c.b / f, c.a / f);}

        // Vergleichsoperatoren
        template <typename T>
        inline bool operator == (const basic_color<T>& a, const basic_color<T>& b) {
            if(a.r != b.r) return false; if(a.g != b.g) return false; if(a.b != b.b) return false; return a.a == b.a;}
        
        template <typename T>
        inline bool operator != (const basic_color<T>& a, const basic_color<T>& b) {
            return !(a == b); }

        // ******************************************************************
        // Funktionen deklarieren

        /**
         * @brief negate
         */
        template <typename T>
        inline basic_color<T>	negate(const basic_color<T>& c) {
            return basic_color<T>(1.0f - c.r, 1.0f - c.g, 1.0f - c.b, 1.0f - c.a);}

        /**
         * @brief brightness
         */
        template <typename T>
        inline T	brightness(const basic_color<T>& c)	{
            return c.r * 0.299f + c.g * 0.587f + c.b * 0.114f;}

        /**
         * @brief interpolate
         * @param c1 The start color
         * @param c2 The end color
         * @param p The step 
         * @return The interpolated color
         */
        template <typename T>
        inline basic_color<T>	interpolate(const basic_color<T>& c1, const basic_color<T>& c2, const T p) {
            return c1 + p * (c2 - c1);}

        /**
         * @brief Get the min color
         * @return The min color
         */
        template <typename T>
        inline basic_color<T> min(const basic_color<T>& c1, const basic_color<T>& c2) {
            return basic_color<T>(mn::min<T>(c1.r, c2.r), mn::min<T>(c1.g, c2.g), mn::min<T>(c1.b, c2.b), mn::min<T>(c1.a, c2.a));}
        
        /**
         * @brief Get the max color
         * @return The max color
         */
        template <typename T>
        inline basic_color<T> max(const basic_color<T>& c1, const basic_color<T>& c2) {
            return basic_color<T>(mn::max<T>(c1.r, c2.r), mn::max<T>(c1.g, c2.g), mn::max<T>(c1.b, c2.b), mn::max<T>(c1.a, c2.a));}
        
        /**
         * @brief Create a RGBA color object from a yuv
         */
        template <typename T>
        basic_color<T> from_yuv(const T y, const T u, const T v) {
            return basic_color<T>(T(1.164 * (y - 16) + 1.596*(v - 128)), 
                                T(1.164 * (y - 16) - 0.813*(v - 128) - 0.391*(u - 128)), 
                                T(1.164 * (y - 16) + 2.018*(u - 128)));
        }
        
        /**
         * @brief Create a RGBA color object from a cmy
         */
        template <typename T>
        basic_color<T> from_cmy(const T c, const T m, const T y) {
            return basic_color<T>( T(1.0 - c), T(1.0 - m), T(1.0 - y));
        }
        
        /**
         * @brief Create a RGBA color object from a hsv
         */
        template <typename T>
        basic_color<T> from_hsv(const T h, const T s, const T v) {
                if( s == 0 ) return basic_color<T>(v,v,v);
                
                T i, f, p, q, t;
                
                h /= 60;			// sector 0 to 5
                i = floor( h );
                f = h - i;			// factorial part of h
                p = v * ( 1 - s );
                q = v * ( 1 - s * f );
                t = v * ( 1 - s * ( 1 - f ) );

                if(i == 0) return basic_color<T>(v, t, p);
                if(i == 1) return basic_color<T>(q, v, p);
                if(i == 2) return basic_color<T>(p, v, t);
                if(i == 3) return basic_color<T>(p, q, v);
                if(i == 4) return basic_color<T>(t, p, v);

                return raColor(v, p, q);
        }

        using color = basic_color<float>;
    }
} // namespace mn


#endif