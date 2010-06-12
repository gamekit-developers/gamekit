/*
-------------------------------------------------------------------------------
--                                                                           --
--  This file is part of GameKit.                                            --
--  http://gamekit.googlecode.com/                                           --
--                                                                           --
--                                                                           --
--  Released under the zlib license.                                         --
--  http://www.gzip.org/zlib/zlib_license.html                               --
--                                                                           --
--                                                                           --
-------------------------------------------------------------------------------
*/
#ifndef _gkLuaGameLogic_h_
#define _gkLuaGameLogic_h_

#include "gkLuaMath.h"
#include "gkLuaPointer.h"

class Scene;
class GameObject;
class Debugger;
class RayTest;

typedef Pointer<Scene>          ScenePtr;
typedef Pointer<GameObject>     GameObjectPtr;
typedef Pointer<Debugger>       DebuggerPtr;
typedef Pointer<RayTest>        RayTestPtr;


// ----------------------------------------------------------------------------
enum TransformSpace
{
    TS_LOCAL,
    TS_PARENT,
    TS_WORLD,
};

// ----------------------------------------------------------------------------
typedef enum GameObjectTypes
{
    OB_UNKNOWN=0,
    OB_CAMERA,
    OB_LIGHT,
    OB_ENTITY,
    OB_EMPTY,
    OB_SKELETON,
};



#endif//_gkLuaGameLogic_h_
