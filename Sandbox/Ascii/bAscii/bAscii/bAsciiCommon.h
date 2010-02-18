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
#ifndef _bAsciiCommon_h_
#define _bAsciiCommon_h_

#include "bUtils/utHashTable.h"
#include "bUtils/utFixedString.h"

namespace Blender
{
	class Object;
}

struct bBlockDecl;

class bAscii;
class bAsciiMain;
class bStatement;
class bAsciiBlock;
class bAsciiCompiler;
class bAsciiScanner;
class bAsciiArgument;

// re-hash if table goes over this
#define utHashBlockSize 32


#define OP_UNKNOWN -1

// op enums must no exceed +127 
#define OP_MIN 0
typedef char bCode;

// ---------------------------------------------------------------------------
typedef utFixedString<32>	bStringF;
typedef utFixedString<256>	bStringFL;
typedef float				bNumber;
typedef utArray<bNumber>	bNumberList;
typedef utArray<bStringF>	bAlphaList;


// ---------------------------------------------------------------------------
typedef utArray<bAsciiBlock*>				bAsciiBlockArray;
typedef utArrayIterator<bAsciiBlockArray>	bAsciiBlockIterator;
typedef utArray<bAsciiArgument*>			bAsciiArgumentArray;

typedef utHashTable<int,		bAsciiArgument*,	utHashBlockSize>		bAsciiArgumentMap;
typedef utHashTable<bStringF,	bAsciiBlock*,		utHashBlockSize>		bAsciiBlockMap;
typedef utHashTable<int,		bAsciiBlock*,		utHashBlockSize>		bAsciiBlockMapI;

UT_INLINE UThash utHash(const bStringF& str) { return (UThash)str.hash(); }



#endif//_bAsciiCommon_h_
