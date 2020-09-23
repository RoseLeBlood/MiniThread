/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2020 Amber-Sophia Schroeck
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
#ifndef _MINLIB_ILOCK_INTERFACE_H_
#define _MINLIB_ILOCK_INTERFACE_H_

/**
 * Interface for all lock types in this library
 * This is an abstract base class.
 * To use this, you need to subclass it. All of your LockObjetcs should
 * be derived from the ILockObject class. Then implement the virtual lock
 * and unlock functions.
 */ 
class ILockObject {
public:
    /**
     *  Aquire (take) a LokObject
     *
     *  Example of blocking indefinitely:
     *      LockObject.Take();
     *
     *  Example of blocking for 100 ticks:
     *      LockObject.Take(100);
     *
     *  @param timeout How long to wait to get the Lock until giving up.
     */
	virtual int lock(unsigned int timeout) = 0;
    /**
     *  Release (give) a semaphore.
     */
	virtual int unlock() = 0;

    /**
     * Try to lock the ILockObject
     * 
     * @note call lock with timeout from 0
     * 
     * @return true if the Lock was acquired, false when not
     */
    virtual bool try_lock() {
        return (lock(0) == 0);
    }

    /**
     * Is the ILockObject created (initialized) ?
     * 
     * @return true if the ILockObject created (initialized) and false when not
     */
    virtual bool is_initialized() const = 0;
};

#endif