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
#ifndef _gkCameraObject_h_
#define _gkCameraObject_h_

#include "gkGameObject.h"
#include "gkCameraObjectTypes.h"
#include "OgreCamera.h"




// ----------------------------------------------------------------------------
class gkCameraObject : public gkGameObject
{
public:
	gkCameraObject(gkSceneObject *scene, const Ogre::String& name, gkManualObjectLoader* loader= 0);
	virtual ~gkCameraObject() {}


	gkCameraProperties& getCameraProperties(void);
	Ogre::Camera* getCamera(void);
	void makeCurrent(void);


	// Property access
	void setProperties(const gkCameraProperties& props);

	void setClip(Ogre::Real start, Ogre::Real end);
	void setFov(const Ogre::Radian& fov);
	void setFov(const Ogre::Degree& fov);

	void setMainCamera(bool v);


	const Ogre::Real& getClipStart(void);
	const Ogre::Real& getClipEnd(void);
	const Ogre::Radian& getFov(void);
	bool isMainCamera(void);


private:
	gkCameraProperties mCameraProps;
	Ogre::Camera *mCamera;

	virtual void loadImpl(void);
	virtual void unloadImpl(void);
};

// ----------------------------------------------------------------------------
GK_INLINE Ogre::Camera* gkCameraObject::getCamera(void)
{
	return mCamera;
}

// ----------------------------------------------------------------------------
GK_INLINE gkCameraProperties& gkCameraObject::getCameraProperties(void)
{
	return mCameraProps;
}

// ----------------------------------------------------------------------------
GK_INLINE void gkCameraObject::setProperties(const gkCameraProperties& props)
{
	mCameraProps= props;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::Real& gkCameraObject::getClipStart(void)
{
	return mCameraProps.clipstart;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::Real& gkCameraObject::getClipEnd(void)
{
	return mCameraProps.clipend;
}

// ----------------------------------------------------------------------------
GK_INLINE const Ogre::Radian& gkCameraObject::getFov(void)
{
	return mCameraProps.fov;
}

// ----------------------------------------------------------------------------
GK_INLINE bool gkCameraObject::isMainCamera(void)
{
	return mCameraProps.start;
}





#endif//_gkCameraObject_h_
