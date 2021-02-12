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
#ifndef MINLIB_ESP32_EVENT_GROUP_
#define MINLIB_ESP32_EVENT_GROUP_

#include "mn_config.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

namespace mn {
    /**
     * @ingroup base
     */ 
    class basic_event_group {
    public:
        /**
         * Create a new event group.
         *
         * @note Although event groups are not related to ticks, for internal implementation
         * reasons the number of bits available for use in an event group is dependent
         * on the configUSE_16_BIT_TICKS setting in FreeRTOSConfig.h.  If
         * configUSE_16_BIT_TICKS is 1 then each event group contains 8 usable bits (bit
         * 0 to bit 7).  If configUSE_16_BIT_TICKS is set to 0 then each event group has
         * 24 usable bits (bit 0 to bit 23).  The EventBits_t type is used to store
         * event bits within an event group.
         * 
         */ 
        basic_event_group();
        /**
         * Create from a FreeRTOS created event group
         */ 
        basic_event_group(EventGroupHandle_t handle);

        /**
         * Delete an event group 
         * Tasks that are blocked on the event group will be
         * unblocked and obtain 0 as the event group's value.
         */
        virtual ~basic_event_group();

        /**
         *  Allow two or more tasks to use an event group to sync each other.
         *
         * @param uxBitsToSet The bits to set in the event group before determining
         * if, and possibly waiting for, all the bits specified by the uxBitsToWait
         * parameter are set.
         *
         * @param uxBitsToWaitFor A bitwise value that indicates the bit or bits to test
         * inside the event group.  For example, to wait for bit 0 and bit 2 set
         * uxBitsToWaitFor to 0x05.  To wait for bits 0 and bit 1 and bit 2 set
         * uxBitsToWaitFor to 0x07.  Etc.
         *
         * @param xTicksToWait The maximum amount of time (specified in 'ticks') to wait
         * for all of the bits specified by uxBitsToWaitFor to become set.
         *
         * @return The value of the event group at the time either the bits being waited
         * for became set, or the block time expired.  Test the return value to know
         * which bits were set.  If xEventGroupSync() returned because its timeout
         * expired then not all the bits being waited for will be set.  If
         * xEventGroupSync() returned because all the bits it was waiting for were
         * set then the returned value is the event group value before any bits were
         * automatically cleared.
         */
        EventBits_t sync( const EventBits_t uxBitsToSet, const EventBits_t uxBitsToWaitFor,
                    TickType_t xTicksToWait);

        /**
         * [Potentially] block to wait for one or more bits to be set within a
         * previously created event group.
         *
         * This function cannot be called from an interrupt.
         *
         * @param uxBitsToWaitFor A bitwise value that indicates the bit
         * or bits to test inside the event group.
         *
         * @param xClearOnExit If xClearOnExit is set to true then any bits
         * set in the value passed as the uxBitsToWaitFor parameter will be
         * cleared in the event group before basic_event_group::wait returns
         * if basic_event_group::wait returns for any reason other than a
         * timeout. The timeout value is set by the xTicksToWait parameter.
         * If xClearOnExit is set to false then the bits set in the event
         * group are not altered when the call to basic_event_group::wait
         * returns.
         *
         * @param xWaitForAllBits is used to create either a logical AND
         * test (where all bits must be set) or a logical OR test (where
         * one or more bits must be set) as follows:
         *
         *     @par    If xWaitForAllBits is set to true then
         *             basic_event_group::wait will return when either all the
         *             bits set in the value passed as the uxBitsToWaitFor
         *             parameter are set in the event group or the specified
         *             block time expires.
         *
         *     @par   If xWaitForAllBits is set to false then
         *             basic_event_group::wait will return when any of the bits
         *             set in the value passed as the uxBitsToWaitFor
         *             parameter are set in the event group or the specified
         *             block time expires.
         *
         * @param xTicksToWait The maximum amount of time (specified in
         * 'ticks') to wait for one/all (depending on the xWaitForAllBits
         * value) of the bits specified by uxBitsToWaitFor to become set.
         *
         * @return The value of the event group at the time either the event
         * bits being waited for became set, or the block time expired. The
         * current value of the event bits in an event group will be
         * different to the returned value if a higher priority task or
         * interrupt changed the value of an event bit between the calling
         * task leaving the Blocked state and exiting the
         * basic_event_group::wait function. Test the return value to know
         * which bits were set. If basic_event_group::wait returned because its
         * timeout expired then not all the bits being waited for will be
         * set. If basic_event_group::wait returned because the bits it was
         * waiting for were set then the returned value is the event group
         * value before any bits were automatically cleared because the
         * xClearOnExit parameter was set to true.
         */
        EventBits_t wait( const EventBits_t uxBitsToWaitFor,
                    bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout);

        /**
         *  Clear bits (flags) within an event group.
         *
         *  @param uxBitsToClear A bitwise value that indicates the bit or
         *  bits to clear in the event group.
         *
         *  @return The value of the event group before the specified bits
         *  were cleared.
         */
        EventBits_t clear(const EventBits_t uxBitsToClear);

        /**
         * Returns the current value of the event bits (event flags) in an
         * event group.
         *
         * @return The value of the event bits in the event group at the time
         * basic_event_group::get was called.
         */
        EventBits_t get();

        /**
         * Set bits (flags) within an event group.
         *
         * @param uxBitsToSet A bitwise value that indicates the bit or bits
         * to set in the event group.
         *
         * @return The value of the event group at the time the call to
         * basic_event_group::set returns
         */
        EventBits_t set(const EventBits_t uxBitsToSet);

        EventGroupHandle_t get_handle() { return m_pHandle; }
    private:
        /**
         *  FreeRTOS Event Group handle.
         */
        EventGroupHandle_t m_pHandle;

        #if( configSUPPORT_STATIC_ALLOCATION == 1 )
            StaticEventGroup_t m_xEventGroupBuffer;
        #endif
    };

    using event_group_t = basic_event_group;
}
#endif