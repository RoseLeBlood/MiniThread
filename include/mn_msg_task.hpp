/**
 * @file
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
#ifndef MINLIB_ESP32_MESSAGE_TASK_
#define MINLIB_ESP32_MESSAGE_TASK_

#include "mn_config.hpp"
#if MN_THREAD_CONFIG_CONDITION_VARIABLE_SUPPORT == MN_THREAD_CONFIG_YES


#include "mn_convar.hpp"
#include "mn_convar_task.hpp"
#include "queue/mn_queue.hpp"

namespace mn {
    namespace ext {
		/**
		* The specific task message
		*/
		struct task_message {
			using message_id = int;

			message_id id;              /*!< The message id */
			void* message;              /*!< The message*/


			task_message(message_id _id, void* _message = nullptr)
				: id(_id), message(_message) { }
		};
		/**
         * @brief Extends the basic_convar_task with a message queue support
         *
         * @note This is an abstract base class.
         * @note To use this, you need to subclass it. All of your task should
         * @note be derived from the basic_task class. Then implement the virtual on_message
         * @note function.
         *
         * @ingroup task
         */
        class basic_message_task : public basic_convar_task  {
        public:
        	using message_id = typename task_message::message_id;

            /**
             * @brief Constructor for this task.
             *
             * @param strName Name of the Task. Only useful for debugging.
             * @param uiPriority FreeRTOS priority of this Task.
             * @param usStackDepth Number of "words" allocated for the Task stack. default 2048
              */
            explicit basic_message_task(std::string strName = "message_task",
										basic_task::priority uiPriority = priority::Normal,
										unsigned short  usStackDepth = MN_THREAD_CONFIG_MINIMAL_STACK_SIZE);

            /**
             * @brief Add a pre-created task message to the task queue
             *
             * @param[in] msg The specific message you are adding to the task queue
             * @param timeout How long to wait to add the item to the queue
             */
            void post_msg(task_message* msg, unsigned int timeout);

            /**
             * @brief Create the task message and add the message to the task queue,
             * without message data
             *
             * @param msg_id The message id
             * @param timeout How long to wait to add the item to the queue
             */
            void post_msg(message_id msg_id, unsigned int timeout) {
                post_msg(new task_message(msg_id, nullptr), timeout );
            }
            /**
             * @brief Create the task message and add the message to the task queue,
             * with message data
             *
             * @param msg_id The message id
             * @param message_data The user message data for the task message
             * @param timeout How long to wait to add the item to the queue
             */
            void post_msg(message_id msg_id, void* message_data, unsigned int timeout) {
                post_msg(new task_message(msg_id, message_data), timeout );
            }

            basic_message_task(const basic_message_task&) = delete;
            basic_message_task& operator=(const basic_message_task&) = delete;
        protected:
            /**
             * @brief Implementation of your actual message code.
             * @note You must override this function.
             *
             * @param[in] id The current message id
             * @param[in] message Pointer of the real message
             */
            virtual void on_message(id_t id, void* message) = 0;
		private:
			 /**
             * @brief The current message handling code
             * @return NULL
             */
            int  on_task();
        protected:
            mutex_t m_ltMessageQueueLock;
            queue::queue_t m_qeMessageQueue;
            convar_t m_cvMessage;
        };

        using message_task_t = basic_message_task;
    }
}
#endif

#endif
