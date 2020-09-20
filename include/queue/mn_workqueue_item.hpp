/* 
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef MINLIB_ESP32_WORK_ITEM_QUEUE_
#define MINLIB_ESP32_WORK_ITEM_QUEUE_

/**
 *  This class encapsulates the idea of a discrete, non-repeating task.
 *  Create a work_queue_item when there is something you need to do on a different
 *  Thread, but doesn't have to happen periodically. This is a great 
 *  construct for one off fire and forget tasks.
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your work_queue_item should
 *  be derived from this class. Then implement the virtual on_work
 *  function.
 */
class work_queue_item {
public:
    /**
     *  Our constructor.
     *
     *  @param deleteAffter If you pass in a true, you are 
     *  requesing the work_queue_t itself to delete this work_queue_item after
     *  it has run it. 
     *  @note Only set deleteAffter = true if:
     *  1) You dynamically allocated it (i.e. used "new")
     *  2) After you call on_work() you promise never to touch 
     *     this object again. 
     */
    work_queue_item(bool deleteAffter = false) 
        : m_bCanDelete(deleteAffter) { }

    /**
     *  Our destructor.
     */
    virtual ~work_queue_item() { }
    
    /**
     *  Allows a client to decide if this work_queue_item is marked
     *  for automatic deletion.
     */
    bool can_delete() { return m_bCanDelete; }

    /**
     *  Implementation of your actual work_queue_item function.
     *  You must override this function.
     */
    virtual void on_work() = 0;

    virtual int create() { return 0; }
    virtual int destroy() { return 0; }
private:
    const bool m_bCanDelete;
};

using work_queue_item_t = work_queue_item;

#endif