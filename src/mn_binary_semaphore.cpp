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

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "mn_binary_semaphore.hpp"

namespace mn {
  //-----------------------------------
  //  construtor
  //-----------------------------------
  basic_binary_semaphore::basic_binary_semaphore() 
    : basic_semaphore() { 
    
  #if( configSUPPORT_STATIC_ALLOCATION == 1 )
    m_pSpinlock = xSemaphoreCreateBinaryStatic(&m_SemaphoreBasicBuffer);
  #else
    m_pSpinlock = xSemaphoreCreateBinary();
  #endif

    if (m_pSpinlock) {
      unlock();
    } else {
      THROW_LOCK_EXP(ERR_SPINLOCK_CANTCREATESPINLOCK);
    }
    
  }

  //-----------------------------------
  //  deconstrutor
  //-----------------------------------
  basic_binary_semaphore::~basic_binary_semaphore() {
    if(m_pSpinlock != NULL) {
      vSemaphoreDelete(m_pSpinlock);
    }
  }
}