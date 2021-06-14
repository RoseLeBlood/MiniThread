/**
 * @file
 * @brief A basic Event system
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

#ifndef __MINILIB_EVENT_H__
#define __MINILIB_EVENT_H__

#include "mn_config.hpp"

#include "mn_function.hpp"

#include "mn_autolock.hpp"
#include "container/mn_vector.hpp"
#include "mn_mutex.hpp"

namespace mn {

	struct base_eventargs {
		base_eventargs() = default;
		const char* get_name() { return "base_eventargs"; }
	};

	class exit_eventargs {
    public:
        exit_eventargs(int exitcode) : m_iExitCode(exitcode) { }

        int get_exitcode() { return m_iExitCode; }
        const char* get_name() { return "exit_eventargs"; }
    private:
        int m_iExitCode;
    };

    template <typename T>
    class handler_eventargs : public base_eventargs {
	public:
		handler_eventargs(T* handler) : m_pHandler(handler) { }

		T* get_handler() { return m_pHandler; }
		const char* get_name() { return "handler_eventargs"; }
	private:
		T* m_pHandler;
    };

	template<class TSender, typename TArgs = base_eventargs>
    struct base_event {
		using self_type = base_event<TSender, TArgs>;
		using args_type = TArgs;
		using sender_type = TSender;
		using sender_reference = TSender&;
		using function_type = mn::function<void(sender_reference, const args_type)>;

		base_event(function_type func) 		 : m_ptr2Func(func) { }
		base_event(const self_type& other)   : m_ptr2Func(other.m_ptr2Func) { }
		base_event(const self_type&& other)  : m_ptr2Func(mn::move(other.m_ptr2Func)) { }

		void emit(const sender_type& sender, const args_type args){
			get_function()(sender, args);
		}

		bool operator == (const self_type& pOther){
			return pOther.m_ptr2Func == m_ptr2Func;
		}

		self_type& operator = (self_type& other) {
			m_ptr2Func = other.m_ptr2Func; return *this;
		}

		function_type get_function() { return m_ptr2Func; }
	protected:
		function_type   m_ptr2Func;
	};

	template< class TSender, typename TArgs,
			  class TBaseEvent = base_event<TSender, TArgs>,
			  class TContainer = mn::container::vector<TBaseEvent>
			>
	class event_system {
	public:
		using self_type = event_system<TSender, TArgs, TBaseEvent, TContainer>;

		using sender_type = TSender;
		using args_type = TArgs;
		using container_type = TContainer;

		using value_type = TBaseEvent;
		using reference = TBaseEvent&;
		using const_reference = const TBaseEvent&;
		using pointer = TBaseEvent*;
		using const_pointer = const TBaseEvent*;

		using size_type = typename container_type::size_type;
		using allocator_type = typename container_type::allocator_type;

		event_system(size_type initialSize = 4, const allocator_type& allocator = allocator_type())
			: m_observers(initialSize, allocator), m_mutex() { }

		event_system(const self_type& other)
		 	: m_observers(other.m_observers),
		 	  m_mutex(other.m_mutex) { }

		void emit(const TSender& sender, const args_type args) {
			automutx_t lock(m_mutex);

            for(auto itr = m_observers.begin();
					 itr != m_observers.end();
					 itr++) {
                (*itr)->emit(sender, args);
            }
        }

		void bind( const_reference funcHandler) {
			automutx_t lock(m_mutex);

            m_observers.push_back(funcHandler);
        }
        void remove( const_reference funcHandler) {
        	automutx_t lock(m_mutex);

			auto it = m_observers.find(funcHandler);
			if(it == m_observers.end()) return;

			m_observers.erase(it);
        }

        void operator += ( const_reference funcHandler) {
            add(funcHandler);
        }
		void operator -= ( const_reference funcHandler) {
			rem(funcHandler);
		}
		void operator () ( const TSender& sender, const args_type args) {
			emit(sender, args);
		}
	private:
		container_type m_observers;
		mutex_t m_mutex;
	};
}

#endif // __MINILIB_EVENT_H__
