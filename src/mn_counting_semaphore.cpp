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
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "mn_counting_semaphore.hpp"

namespace mn {
  //-----------------------------------
  //  construtor
  //-----------------------------------
  basic_counting_semaphore::basic_counting_semaphore(int count, int maxcount) 
    : basic_semaphore(), m_uiCount(count), m_uiMaxCount(maxcount) {
      
      if ( (m_uiMaxCount < m_uiCount) && (m_uiMaxCount == 0) ) {
        THROW_LOCK_EXP(ERR_SPINLOCK_BAD_INITIALCOUNT);
      } else {
      #if( configSUPPORT_STATIC_ALLOCATION == 1 )
        m_pSpinlock = xSemaphoreCreateCountingStatic(m_uiMaxCount, m_uiCount, &m_SemaphoreBasicBuffer);
      #else
        m_pSpinlock = xSemaphoreCreateCounting(m_uiMaxCount, m_uiCount);
      #endif

        if (m_pSpinlock) {
          unlock();
        } else {
          THROW_LOCK_EXP(ERR_SPINLOCK_CANTCREATESPINLOCK);
        }
      }
  }

  //-----------------------------------
  //  deconstrutor
  //-----------------------------------
  basic_counting_semaphore::~basic_counting_semaphore() {
    if(m_pSpinlock) {
      vSemaphoreDelete(m_pSpinlock);
    } else {
      THROW_LOCK_EXP(ERR_MUTEX_NOTINIT);
    }
  }

  //-----------------------------------
  //  get_count
  //-----------------------------------
  int basic_counting_semaphore::get_count() const {
    return uxQueueMessagesWaiting(m_pSpinlock);
  }
}