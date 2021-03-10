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
#ifndef MINLIB_65db7f3d_c57e_4420_884f_350c78711f41_H_
#define MINLIB_65db7f3d_c57e_4420_884f_350c78711f41_H_

#include "mn_config.hpp"

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

namespace mn {
    enum class pin_direction {
        Disable = GPIO_MODE_DISABLE,
        Input = GPIO_MODE_INPUT,
        Output = GPIO_MODE_OUTPUT,
        InOut = GPIO_MODE_INPUT_OUTPUT;
    };

    template <int TPin>
    class basic_hardware_pin { };


    
    template <int TPin, bool TLevel = false, pin_direction TDirection = pin_direction::Output>
    class basic_esp32_hardware_digital_pin : public basic_hardware_pin<TPin> { 
    public:
        basic_esp32_hardware_pin() {
            gpio_pad_select_gpio(TPin); 
            gpio_set_level(TPin, TLevel ? 1 : 0);
            gpio_set_direction(TPin, TDirection); 
        }
        void set(bool on) { 
            gpio_set_level(TPin, on ? 1 : 0); 
        }
        void reset() { 
            gpio_reset_pin(TPin); 
        }
        bool get() { 
            return gpio_get_level(TPin) == 1; 
        }
        void set_direction(pin_direction& direction) {
            gpio_set_direction(TPin, direction);
        }


    };

    template <int TPin, bool TLevel = false, pin_direction TDirection = pin_direction::Output>
    using esp32_digital_pin = basic_esp32_hardware_pin<TPin>;




}


#endif