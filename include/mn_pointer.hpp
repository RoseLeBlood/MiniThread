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

#include "mn_def.hpp"
#include "mn_allocator.hpp"

#include "pointer/mn_save_ptr.hpp"
#include "pointer/mn_clone_ptr.hpp"
#include "pointer/mn_shared_ptr.hpp"
#include "pointer/mn_scoped_ptr.hpp"
#include "pointer/mn_lock_ptr.hpp"
#include "pointer/mn_weak_ptr.hpp"
#include "pointer/mn_linked_ptr.hpp"
#include "pointer/mn_any_ptr.hpp"
#include "pointer/mn_auto_ptr.hpp"

#include "pointer/mn_unique_ptr.hpp"

namespace mn {

    //----------------------------------------
    // USINGS
    //----------------------------------------
    MN_TEMPLATE_USING_ONE(clone_ptr, 	pointer::basic_clone_ptr, 	typename, T);
    MN_TEMPLATE_USING_TWO(clone_ptr_ex, pointer::basic_clone_ptr, 	typename, T, class, TInterface);
    MN_TEMPLATE_USING_ONE(shared_ptr, 	pointer::basic_shared_ptr, 	typename, T);
    MN_TEMPLATE_USING_ONE(scoped_ptr, 	pointer::basic_scoped_ptr, 	typename, T);
    MN_TEMPLATE_USING_ONE(save_ptr, 	pointer::basic_save_ptr, 	typename, T);
    MN_TEMPLATE_USING_ONE(lock_ptr, 	pointer::basic_save_ptr, 	typename, T);
    MN_TEMPLATE_USING_TWO(lock_ptr_ex, 	pointer::basic_lock_ptr, 	typename, T, class, TLOCK);
    MN_TEMPLATE_USING_ONE(weak_ptr, 	pointer::basic_weak_ptr, 	typename, T);
    MN_TEMPLATE_USING_ONE(linked_ptr, 	pointer::basic_linked_ptr, 	typename, T);

    MN_TEMPLATE_USING(any_ptr, 			pointer::basic_any_ptr<void>		   );
    MN_TEMPLATE_USING_ONE(auto_ptr, 	pointer::basic_auto_ptr, 	typename, T);

    //----------------------------------------
    // LOCK OKJECT
    //----------------------------------------

    /**
     * @brief
     */
    MN_TEMPLATE_FULL_DECL_TWO(typename, T, class, TLOCK)
    inline lock_ptr_ex<T, TLOCK>  lock_object(volatile T* v, TLOCK& m) {
        return lock_ptr_ex<T, TLOCK> (v, m);
    }

    /**
     * @brief
     */
    MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    inline lock_ptr_ex<T, LockType_t>  lock_object(volatile T* v, LockType_t& m) {
        return lock_ptr_ex<T, LockType_t> (v, m);
    }


    /**
     * @brief Make a shared pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline shared_ptr<T> make_shared(Args&&... args) {
        return shared_ptr<T>(new T (mn::forward<Args>(args)...) );
    }

    /**
     * @brief Make an clone pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, class TInterface, typename... Args >
    inline clone_ptr_ex<T, TInterface> make_clone(Args&&... args) {
        return clone_ptr_ex<T, TInterface>(new T (mn::forward<Args>(args)...) );
    }

    /**
     * @brief Make a clone pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline clone_ptr<T> make_clone(Args&&... args) {
        return clone_ptr<T>(new T (mn::forward<Args>(args)...) );
    }

	/**
     * @brief Make a scoped pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline scoped_ptr<T> make_scoped(Args&&... args) {
        return scoped_ptr<T>(new T (mn::forward<Args>(args)...) );
    }

    /**
     * @brief Make a save pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline save_ptr<T> make_save(Args&&... args) {
        return save_ptr<T>(new T (mn::forward<Args>(args)...) );
    }

    /**
     * @brief Make a weak pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
	template<typename T, typename... Args >
    inline weak_ptr<T>  make_weak(Args&&... args) {
        return weak_ptr<T>(new T (mn::forward<Args>(args)...) );
    }

    /**
     * @brief Make a auto pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
	inline auto_ptr<T> make_auto(Args&&... args) {
		return auto_ptr<T>(new T (mn::forward<Args>(args)...) );
	}

	/**
     * @brief Make a linked pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    inline linked_ptr<T>  make_link(const linked_ptr<T>& value) {
        return linked_ptr<T>(value);
    }

    /**
     * @brief Make a linked pointer from a other linked pointer type
     * @tparam T Value type of the pointer.
     * @tparam U Value of the type of td
     */
    MN_TEMPLATE_FULL_DECL_TWO(typename, T, typename, U)
    inline linked_ptr<T>  make_link(const linked_ptr<U>& value) {
        return linked_ptr<T>(value);
    }

    /**
     * @brief Make a link pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    MN_TEMPLATE_FULL_DECL_ONE(typename, T)
    inline linked_ptr<T>  make_link(T* value) {
        return linked_ptr<T>(value);
    }



    //----------------------------------------
    // SWAP
    //----------------------------------------

    /**
     * @brief swap a scoped pointer.
     */
    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(scoped_ptr<T> & a, scoped_ptr<T> & b) {
        a.swap(b);
    }

    /**
     * @brief swap a save pointer.
     */
    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(save_ptr<T> & a, save_ptr<T> & b) {
        a.swap(b);
    }

    /**
     * @brief swap a weak pointer.
     */
    MN_TEMPLATE_FULL_DECL_ONE(class, T)
    inline void swap(weak_ptr<T> & a, weak_ptr<T> & b) {
        a.swap(b);
    }

    /**
     * @brief swap a auto pointer.
     */
    MN_TEMPLATE_FULL_DECL_ONE(class, T)
	inline void swap(auto_ptr<T>& a, auto_ptr<T>& b) {
		a.swap(b);
	}
}


#endif
