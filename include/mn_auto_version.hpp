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
#ifndef _MINLIB_AUTO_VERSION_H_
#define _MINLIB_AUTO_VERSION_H_

namespace mn{

	//Date Version Types
	#define MN_VERSION_DATE "24"
	#define MN_VERSION_MONTH "05"
	#define MN_VERSION_YEAR "2021"
	#define MN_VERSION_UBUNTU_VERSION_STYLE  "21.05"

	//Software Status
	#define MN_VERSION_STATUS  "develop"
	#define MN_VERSION_STATUS_SHORT  "dev"

	//Standard Version Type
	#define MN_VERSION_MAJOR  2
	#define MN_VERSION_MINOR  295
	#define MN_VERSION_BUILD  8908
	#define MN_VERSION_REVISION  8908

	//Miscellaneous Version Types
	#define MN_VERSION_BUILDS_COUNT  206
	#define MN_VERSION_RC_FILEVERSION 2,295,8908,8908
	#define MN_VERSION_RC_FILEVERSION_STRING "2, 295, 8908, 8908\0"
	#define MN_VERSION_FULLVERSION_STRING  "2.295.8908.8908"

	//These values are to keep track of your versioning state, don't modify them.
	#define MN_VERSION_BUILD_HISTORY  0


}
#endif //_MINLIB_AUTO_VERSION_H_
