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
#ifndef _gkCameraObject_h_
#define _gkCameraObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"


class gkCamera : public gkGameObject
{
public:
	gkCamera(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkCamera() {}

	GK_INLINE gkCameraProperties& getCameraProperties(void) {return m_cameraProps;}


	void setClip(gkScalar start, gkScalar end);
	void setFov(const gkRadian& fov);
	void setFov(const gkDegree& fov);
	void setOrthoScale(const gkScalar& scale);
	void setMainCamera(bool v);
	void makeCurrent(void);
	void setProjType(gkCameraProperties::Type type);

	GK_INLINE Ogre::Camera* getCamera(void)              {return m_camera;}
	GK_INLINE const gkScalar& getClipStart(void)         {return m_cameraProps.m_clipstart;}
	GK_INLINE const gkScalar& getClipEnd(void)           {return m_cameraProps.m_clipend;}
	GK_INLINE const gkScalar& getFov(void)               {return m_cameraProps.m_fov;}
	GK_INLINE const gkScalar& getOrthoScale(void)        {return m_cameraProps.m_orthoscale;}
	GK_INLINE bool isMainCamera(void)                    {return m_cameraProps.m_start;}
	GK_INLINE gkCameraProperties::Type getProjType(void) {return (gkCameraProperties::Type)m_cameraProps.m_type;}

private:
	gkGameObject* clone(const gkString& name);

	gkCameraProperties  m_cameraProps;
	Ogre::Camera*        m_camera;

	virtual void createInstanceImpl(void);
	virtual void destroyInstanceImpl(void);
};

#endif//_gkCameraObject_h_
