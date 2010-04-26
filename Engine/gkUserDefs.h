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
#ifndef _gkUserDefs_h_
#define _gkUserDefs_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkString.h"
#include "gkRenderFactory.h"


// User defined options (TODO needs cleaned up)
class gkUserDefs
{
public:
    gkUserDefs();
    void load(const gkString &fname);

    gkString                plugins;            // plugins to load, unused
    OgreRenderSystem        rendersystem;       // Ogre render system to use
    int                     sceneManager;       // TODO scene manager to use
    gkString                log;                // Main log file name
    bool                    verbose;            // Enable/Disable Ogre print-out
    gkVector2               winsize;            // Window width & height
    gkString                wintitle;           // Window title
    bool                    fullscreen;         // Fullscreen mode
    gkString                resources;          // resources to load
    gkScalar                animspeed;          // animation speed, depreciated
    gkScalar                startframe;         // animation frame at the start of the game
    gkScalar                tickrate;           // user defined tick rate, depreciated
    bool                    blendermat;         // convert meshes using blender materials
    bool                    userWindow;         // user defined in blender  
    bool                    grabInput;          // hide & grab the mouse
    bool                    debugPhysics;       // enable / disable physics debugging
    bool                    debugPhysicsAabb;   // show / hide bounding box
    bool                    buildInstances;     // Use instanced geometry
    bool                    useBulletDbvt;      // Use Bullet Dynamic AABB Tree
	bool					enableshadows;
	gkString				shadowtechnique;
	gkColor					colourshadow;
	gkScalar				fardistanceshadow;
};




#endif//_gkUserDefs_h_
