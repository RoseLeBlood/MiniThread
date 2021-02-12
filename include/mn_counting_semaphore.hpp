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
#ifndef MINLIB_ESP32_COUNTING_SEMAPHORE_
#define MINLIB_ESP32_COUNTING_SEMAPHORE_

#include "mn_basic_semaphore.hpp"

namespace mn {
  /**
   * @ingroup semaphore
   * @ingroup lock
   */ 
  class basic_counting_semaphore : public basic_semaphore {
  public:
    /**
     * Create the counting semaphore
     * 
     * @note When enabled the config item MN_THREAD_CONFIG_USE_LOCK_CREATE then throw on error
     * the lockcreate_exception exceptions and the config item MN_THREAD_CONFIG_DEBUG 
     * enabled, then with debug informations.
     * When the config item MN_THREAD_CONFIG_USE_LOCK_CREATE disabled then get the created error code
     * with basic_semaphore::get_error()
     *
     * 
     *  @param count Must not be greater than maxCount.
     *  @param maxcount Must be greater than 0.
     */
    basic_counting_semaphore(int count = MN_THREAD_CONFIG_CSEMAPHORE_MIN_COUNT, 
                            int maxcount = MN_THREAD_CONFIG_CSEMAPHORE_MAX_COUNT);

    virtual ~basic_counting_semaphore();

    /**
     * Set the initial count
     * 
     * @param count the new initial count
     * @note only call befor create, after this have no effect
     */ 
    void set_initial_count(int count)       { m_uiCount = count; }
    /**
     * Set the max count
     * 
     * @param count the new max count
     * @note only call befor create, after this have no effect
     */ 
    void set_max_count(int count)           { m_uiMaxCount = count; }

    /**
     * Get the current count value. 
     *  
     * @return The current count and 0 if the semaphore is not available.
     */ 
    int get_count() const;
  protected:
    int m_uiCount;
    int m_uiMaxCount;
  };

  using counting_semaphore_t = basic_counting_semaphore;
}

#endif