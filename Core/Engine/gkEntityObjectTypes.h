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
#ifndef _gkEntityObjectTypes_h_
#define _gkEntityObjectTypes_h_

#include "gkCommon.h"
#include "OgrePrerequisites.h"
#include "gkMathUtils.h"
#include "OgreString.h"






// ---------------------------------------------------------------------------
class gkEntityProperties
{
public:
	gkEntityProperties();
	Ogre::String source;
	int prefab;
	bool casts, group;
};


// ---------------------------------------------------------------------------
class gkAnimProperties
{
public:
	gkAnimProperties();
	Ogre::String anim;
	Ogre::Real start, end;
	Ogre::Real speed, blend;
	int state;

	enum State
	{
		APS_LOOP= 0,
		APS_PLAY_STOP,
	};
};

// ---------------------------------------------------------------------------
GK_INLINE gkEntityProperties::gkEntityProperties() :
		source(Ogre::StringUtil::BLANK),
		prefab(-1), casts(true), group(false)
{
}

// ---------------------------------------------------------------------------
GK_INLINE gkAnimProperties::gkAnimProperties() :
		anim(Ogre::StringUtil::BLANK),
		start(1.0), end(1.0),
		speed(0.0), blend(0.0),
		state(APS_LOOP)
{
}





#endif//_gkEntityObjectTypes_h_
