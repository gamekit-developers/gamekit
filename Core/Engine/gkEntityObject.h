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
#ifndef _gkEntityObject_h_
#define _gkEntityObject_h_

#include "gkGameObject.h"
#include "gkEntityObjectTypes.h"




// ---------------------------------------------------------------------------
class gkEntityObject : public gkGameObject
{
public:
	gkEntityObject(gkSceneObject *scene, const Ogre::String& name, gkManualObjectLoader* loader= 0);
	virtual ~gkEntityObject() {}

	gkEntityProperties& getEntityProperties(void);
	Ogre::Entity* getEntity(void);

	void _resetPose(void);
	void updateAnimations(void);

	gkAnimProperties& getAnimationProperties(void);

protected:
	gkEntityProperties mEntityProps;
	Ogre::Entity* mEntity;

	gkAnimProperties mAnimProps;

	Ogre::AnimationState* mActive, *mDefault;
	Ogre::Real mAnimTime, mBlendTime;
	Ogre::String mActiveName;


	virtual void loadImpl();
	virtual void unloadImpl();
};

// ---------------------------------------------------------------------------
GK_INLINE gkEntityProperties& gkEntityObject::getEntityProperties(void)
{
	return mEntityProps;
}

// ---------------------------------------------------------------------------
GK_INLINE Ogre::Entity* gkEntityObject::getEntity(void)
{
	return mEntity;
}

// ---------------------------------------------------------------------------
GK_INLINE gkAnimProperties& gkEntityObject::getAnimationProperties(void)
{
	return mAnimProps;
}






#endif//_gkEntityObject_h_
