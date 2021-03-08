/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018 Amber-Sophia Schroeck
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
#ifndef _MNTHREAD_MICROS_H_
#define _MNTHREAD_MICROS_H_

#include <sys/time.h>
#include <time.h>

namespace mn {
    /**
     * @ingroup base
     */

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

    /**
     * Convert timeval to milliseconds
     * 
     * @param time The timeval to convert
     * @return the timeval in milliseconds
     */ 
    unsigned int time_to_ms(const struct timeval* time);

    /**
     * Convert timeval to ticks
     * 
     * @param time The timeval to convert
     * @return the timeval in ticks
     */ 
    unsigned int time_to_ticks(const struct timeval* time);

    /**
     * Operator to sub two timevals
     * 
     * @param lhs The left operater timeval
     * @param rhs The right operater timeval
     */ 
    static inline struct timeval operator - (const struct timeval &lhs, const struct timeval &rhs) {
        int s = lhs.tv_sec - rhs.tv_sec;		
        int ns = lhs.tv_usec - rhs.tv_usec;	
        if (ns < 0) {			
            s--;				
            ns += 1000000;			
        }							
            
        return timeval{s, ns};
    }

    /**
     * Operator to add two timevals
     * 
     * @param lhs The left operater timeval
     * @param rhs The right operater timeval
     */ 
    static inline struct timeval operator + (const struct timeval &lhs, const struct timeval &rhs) {
        int s = lhs.tv_sec + rhs.tv_sec;		
        int ns = lhs.tv_usec + rhs.tv_usec;	
        if (ns >= 1000000) {			
            s++;				
            ns -= 1000000;			
        }							
            
        return timeval{s, ns};
    }
}
#endif
