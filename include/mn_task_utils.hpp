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
#ifndef MINLIB_ESP32_THREAD_UTILS_
#define MINLIB_ESP32_THREAD_UTILS_

#include "mn_task.hpp"
#include "mn_autolock.hpp"

class basic_task_lock : public ILockObject {
public:
  basic_task_lock(task_t* task) : m_pTask(task) { }

  virtual int lock(int timeout = 0) { 
    basic_task::lock(m_pTask); return 0;
  }
  virtual int unlock(int timeout = 0) { 
    basic_task::unlock(m_pTask); return 0;
  }
private: 
  task_t*  m_pTask;
};

/**
 * A autolock type for a task_t objects
 * 
 * @note lock the running mutex
 */
using autotask_t = basic_autolock<basic_task_lock>;

#endif