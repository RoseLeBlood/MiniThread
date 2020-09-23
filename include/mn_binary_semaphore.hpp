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
#ifndef MINLIB_ESP32_BINARY_SEMAPHORE_
#define MINLIB_ESP32_BINARY_SEMAPHORE_

#include "mn_basic_semaphore.hpp"

class basic_binary_semaphore : public basic_semaphore {
public:
  basic_binary_semaphore();
  virtual ~basic_binary_semaphore() { destroy(); }

  /**
   * Create the binary semaphore  
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are created, 'ERR_SPINLOCK_ALREADYINIT' the mutex are already created,
   *         'ERR_SPINLOCK_BAD_INITIALCOUNT' when the initial count greater than maxcount is and 
   *         'ERR_SPINLOCK_CANTCREATEMUTEX' on error.
   * 
   */
  virtual int create();

  /**
   * Destroyed the binary semaphore
   * 
   * @return 'ERR_SPINLOCK_OK' the mutex are destroyed 
   * or ERR_SPINLOCK_NOTINIT when mutex not created
   */
  virtual int destroy();
};

using binary_semaphore_t = basic_binary_semaphore;

#endif