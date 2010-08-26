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

#include "gkHashedString.h"

#if defined(WIN32)
// std min / max fix
#ifndef NOMINMAX
#define NOMINMAX 1 
#endif
#endif

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

#define GK_DEF_GROUP "General"

class gkObject;
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
class gkMeshManager;
class gkMesh;


class gkAction;
class gkActionChannel;
class gkActionManager;
class gkBezierSpline;

class gkGameObjectGroup;
class gkGameObjectInstance;
class gkGroupManager;

class gkConstraintManager;
class gkConstraint;

class gkPhysicsController;
class gkCharacter;
class gkRigidBody;


class gkLogicLink;
class btCollisionObject;
class btCollisionShape;

class gkDynamicsWorld;
class gkDebugger;
class gkScene;
class gkActiveObject;


// Common types
typedef utHashTable<gkHashedString, gkGameObject *>  gkGameObjectHashMap;
typedef utArray<gkGameObject *>                      gkGameObjectArray;

typedef utHashSet<gkGameObject *>                    gkGameObjectSet;
typedef utHashSet<gkCamera *>                        gkCameraSet;
typedef utHashSet<gkLight *>                         gkLightSet;
typedef utHashSet<gkEntity *>                        gkEntitySet;
typedef utHashSet<gkSkeleton *>                      gkSkeletonSet;
typedef utHashSet<gkPhysicsController *>             gkPhysicsControllerSet;
typedef utArray<gkPhysicsController *>               gkPhysicsControllers;


enum gkTransformSpace
{
	TRANSFORM_LOCAL,
	TRANSFORM_PARENT,
	TRANSFORM_WORLD,
};

// Generates a distinct type for each distinct constant integral
// value. Used to call one of several different functions, depending
// on a compile-time constant. [Alexandrescu (2000b)]
template<int v>
struct Int2Type
{
	enum { value = v };
};




// Do user initialize commands outside, logic / physics systems
struct gkInitCmd
{
	enum Cmd
	{
		REINITIALIZE,
		INITIALIZE,
		FINALIZE
	};

	gkObject *first;
	Cmd       second;

	GK_INLINE bool operator == (const gkInitCmd& cmd) const 
	{ return first == cmd.first && second == cmd.second; }
};

typedef utArray<gkInitCmd> gkInitializers;


#endif//_gkCommon_h_
