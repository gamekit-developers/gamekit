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
#ifndef _gkLoaderCommon_h_
#define _gkLoaderCommon_h_

#include "gkCommon.h"
#include "gkManualLoader.h"
#include "gkHashTable.h"
#include "gkArray.h"
#include "gkList.h"

/// utility for consistency
#define GKB_IDNAME(x) ((x) && (x)->id.name[0] != '0' ? (x)->id.name + 2 : "")


namespace bParse
{
// ----------------------------------------------------------------------------
class bBlenderFile;
}

namespace Blender
{
// ----------------------------------------------------------------------------
class Scene;
class Object;
class Mesh;
class Material;
class Image;
class MTex;
}


class Ogre::ManualResourceLoader;



// ----------------------------------------------------------------------------
class gkBlendFile;
class gkSceneObjectLoader;
class gkGameObjectLoader;

typedef gkList<Ogre::ManualResourceLoader*> gkManualResourceLoaderList;
typedef gkListIterator<gkManualResourceLoaderList> gkManualResourceLoaderListIterator;

typedef gkList<gkManualObjectLoader*> gkManualObjectLoaderList;
typedef gkListIterator<gkManualObjectLoaderList> gkManualObjectLoaderListIterator;

typedef gkList<gkBlendFile*> gkBlendFileList;
typedef gkListIterator<gkBlendFileList> gkBlendFileIterator;

typedef gkList<gkSceneObject*> gkSceneList;
typedef gkListIterator<gkSceneList> gkSceneIterator;

typedef gkList<Ogre::Texture*> gkImageList;
typedef gkListIterator<gkImageList> gkImageIterator;

typedef gkHashTable<Blender::Image*, Ogre::Texture*> gkImageTextureHashMap;





#endif//_gkLoaderCommon_h_
