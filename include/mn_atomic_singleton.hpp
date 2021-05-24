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
#ifndef __MINLIB_ATOMIC_SINGLETON_H__
#define __MINLIB_ATOMIC_SINGLETON_H__

#include "mn_config.hpp"

#include "mn_atomic.hpp"
#include "mn_autolock.hpp"
#include "mn_copyable.hpp"


namespace mn {

	/**
	 * @brief Singleton with double-checked locking pattern with atomic and mutex lock
	 * @tparam T The type of the class to be make as singleton.
	 */
	template < typename T>
	class basic_atomic_singleton {
		friend class T;
	public:
		using value_type = T;
        using reference = value_type&;
        using pointer = T*;
        using self_type = basic_atomic_singleton<T>;

		basic_atomic_singleton(self_type&& other) = delete;
		basic_atomic_singleton(const self_type&) = delete;

		static reference instance() {
			pointer _tmp = m_pInstance.load (memory_order::Consume);
			if(!_tmp) {
				automutx_t lock(m_tLock);

				_tmp = m_pInstance.load (memory_order::Consume);
				if( !_tmp ) {
					_tmp = new T();
					m_pInstance.store (_tmp, memory_order::Release);
				}
			}
			return tmp;
		}
	private:
		static mn::_atomic<pointer> m_pInstance;
        static mn::mutex_t  m_tLock;
	};

	template < typename T >
    mn::_atomic<T*> basic_atomic_singleton<T>::m_pInstance;

    template < typename T >
    mn::mutex_t basic_atomic_singleton<T>::m_tLock;

	template <typename T>
	using atomic_singleton = basic_atomic_singleton<T>;
}


#endif
