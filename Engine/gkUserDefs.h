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


class gkUserDefs
{
public:
	gkUserDefs();
	void load(const gkString& fname);
	void parseString(const gkString& key, const gkString& val);


	OgreRenderSystem        rendersystem;       // Ogre render system to use
	gkString                viewportOrientation;// portrait, landscaperight, landscapeleft
	int                     sceneManager;       // TODO scene manager to use
	gkString                log;                // Main log file name
	bool                    verbose;            // Enable/Disable Ogre print-out
	gkVector2               winsize;            // Window width & height
	gkString                wintitle;           // Window title
	bool                    fullscreen;         // Fullscreen mode
	int                     framingType;        // crop/letterbox/extend. Used when window is larger then requested
	gkString                resources;          // resources to load
	bool                    blendermat;         // convert meshes using blender materials
	bool					matblending;		// Eanable material blending mode
	bool                    grabInput;          // hide & grab the mouse
	bool                    debugFps;           // show fps and profiling information
	bool                    debugPhysics;       // enable / disable physics debugging
	bool                    debugPhysicsAabb;   // show / hide bounding box
	bool                    buildStaticGeometry;// Use Static geometry
	bool                    useBulletDbvt;      // Use Bullet Dynamic AABB Tree
	bool                    showDebugProps;     // Show variable debugging information.
	bool                    debugSounds;        // Show 3D sound debug info
	bool                    disableSound;       // Disable OpenAL sound.
	bool                    fsaa;               // Enable Full scene anti aliasing.
	int                     fsaaSamples;        // Anti aliasing samples.
	bool                    enableshadows;
	int                     defaultMipMap;      // Number of mipmaps to generate per texture (default 5)
	gkString				shaderCachePath;	// RTShaderSystem cache file path

	gkString                shadowtechnique;
	gkColor                 colourshadow;
	gkScalar                fardistanceshadow;

	gkString                extWinhandle;       // External Window Handle
	gkScalar				animFps;			// Default animation fps
	bool					rtss;				// Enable RTShadingSystem
	bool					hasFixedCapability; // Renderer supports fixed-function pipeline

	GK_INLINE bool			isD3DRenderSystem() { return isD3DRenderSystem(rendersystem); }

	static OgreRenderSystem getOgreRenderSystem(const gkString& val);
	static bool isD3DRenderSystem(OgreRenderSystem rs);
	static int getViewportFramingType(const gkString& val);
};


#endif//_gkUserDefs_h_
