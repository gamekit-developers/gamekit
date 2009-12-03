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
#ifndef _gkCommon_h_
#define _gkCommon_h_

#include "gkMacros.inl"

#include "OgreBuildSettings.h"
#include "OgreConfig.h"
#include "OgrePlatform.h"
#include "OgreIteratorWrappers.h"
#include "OgrePrerequisites.h"

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
# include "gkWarningTemp.inl"
#endif

#include "gkHashTable.h"
#include "gkHashedString.h"
#include "gkArray.h"
#include "gkList.h"

#define gkIS_PROGRAM(name)		(!Ogre::HighLevelGpuProgramManager::getSingleton().getByName(name).isNull())
#define gkIS_OPENGL()			(Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos)
#define gkHAS_CAPABILITY(x)		(Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(x))

// ---------------------------------------------------------------------------
class gkArmatureObject;
class gkCameraObject;
class gkEngine;
class gkEntityObject;
class gkGameObject;
class GameObjectManager;
class gkVariable;
class gkKeyboardDevice;
class gkLightObject;
class gkMouseDevice;
class gkSceneObject;
class gkSceneObjectManager;
class gkSoundReferenceObject;
class gkSoundObject;
class gkLogicManager;
class gkLogicTree;
class gkConstraint;
class gkGameObjectGroup;
class gkGameObjectGroupInstance;
class gkManualObjectLoader;
class gkConsole;
class gkLogicManager;
class gkVariable;
class gkDebugPage;
class gkWindowSystem;
class gkBlendFile;
class gkUserDefs;

// ---------------------------------------------------------------------------
typedef std::vector<gkGameObjectGroupInstance*> gkInstanceList;
typedef Ogre::VectorIterator<gkInstanceList> gkInstanceListIterator;

typedef std::vector<gkGameObject*> gkGameObjectList;

typedef HashMap<Ogre::String, gkGameObject*> gkGameObjectMap;
typedef Ogre::MapIterator<gkGameObjectMap> gkGameObjectMapIterator;

typedef HashMap<Ogre::String, gkGameObjectGroup*> gkGameObjectGroupMap;
typedef Ogre::MapIterator<gkGameObjectGroupMap> GameObjectGroupMapIterator;

typedef HashMap<Ogre::String, gkVariable*> gkVariableMap;
typedef Ogre::MapIterator<gkVariableMap> gkVariableMapIterator;

typedef std::vector<gkConstraint*> gkConstraintList;
typedef Ogre::VectorIterator<gkConstraintList> gkConstraintListIterator;

typedef gkHashTable<gkHashedString, gkGameObject*> gkGameObjectHashMap;
typedef gkHashTableIterator<gkGameObjectHashMap> GameObjectHashMapIterator;

typedef gkArray<gkGameObject*> gkGameObjectArray;
typedef gkArrayIterator<gkGameObjectArray> gkGameObjectArrayIterator;

#endif//_gkCommon_h_
