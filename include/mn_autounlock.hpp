/**
 * @file
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
#ifndef _MINLIB_AUTO_UN_LOCK_H_
#define _MINLIB_AUTO_UN_LOCK_H_

#include "mn_autolock.hpp"

namespace mn {
  /**
   * A auto unlock type for counting_semaphore_t objects
   */
  using autouncsemp_t = basic_autounlock<counting_semaphore_t>;

  /**
   * A auto unlock type for binary_semaphore_t objects
   */
  using autounbinsemp_t = basic_autounlock<binary_semaphore_t>;

  /**
   * A auto unlock type for mutex_t objects
   */
  using autounmutx_t = basic_autounlock<mutex_t>;

  #if (MN_THREAD_CONFIG_RECURSIVE_MUTEX == MN_THREAD_CONFIG_YES)
  //using autounremutx_t = basic_autounlock<remutex_t>;
  #endif


  using unlock_t = basic_autounlock<LockType_t>;
}

#endif

