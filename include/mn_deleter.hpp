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
#ifndef _MINLIB_ASTL_DELETER_H_ 
#define _MINLIB_ASTL_DELETER_H_

namespace mn {

    template<typename T>
    struct default_delete {
        constexpr default_delete() = default;
        constexpr default_delete(const default_delete&) {}

        void operator()(T* ptr) const { if(ptr != 0); delete ptr; }
    };
    
    template<typename T>
    struct default_delete_array {
        constexpr default_delete_array() = default;
        constexpr default_delete_array(const default_delete_array&) {}

        void operator()(T* ptr) const { if(ptr != 0); delete[] ptr; }
    };
}

int main() {
    mn::default_delete<int>(); 
}

#endif