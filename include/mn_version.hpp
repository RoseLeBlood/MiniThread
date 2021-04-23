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
#ifndef _MINLIB_VERSION_H_
#define _MINLIB_VERSION_H_

#define MN_THREAD_VERSION_STRING_LEN sizeof(MN_VERSION_FULLVERSION_STRING) + sizeof(MN_VERSION_STATUS) + 8
#define MN_THREAD_VERSION_INFO(PRJNAME)  mn::minilib_version_print(PRJNAME)


#include "mn_config.hpp"
#include "mn_auto_version.hpp"

MN_EXTERNC_BEGINN

namespace mn {
	inline const char* minilib_version_string(char version_string[MN_THREAD_VERSION_STRING_LEN]) {

		snprintf(version_string, MN_THREAD_VERSION_STRING_LEN, "%s-%s-LGPL", MN_VERSION_FULLVERSION_STRING,
				MN_VERSION_STATUS);

		return version_string;
	}
	inline void minilib_version_print(const char* strPrjName, const char* strExtraInfo = "") {
		char _version_string[MN_THREAD_VERSION_STRING_LEN];
		minilib_version_string(_version_string);

		printf("%s using %s\n",strPrjName, _version_string);
			if(strlen(strExtraInfo) > 0) {
				printf("%s\n" , strExtraInfo);
			}
	}
}
MN_EXTERNC_END

#endif
