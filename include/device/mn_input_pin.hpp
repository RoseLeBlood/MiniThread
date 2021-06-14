/**
 * @file
 * @brief The basic input device interafaces
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
#ifndef __MINLIB_DEVICE_BASIC_INPUT_PIN_H__
#define __MINLIB_DEVICE_BASIC_INPUT_PIN_H__

#include "../mn_config.hpp"


namespace mn {
	namespace device {
		/**
		 * @brief basic_input_pin class is an interface for single input pin.
		 * @ingroup device
		 */
		class basic_input_pin {
		public:
			virtual ~basic_input_pin() = default;

			/**
			 * @brief Get current value of the pin.
			 * @return Get the value of the pin.
			 */
			virtual int read() noexcept = 0;
			/**
			 * @brief Get current value of the pin.
			 * @return Get the value of the pin.
			 */
			virtual const int read() const noexcept = 0;
			/**
			 * @brief Get current state of the pin.
			 * @return Get the state of the pin.
			 */
			virtual bool get() noexcept  = 0;
			/**
			 * @brief Get current state of the pin.
			 * @return Get the state of the pin.
			 */
			virtual const bool get() const noexcept  = 0;
		};
	}
}




#endif // __MINLIB_DEVICE_BASIC_INPUT_PIN_H__
