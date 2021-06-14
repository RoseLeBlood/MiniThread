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

#include "mn_config.hpp"
#include "utils/mn_ramdom_xorshift.hpp"

namespace mn {

	void basic_ramdom_xorshift::init(unsigned int startSeed) {
		m_xorandArray[0] = 876543210 + startSeed;
		m_xorandArray[1] = 123456789 + startSeed;
		m_xorandArray[2] = 565458125 + startSeed;
		m_xorandArray[3] = 6128845 + startSeed;

		for (unsigned char i = 0; i != 4; ++i) {
			if (m_xorandArray[i] == 0) {
				m_xorandArray[i] = 1;
			}
		}
	}

	unsigned char basic_ramdom_xorshift::rand8() {
		IPseudoRandomUtil::convers tp;
		tp.g32 = rand32();

		return tp.g[0] ^ tp.g[3];
	}

	unsigned short basic_ramdom_xorshift::rand16() {
		IPseudoRandomUtil::convers tp;
		tp.g32 = rand32();

		return tp.g16[0] ^ (tp.g16[1] >> 8);
	}

	unsigned int basic_ramdom_xorshift::rand32() {
		const unsigned long _temp = m_xorandArray[0] ^ (m_xorandArray[0] << 11);

		// Rotate the static values ([3] rotation in return statement):
		m_xorandArray[0] = m_xorandArray[1];
		m_xorandArray[1] = m_xorandArray[2];
		m_xorandArray[2] = m_xorandArray[3];

		m_xorandArray[3] = m_xorandArray[3] ^ (m_xorandArray[3] >> 19) ^ (_temp ^ (_temp >> 8));

		return static_cast<unsigned int>(m_xorandArray[3]);
	}

}
