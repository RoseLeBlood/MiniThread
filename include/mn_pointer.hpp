/**
* This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
* Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
* The Mini Thread Library is free software; you can redistribute it and/or modify  
* it under the terms of the GNU Lesser General Public License as published by  
* the Free Software Foundation, version 3, or (at your option) any later version.

* The Mini Thread Library is distributed in the hope that it will be useful, but 
* WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with the Mini Thread  Library; if not, see
* <https://www.gnu.org/licenses/>.  
*/
#ifndef _MINILIB_14670b4a_3b21_4a28_aece_ea03219c6250_H_
#define _MINILIB_14670b4a_3b21_4a28_aece_ea03219c6250_H_

#include "mn_config.hpp"
#include <stddef.h>
#include "mn_allocator.hpp"
#include "pointer/mn_save_ptr.hpp"
#include "pointer/mn_clone_ptr.hpp"
#include "pointer/mn_shared_ptr.hpp"
#include "pointer/mn_scoped_ptr.hpp"
#include "pointer/mn_lock_ptr.hpp"
#include "pointer/mn_weak_ptr.hpp"
#include "pointer/mn_linked_ptr.hpp"

#include "pointer/mn_any_ptr.hpp"



namespace mn {

    //----------------------------------------
    // USINGS
    //----------------------------------------
    MN_TEMPLATE_USING_ONE(clone_ptr, pointer::basic_clone_ptr, typename, T);
    MN_TEMPLATE_USING_TWO(clone_ptr_ex, pointer::basic_clone_ptr, typename, T, class, TInterface);
    MN_TEMPLATE_USING_ONE(shared_ptr, pointer::basic_shared_ptr, typename, T);
    MN_TEMPLATE_USING_ONE(scoped_ptr, pointer::basic_scoped_ptr, typename, T);
    MN_TEMPLATE_USING_ONE(save_ptr, pointer::basic_save_ptr, typename, T);
    MN_TEMPLATE_USING_ONE(lock_ptr, pointer::basic_save_ptr, typename, T);
    MN_TEMPLATE_USING_TWO(lock_ptr_ex, pointer::basic_lock_ptr, typename, T, class, TLOCK);
    MN_TEMPLATE_USING_ONE(weak_ptr, pointer::basic_weak_ptr, typename, T);
    MN_TEMPLATE_USING_ONE(linked_ptr, pointer::basic_linked_ptr, typename, T);

    MN_TEMPLATE_USING(any_ptr, pointer::basic_any_ptr<void>);


    //----------------------------------------
    // LOCK OKJECT
    //----------------------------------------

    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TLOCK)
    inline lock_ptr_ex<T, TLOCK>  lock_object(volatile T* v, TLOCK& m) {
        return lock_ptr_ex<T, TLOCK> (v, m);
    }

    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t)
    inline lock_ptr_ex<T, LockType_t>  lock_object(volatile T* v, LockType_t& m) {
        return lock_ptr_ex<T, LockType_t> (v, m);
    }

    //----------------------------------------
    // MAKE
    //----------------------------------------
    namespace internal {
        /**
         * @brief Make the given pointer 
         * 
         * @tparam TPOINTER The returned pointer class
         * @tparam T Value type of the pointer
         * @tparam TALLOCATOR The using allocator to create the raw pointer for the class.
         * @param value The value to hold
         * @return The new pointer class with the given value 
         */
        MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR) 
        inline T* make_buffer(T value) {
            TALLOCATOR ac;
            T* a = (T*)ac.alloc(sizeof(T)); assert(a != NULL); *a = value; 
            return a;
        }

        MN_TEMPLATE_FULL_DECL_ONE(typename, T) 
        inline T* make_buffer(T value) {
            memory::default_allocator_t ac;
            T* a = (T*)ac.alloc(sizeof(T)); assert(a != NULL); *a = value; 
            return a;
        }
    }

    


    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t) 
    inline shared_ptr<T> make_shared(T value) { 
        return shared_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    MN_TEMPLATE_FULL_DECL_THREE(typename, T, class, TInterface, class, TALLOCATOR = memory::default_allocator_t)
    inline clone_ptr_ex<T, TInterface> make_clone(T value) {
        return clone_ptr_ex<T, TInterface>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t) 
    inline clone_ptr<T> make_clone(T value) {
        return clone_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }


    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t)  
    inline scoped_ptr<T> make_scoped(T value) { 
        return scoped_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t)  
    inline save_ptr<T> make_save(T value) { 
        return save_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t)
    inline weak_ptr<T>  make_weak(T* value) { 
        return weak_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    inline linked_ptr<T>  make_link(const linked_ptr<T>& value) { 
        return linked_ptr<T>(value);
    }
    MN_TEMPLATE_FULL_DECL_TWO(typename, T, typename, U)
    inline linked_ptr<T>  make_link(const linked_ptr<U>& value) { 
        return linked_ptr<T>(value);
    }
    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TALLOCATOR = memory::default_allocator_t)
    inline linked_ptr<T>  make_link(T* value) { 
        return linked_ptr<T>(internal::make_buffer<T, TALLOCATOR>(value));
    }

    //----------------------------------------
    // SWAP
    //----------------------------------------

    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) {
        a.swap(b);
    }

    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(save_ptr<T> & a, save_ptr<T> & b) {
        a.swap(b);
    }

    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(weak_ptr<T> & a, weak_ptr<T> & b) {
        a.swap(b);
    }
}

#endif