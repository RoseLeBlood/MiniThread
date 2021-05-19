/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2021 Amber-Sophia Schroeck
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
#ifndef MINLIB_22d7d658_9601_4b2d_afb1_c4b43d387f33_H_
#define MINLIB_22d7d658_9601_4b2d_afb1_c4b43d387f33_H_

#include "../mn_config.hpp"

#include "../mn_algorithm.hpp"
#include "../mn_def.hpp"
#include "../utils/mn_addressof.hpp"

namespace mn {
    namespace pointer {

        template <class TPR>
        struct smart_pointer_traits {
            using value_type = typename TPR::value_type;
            using element_type = typename TPR::element_type;
            using reference = typename TPR::reference;
            using const_value_type = typename TPR::const_value_type;
            using pointer = typename TPR::pointer;

            static inline element_type* to_address(const TPR& _pPtr) noexcept {
        		return mn::addressof<element_type>(_pPtr.get()); }
        };

        template <class TPR>
        inline typename smart_pointer_traits<TPR>::pointer get_pointer(const TPR& _pPtr) {
            return _pPtr.get(); }

		template <class TPR>
        inline bool is_null(const TPR& _pPtr) {
            return _pPtr.get() == nullptr; }



    }
}


#endif
