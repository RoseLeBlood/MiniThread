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
#ifndef _MINLIB_cfc6e05b_d8d4_4a9d_ae4b_42fa56cd3443_H_
#define _MINLIB_cfc6e05b_d8d4_4a9d_ae4b_42fa56cd3443_H_

#define MN_TEMPLATE_FULL_DECL()                                     template< >
#define MN_TEMPLATE_FULL_DECL_ONE(d1,t1)		                    template <d1 t1>
#define MN_TEMPLATE_FULL_DECL_TWO(d1,t1,d2,t2)	                    template <d1 t1, d2 t2>
#define MN_TEMPLATE_FULL_DECL_THREE(d1,t1,d2,t2,d3,t3)	            template <d1 t1, d2 t2, d3 t3>
#define MN_TEMPLATE_DECL_ONE(t1)		                            MN_TEMPLATE_FULL_DECL_ONE(typename,t1)
#define MN_TEMPLATE_DECL_TWO(t1,t2)		                            MN_TEMPLATE_FULL_DECL_TWO(typename,t1,typename,t2)
#define MN_TEMPLATE_DECL_THREE(t1,t2,t3)	                        MN_TEMPLATE_FULL_DECL_THREE(typename,t1,typename,t2,typename,t3)
#define MN_TEMPLATE_TYPE_ONE(type,a1)		                        type<a1>
#define MN_TEMPLATE_TYPE_TWO(type,a1,a2)	                        type<a1,a2>
#define MN_TEMPLATE_TYPE_THREE(type,a1,a2,a3)	                    type<a1,a2,a3>

#define MN_TEMPLATE_USING_ONE(name, basic_name,d1,t1)               template <d1 t1> using name = basic_name<t1>;
#define MN_TEMPLATE_USING_TWO(name,basic_name, d1,t1,d2,t2)         template <d1 t1, d2 t2> using name = basic_name<t1, t2>;
#define MN_TEMPLATE_USING_THREE(name,basic_name,d1,t1,d2,t2,d3,t3)  template <d1 t1, d2 t2, d3 t3> using name = basic_name<t1, t2, t3>;
#define MN_TEMPLATE_USING(name, basic_name)                         using name = basic_name;

#endif