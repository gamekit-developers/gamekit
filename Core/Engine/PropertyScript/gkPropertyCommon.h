/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
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
#ifndef _gkPropertyCommon_h_
#define _gkPropertyCommon_h_

#include "OgreString.h"
#include "OgreStringConverter.h"
#include "gkCommon.h"
#include "gkList.h"
#include "gkHashTable.h"
#include "gkHashedString.h"



// ----------------------------------------------------------------------------
class gkPropertyTree;
class gkPropertyNode;
class gkPropertyAttribute;
class gkPropertyScanner;

typedef gkList<gkPropertyNode*> gkNodeList;
typedef gkList<gkPropertyAttribute*> gkAttributeList;
typedef gkHashTable<gkHashedString, gkPropertyAttribute*> gkAttributeMap;
typedef gkListIterator<gkNodeList> gkNodeIterator;
typedef gkListIterator<gkAttributeList> gkAttributeIterator;

// ----------------------------------------------------------------------------
struct gkDebugNode
{
	size_t line;
	Ogre::String source;
};






#endif//_gkPropertyCommon_h_
