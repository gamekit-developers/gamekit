/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 harkon.kr

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


#ifndef _okCamera_h_
#define _okCamera_h_


class okCamera 
{
public:
	enum CAMERA_MODE { MODE_FREE, MODE_TARGET };
	enum CAMERA_DIR  { DIR_RESET, DIR_TOP, DIR_FRONT, DIR_RIGHT };
	enum SCREEN_POS  { CENTER, LT, RT, LB, RB };

protected:
	CAMERA_MODE m_mode;

	Ogre::SceneManager* m_sceneMgr;
	Ogre::Camera* m_camera;

	Ogre::SceneNode* m_targetNode;
	Ogre::SceneNode* m_cameraNode;

	Ogre::SceneNode* m_axisNode;
	Ogre::MovableObject* m_axis;
	bool m_showAxis;
	SCREEN_POS m_axisPos;


public:
	okCamera(Ogre::SceneManager* smgr, Ogre::Camera* camera=NULL, CAMERA_MODE mode=MODE_FREE);
	virtual ~okCamera();
	
	CAMERA_MODE getMode() { return m_mode; }
	void setMode(CAMERA_MODE mode) { m_mode = mode; }

	bool attachCamera(Ogre::Camera* camera);
	Ogre::Camera* detachCamera();

	GK_INLINE Ogre::Camera* getCamera() { return m_camera; }

	void showAxis(bool show, SCREEN_POS pos = CENTER);
	GK_INLINE bool getShowAxis() { return m_showAxis; }

	void setViewport(gkScalar left, gkScalar top, gkScalar right, gkScalar bottom);

	void setAspectRatio(gkScalar ratio);
	void setPolygonMode(Ogre::PolygonMode sd);

	void setTargetPostion(gkVector3 pos, bool relative=false);
	void setTargetMode(gkVector3 pos, gkScalar Radius=5.0f);
	void setFreeMode();

	GK_INLINE void zoom(gkScalar zoom)   { rotate(gkRadian(0), gkRadian(0), zoom);  }
	GK_INLINE void yaw(gkRadian yaw)     { rotate(yaw, gkRadian(0), 0);	        }
	GK_INLINE void pitch(gkRadian pitch) { rotate(gkRadian(0), pitch, 0);         }

	void rotate(gkRadian yaw, gkRadian pitch, gkScalar zoom=0.0f);

	GK_INLINE gkVector3    getPosition()    { return m_cameraNode->getPosition();    }
	GK_INLINE gkQuaternion getOrientation() { return m_cameraNode->getOrientation(); }
	GK_INLINE gkVector3    getDirection()   { return getOrientation() * gkVector3::NEGATIVE_UNIT_Z; }

	gkVector3 getScreenPos(SCREEN_POS sp);

	GK_INLINE Ogre::Viewport* getViewport() { return m_camera ? m_camera->getViewport() : NULL; }
	GK_INLINE gkString getName() { return m_camera ? m_camera->getName() : ""; }

	gkScalar getBoundingRadius();

	GK_INLINE Ogre::SceneNode* getCameraNode()  { return m_cameraNode; }
	GK_INLINE Ogre::SceneNode* getTargetNode()  { return m_targetNode; }

	void reset(const gkVector3& pos, CAMERA_DIR dir = DIR_RESET);
	void update(const gkVector3& pos);

	void updateHelper(); //axis etc

	void setOverlaysEnabled(bool enable);

	GK_INLINE Ogre::Ray getCameraToViewportRay(gkScalar relX, gkScalar relY) 
	{ return m_camera ? m_camera->getCameraToViewportRay(relX, relY) : Ogre::Ray(); }
};



#endif //_okCamera_h_