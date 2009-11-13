/* Copyright (C) 2006 Charlie C
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#ifndef __BCOMMON_H__
#define __BCOMMON_H__

#include <iostream>
#include <map>
#include <vector>
#include <assert.h>
#include "bLog.h"

namespace bParse {

	class bMain;
	class bFileData;
	class bFile;
	class bDNA;

	// delete void* undefined
	typedef struct bStructHandle {int unused;}bStructHandle;

	typedef std::vector<bStructHandle*>			bListBasePtr;
	typedef std::string							bString;
	typedef std::map<void *, bStructHandle*>	bPtrMap;
}


#endif//__BCOMMON_H__
