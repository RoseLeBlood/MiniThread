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
