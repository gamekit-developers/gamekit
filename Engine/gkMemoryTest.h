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
#ifndef _gkMemoryTest_h_
#define _gkMemoryTest_h_

#ifdef WIN32
# ifdef _MSC_VER
/* visual studio leak checks */
# define _CRTDBG_MAP_ALLOC 1
// {,,msvcr71d.dll}_crtBreakAlloc
#  include <stdlib.h>
#  include <stdio.h>
#  include <crtdbg.h>



inline int debug_hook(int type, char* msg, int* ret)
{
	printf("%s", msg);
	return 0;
}

#  define TestMemory  {                                \
        int flag=  _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);    \
        flag |= _CRTDBG_LEAK_CHECK_DF;                      \
        flag &= ~_CRTDBG_CHECK_CRT_DF;                      \
        _CrtSetDbgFlag(flag);                               \
        _CrtSetReportHook(debug_hook);                      \
    }

#   define PrintLeaks _CrtDumpMemoryLeaks();
#   include <windows.h>
#   define AddLeak { new int;}
#   define DisableLeakTest _CrtSetDbgFlag(0); _CrtSetReportHook(0);

# else // _MSC_VER
#   include <windows.h>
#   define TestMemory
#   define PrintLeaks
#   define AddLeak
#   define DisableLeakTest
# endif
# else // WIN32
#   define TestMemory
#   define PrintLeaks
#   define AddLeak
#   define DisableLeakTest
#endif


#endif//_gkMemoryTest_h_
