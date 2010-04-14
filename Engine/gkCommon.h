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
#ifndef _gkCommon_h_
#define _gkCommon_h_

#include "Utils/utCommon.h"
#include "gkHashedString.h"


#include "OgreBuildSettings.h"
#include "OgreConfig.h"
#include "OgrePlatform.h"
#include "OgrePrerequisites.h"

#include "Utils/utTypes.h"


#define GK_NPOS             UT_NPOS
#define GK_PLATFORM_WIN32   UT_PLATFORM_WIN32
#define GK_PLATFORM_APPLE   UT_PLATFORM_APPLE
#define GK_PLATFORM_LINUX   UT_PLATFORM_LINUX
#define GK_PLATFORM         UT_PLATFORM
#define GK_INLINE           UT_INLINE
#define GK_ASSERT           UT_ASSERT

#define GK_IMPLEMENT_SINGLETON(cls)\
    template<> cls* Ogre::Singleton<cls>::ms_Singleton= 0;\
    cls& cls::getSingleton() {  \
        UT_ASSERT(ms_Singleton);   \
        return *ms_Singleton;   \
    }\
    cls* cls::getSingletonPtr(){ \
        return ms_Singleton;\
    }

#define gkMakeHandle(name) typedef struct name##__ { int unused; } *name
#define GK_DEF_GROUP "General"



#define gkIS_PROGRAM(name)      (!Ogre::HighLevelGpuProgramManager::getSingleton().getByName(name).isNull())
#define gkIS_OPENGL()           (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != gkString::npos)
#define gkHAS_CAPABILITY(x)     (Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(x))


class gkCamera;
class gkEntity;
class gkLight;
class gkSkeleton;
class gkGameObject;
class gkEngine;
class gkVariable;
class gkKeyboard;
class gkMouse;
class gkScene;
class gkSceneManager;
class gkLogicManager;
class gkLogicTree;
class gkConstraint;
class gkLogicManager;
class gkVariable;
class gkWindowSystem;
class gkBlendFile;
class gkUserDefs;

class gkAction;
class gkActionChannel;
class gkActionManager;
class gkBezierSpline;


class gkGameObjectGroup;
class gkGameObjectInstance;

enum gkTransformSpace
{
    TRANSFORM_LOCAL,
    TRANSFORM_PARENT,
    TRANSFORM_WORLD,
};


#endif//_gkCommon_h_
