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
#ifndef _gkLightObject_h_
#define _gkLightObject_h_

#include "gkGameObject.h"
#include "gkLightObjectTypes.h"




// ---------------------------------------------------------------------------
class gkLightObject : public gkGameObject
{
public:
	gkLightObject(gkSceneObject *scene, const Ogre::String& name, gkManualObjectLoader* loader= 0);
	virtual ~gkLightObject() {}

	gkLightProperties& getLightProperties(void);

	void _update(void);
	Ogre::Light* getLight(void);

private:
	gkLightProperties mLightProps;
	Ogre::Light *mLight;

	virtual void loadImpl(void);
	virtual void unloadImpl(void);
};


// ---------------------------------------------------------------------------
GK_INLINE gkLightProperties& gkLightObject::getLightProperties(void)
{
	return mLightProps;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Light* gkLightObject::getLight(void)
{
	return mLight;
}






#endif//_gkLightObject_h_
