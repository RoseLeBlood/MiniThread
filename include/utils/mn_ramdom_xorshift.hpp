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

#ifndef __MINILIB_BASIC_RAMDOM_XORSHIFT_H__
#define __MINILIB_BASIC_RAMDOM_XORSHIFT_H__

#include "../mn_config.hpp"

#include "mn_random.hpp"

namespace mn {
	/**
     * @brief Pseudro-Random Number Generator based on xor_shift128++ generator
     *
     * @note Based on https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/
     *
     * @author Amber-Sophia Schröck
     */
	class basic_ramdom_xorshift : public IPseudoRandomUtil<unsigned int>  {
		using base_type = IPseudoRandomUtil<unsigned int> ;
	public:
		basic_ramdom_xorshift(unsigned int startSeed)
            : base_type(startSeed) { init(startSeed); }

		 /**
         * @brief Get a random unsigned char number
         *
         * @return A random unsigned char number
         */
        virtual unsigned char rand8() override;
        /**
         * @brief Get a random unsigned short number
         *
         * @return A random unsigned short number
         */
        virtual unsigned short rand16() override;
        /**
         * @brief Get a random unsigned int number
         *
         * @return A random unsigned int number
         */
        virtual unsigned int rand32() override;

        virtual seed_t get_seed() override { return m_xorandArray[0]; }
        virtual void set_seed(seed_t seed) override { init(seed); }
	private:
		void init(unsigned int seed);
	private:
		unsigned long m_xorandArray[4];
	};
}
#endif // __MINILIB_BASIC_RAMDOM_XORSHIFT_H__
