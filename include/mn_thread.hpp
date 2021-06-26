/**
 * @file
 * @brief
 * This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
 * @author Copyright (c) 2021 Amber-Sophia Schroeck
 * @par License
 * The Mini Thread Library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, version 3, or (at your option) any later version.
 *
 * The Mini Thread Library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the Mini Thread  Library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef __MINILIB_THREAD_H__
#define __MINILIB_THREAD_H__

#include "mn_config.hpp"
#include "mn_functional.hpp"
#include "mn_function.hpp"

#include "mn_task.hpp"

namespace mn {

	class basic_thread : public basic_task {
	public:
		class id {
		public:

		};
		using base_type = basic_task;
		using native_handle_type = typename base_type::native_handle_type;
		using func_type = mn::function<int(void*)>;


		basic_thread(std::string strName, func_type* func,
				basic_task::priority uiPriority = basic_task::priority::Normal,
        		unsigned short  usStackDepth = MN_THREAD_CONFIG_MINIMAL_STACK_SIZE) noexcept
        		: base_type(strName, uiPriority, usStackDepth),
				  m_funcTion(func) {}


		basic_thread(const basic_thread&) = delete;
    	basic_thread& operator=(const basic_thread&) = delete;
	protected:
		int  on_task() {
			if(m_funcTion) return m_funcTion(m_userData);
			else return -127;
		}
	private:
		func_type m_funcTion;
	};

}

#endif // __MINILIB_THREAD_H__
