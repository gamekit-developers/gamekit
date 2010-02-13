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
#include "gkSerialize.h"

// default values



gkCameraProperties::gkCameraProperties()
:       clipstart(0.1f),
        clipend(100.f),
        fov(49.13f),
        start(false)
{
}



gkEntityProperties::gkEntityProperties()
:       source(""),
        prefab(-1),
        casts(false),
        group(false)
{
}



gkGameObjectProperties::gkGameObjectProperties()
:       position(0, 0, 0),
        scale(1, 1, 1),
        orientation(1, 0, 0, 0),
        isStatic(false),
        showAABB(false)
{
}


gkLightProperties::gkLightProperties()
:       diffuse(1, 1, 1, 1),
        specular(1, 1, 1, 1),
        type(0),
        spot_inner(30),
        spot_outer(40),
        direction(0, 0, -1),
        power(1),
        falloff(1),
        range(10000),
        constant(1),
        linear(0),
        quadratic(0),
        casts(false),
        extra(false)

{
    param[0] = 0; param[1] = 0;
    param[2] = 0; param[3] = 0;
}

gkSceneProperties::gkSceneProperties()
:       manager("generic"),
        world_color(0.501f, 0.501f, 0.501f, 1.f),
        ambient(0, 0, 0, 1),
        gravity(0, 0, -10),
        skyMat(""), skyDist(10000.f)
{
}

gkAnimProperties::gkAnimProperties()
:       anim(""),
        start(1),
        end(1),
        speed(0),
        blend(0),
        state(APS_LOOP)
{
}
