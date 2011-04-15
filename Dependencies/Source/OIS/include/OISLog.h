/*
The zlib/libpng License

Copyright (c) 2005-2010 harkon.kr(http://gamekit.googlecode.com/)

This software is provided 'as-is', without any express or implied warranty. In no event will
the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial 
applications, and to alter it and redistribute it freely, subject to the following
restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that 
		you wrote the original software. If you use this software in a product, 
		an acknowledgment in the product documentation would be appreciated but is 
		not required.

    2. Altered source versions must be plainly marked as such, and must not be 
		misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/


#ifndef  OIS_LOG_H
#define OIS_LOG_H

#ifdef __ANDROID__

#include <android/log.h>

#define	OIS_LOGI(...)	__android_log_print(ANDROID_LOG_INFO, "OIS",__VA_ARGS__)
#define	OIS_LOGE(...)	__android_log_print(ANDROID_LOG_ERROR,"OIS",__VA_ARGS__)
#define _OIS_LOG_FOOT_	OIS_LOGI("%s %d", __FILE__, __LINE__);

#else

#define	OIS_LOGI(...)
#define	OIS_LOGE(...)
#define _OIS_LOG_FOOT_

#endif

#endif