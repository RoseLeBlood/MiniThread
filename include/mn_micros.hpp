/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2018 Amber-Sophia Schroeck
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _MNTHREAD_MICROS_H_
#define _MNTHREAD_MICROS_H_

/**
 * Get the current micros 
 * @return Current micros
 */
unsigned long micros();
/**
 * Get the current millis or from ISR context - automatic switch
 * @return Current milliseconds
 */
unsigned long millis();

/**
 *  Get the current tick count or from ISR context - automatic switch
 *  @return Current tick count.
 */
unsigned int get_ticks();

/**
 *  Convert from ticks to ms.
 *
 *  @param ticks ticks to convert.
 *  @return milliseconds.
 */
unsigned int ticks_to_ms(unsigned int ticks);

/**
 *  Convert from ms to ticks.
 *
 *  @param milliseconds milliseconds to convert.
 *  @return ticks
 */
unsigned int ms_to_ticks(unsigned int ms);

/**
 *  Convert from seconds to ticks.
 *
 *  @param seconds seconds to convert.
 *  @return ticks
 */
unsigned int seconds_to_ticks(unsigned int ms);

#endif
