/**
 * @file
 * @brief The basic output device interafaces
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
#ifndef __MINLIB_DEVICE_BASIC_OUTPUT_IN_H__
#define __MINLIB_DEVICE_BASIC_OUTPUT_IN_H__

#include "../mn_config.hpp"
#include "mn_input_pin.hpp"

namespace mn {
	namespace device {
		/**
		 * @brief basic_output_pin class is an interface for single output pin.
		 * @ingroup device
		 */
		class basic_output_pin : public basic_input_pin {
		public:
			/**
			 * @brief Sets value of pin.
			 * @param [in] data is the new state of pin.
			 */
			virtual void write(int data) noexcept = 0;
			/**
			 * @brief Sets value of pin.
			 * @param [in] data is the new state of pin.
			 */
			virtual void write(const int& data) noexcept = 0;
			/**
			 * @brief Sets state of pin.
			 * @param [in] state is the new state of pin.
			 */
			virtual void set(bool state) noexcept = 0;
			/**
			 * @brief Sets state of pin.
			 * @param [in] state is the new state of pin.
			 */
			virtual void set(const bool& state) noexcept = 0;
		};
	}
}



#endif // __MINLIB_DEVICE_BASIC_OUTPUT_IN_H__
