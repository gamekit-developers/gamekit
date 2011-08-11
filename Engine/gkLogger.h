/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _gkLogger_h_
#define _gkLogger_h_

#include "gkString.h"
#include "gkCommon.h"
#include "gkDebugScreen.h"



class gkLogger
{
public:
	static void enable(const gkString& name, bool verbose);
	static void disable();
	static void write(const gkString& msg, bool force = false);
};

// printf style logging
extern void gkPrintf(const char* fmt, ...);



// std::cout style logging
#define gkLogMessage(msg) {                 \
    std::stringstream stream;               \
    stream << msg;                          \
    gkLogger::write(stream.str(), true);    \
}


#define gkPrint(x)    gkLogMessage(x)
#define gkPrintSTD(x) std::cout << x << std::endl

#define gkPrintSLM(src, line, msg)  gkPrint(src << '(' << line << "): " << msg)
#define gkPrintSLF(src, line, func)  gkPrint(src << '(' << line << "): " << func)

#ifdef __ANDROID__
#include <android/log.h>
#define _LOGI_(...)  __android_log_print(ANDROID_LOG_INFO,  "OgreKit", __VA_ARGS__);
#define _LOGE_(...)  __android_log_print(ANDROID_LOG_ERROR, "OgreKit", __VA_ARGS__);
#define _LOG_FOOT_   _LOGI_("%s (%d): %s", __FILE__, __LINE__, __FUNCTION__);
#else
#define _LOGI_(...)
#define _LOGE_(...)
#define _LOG_FOOT_
#endif

#endif//_gkLogger_h_
