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
#ifndef _MINILIB_14670b4a_3b21_4a28_aece_ea03219c6250_H_
#define _MINILIB_14670b4a_3b21_4a28_aece_ea03219c6250_H_

#include "mn_config.hpp"
#include <stddef.h>

#include "mn_allocator.hpp"

#include "pointer/mn_auto_ptr.hpp"
#include "pointer/mn_save_ptr.hpp"
#include "pointer/mn_clone_ptr.hpp"
#include "pointer/mn_shared_ptr.hpp"
#include "pointer/mn_scoped_ptr.hpp"
#include "pointer/mn_lock_ptr.hpp"




namespace mn {
    template<typename T> 
    using auto_ptr = pointer::basic_auto_ptr<T>;

    template<typename T, class TInterface = pointer::clone_ptr_interface<T> >
    using clone_ptr = pointer::clone_ptr<T, TInterface>;

    template < typename T > 
    using shared_ptr = pointer::basic_shared_ptr<T>;

    template <typename T>
    using scoped_ptr = pointer::basic_scoped_ptr<T>;

    template <typename T>
    using save_ptr = pointer::basic_save_ptr<T>;

    template <typename T, class TLOCK = LockType_t>
    using lock_ptr = pointer::basic_lock_ptr<T, TLOCK>;

    template <typename T, class TLOCK>
    inline lock_ptr<T, LockType_t>  lock_object(volatile T* v, TLOCK& m) {
        return lock_ptr<T, TLOCK> (v, m);
    }
    template <typename T>
    inline lock_ptr<T, LockType_t>  lock_object(volatile T* v, LockType_t& m) {
        return lock_ptr<T, LockType_t> (v, m);
    }
    template < >
    inline lock_ptr<void, LockType_t> lock_object(volatile void* v, LockType_t& m) {
        return lock_ptr<void, LockType_t> (v, m);
    }


    template<typename T, class TALLOCATOR = memory::default_allocator_t<T> > 
    inline auto_ptr<T> make_auto(T value) {
        T* a = TALLOCATOR.alloc(); assert(a != NULL); *a = value; 
        return auto_ptr<T>(a);
    }

    template<typename T, class TALLOCATOR = memory::default_allocator_t<T> > 
    inline shared_ptr<T> make_shared(T value) {
        T* a = TALLOCATOR.alloc(); assert(a != NULL); *a = value; 
        return shared_ptr<T>(a);
    }

    template<typename T, class TInterface = pointer::clone_ptr_interface<T>, class TALLOCATOR = memory::default_allocator_t<T> >
    inline clone_ptr<T, TInterface> make_clone(T value) {
        T* a = TALLOCATOR.alloc(); assert(a != NULL); *a = value;
        return clone_ptr<T, TInterface>(a);
    }

    template<typename T, class TALLOCATOR = memory::default_allocator_t<T> > 
    inline scoped_ptr<T> make_scoped(T value) {
        T* a = TALLOCATOR.alloc(); assert(a != NULL); *a = value; 
        return scoped_ptr<T>(a);
    }

    template<typename T > 
    inline scoped_ptr<T> make_scoped(auto_ptr<T> value) {
        return scoped_ptr<T>(value);
    }

    template<typename T, class TALLOCATOR = memory::default_allocator_t<T> > 
    inline save_ptr<T> make_save(T value) {
        T* a = TALLOCATOR.alloc(); assert(a != NULL); *a = value; 
        return save_ptr<T>(a);
    }

    template<class T>
    inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) {
        a.swap(b);
    }

    template<class T>
    inline void swap(save_ptr<T> & a, save_ptr<T> & b) {
        a.swap(b);
    }
}

#endif