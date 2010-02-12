/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#ifndef _utStringDefines_h_
#define _utStringDefines_h_

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "utCommon.h"

#if UT_COMPILER == UT_COMPILER_MSVC
# define ut_snprintf	_snprintf
# define ut_vsnprintf	_vsnprintf
#else
# define ut_snprintf	snprintf
# define ut_vsnprintf	vsnprintf
#endif

#define utStrlen strlen
#define utCharNEq(a, b, n)  ((a && b) && !strncmp(a, b, n))
#define utCharEq(a, b)      ((a && b) && (*a == *b) && !strcmp(a, b))
#define utCharEqL(a, b, l)  ((a && b) && (*a == *b) && !memcmp(a, b, l))
#define utStrcpy           memcpy

#define UT_NullChar '\0'
#define UT_NullCharCheck(x) (!((x) && (x)[0] != UT_NullChar))

#define utALLOCPADDING 32

#endif//_utStringDefines_h_

