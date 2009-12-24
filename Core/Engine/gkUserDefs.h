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
#ifndef _gkUserDefs_h_
#define _gkUserDefs_h_

#include "gkCommon.h"
#include "gkMathUtils.h"
#include "gkRenderFactory.h"





// ----------------------------------------------------------------------------
class gkUserDefs
{
public:
	gkUserDefs();
	void load(const Ogre::String &fname);

	Ogre::String		plugins;			// default plugin dir
	OgreRenderSystem	rendersystem;		// main render system
	Ogre::String		log;				// name of output log
	bool				verbose;			// print everything Ogre has to say
	Ogre::Vector2		winsize;			// window size
	Ogre::String		wintitle;
	bool				fullscreen;
	Ogre::String		resources;			// resources to load
	Ogre::Real			animspeed;
	Ogre::Real			startframe;
	Ogre::Real			tickrate;
	bool				blendermat;

	// disabled for now
	bool				multipassmat;
	Ogre::String		gl_vert_profile;	// arbvp1
	Ogre::String		gl_frag_profile;	// arbfp1
	Ogre::String		dx_vert_profile;	// vs_1_1
	Ogre::String		dx_frag_profile;	// ps_2_x
};




#endif//_gkUserDefs_h_
