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
#ifndef _gkSerialize_h_
#define _gkSerialize_h_

// For potential future use as a custom file loader.
// For now the properties here are used to pass
// data from Blender objects to OgreKit objects

#include "gkMathUtils.h"
#include "gkString.h"
#include "Utils/utString.h"

// Camera datatypes
class gkCameraProperties
{
public:
    gkCameraProperties();

    gkScalar    clipstart;
    gkScalar    clipend;
    gkScalar    fov;
    bool        start;
};


// Mesh/Entity datatypes
class gkEntityProperties
{
public:
    gkEntityProperties();

    gkString    source;
    int         prefab;
    bool        casts;
    bool        group;
};


// Base object types
typedef enum gkGameObjectTypes
{
    GK_OB_NULL = 0,
    GK_CAMERA,
    GK_LIGHT,
    GK_ENTITY,
    GK_OBJECT,
    GK_SKELETON,
}gkGameObjectTypes;



// GameObject datatypes
class gkGameObjectProperties
{
public:

    gkGameObjectProperties();

    gkVector3           position;
    gkVector3           scale;
    gkQuaternion        orientation;
    bool                isStatic;
    bool                showAABB;
};


// Light datatypes
class gkLightProperties
{
public:
    gkLightProperties();

    gkColor             diffuse;
    gkColor             specular;
    int                 type;
    gkScalar            spot_inner;
    gkScalar            spot_outer;
    gkVector3           direction;
    gkScalar            power;
    gkScalar            falloff;
    gkScalar            range;
    gkScalar            constant;
    gkScalar            linear;
    gkScalar            quadratic;
    bool                casts;

    bool                extra;
    gkScalar            param[4];
};



// Scene datatypes
class gkSceneProperties
{
public:
    gkSceneProperties();
    gkString        manager;
    gkColor         world_color;
    gkColor         ambient;
    gkVector3       gravity;
    gkString        skyMat;
    gkScalar        skyDist;
};

// Animation datatypes
class gkAnimProperties
{
public:
    gkAnimProperties();

    gkString            anim;
    gkScalar            start;
    gkScalar            end;
    gkScalar            speed;
    gkScalar            blend;
    int state;

    enum State
    {
        APS_LOOP = 0,
        APS_PLAY_STOP,
    };
};

// TODO use bullet serialization

#endif//_gkSerialize_h_
