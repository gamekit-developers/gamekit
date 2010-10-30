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


#include "StdAfx.h"
#include "okCamera.h"

#define USE_DEBUG_CAMERA	0
#define DEFAULT_MIN_RADIUS	0.1f
#define DEFAULT_MAX_RADIUS	100.0f
#define AXIS_OBJ_NAME		"AxisObj"
#define AXIS_LENGTH			2

#define MAT_X_AXIS			"axis/red"	
#define MAT_Y_AXIS			"axis/green"
#define MAT_Z_AXIS			"axis/blue"


//-- spehre coordinate


GK_INLINE gkVector3 sphere2cart(const gkVector3& p) //p(radius,yaw,pitch)
{
	gkVector3 t(gkVector3::ZERO);

	t.x = p.x * Ogre::Math::Sin(p.y) * Ogre::Math::Sin(p.z + Ogre::Math::PI/2);
	t.y = p.x * Ogre::Math::Cos(p.z + Ogre::Math::PI/2);
	t.z = p.x * Ogre::Math::Cos(p.y) * Ogre::Math::Sin(p.z + Ogre::Math::PI/2);

	return t;
}

GK_INLINE gkVector3 cart2sphere(const gkVector3& p) //ret(radius,yaw,pitch)
{
	gkVector3 t(gkVector3::ZERO);

	gkScalar len = p.length();
	
	t.x = len;
	t.y = Ogre::Math::ATan2(p.x, p.z).valueRadians();
	t.z = Ogre::Math::ACos(p.y / len).valueRadians() - Ogre::Math::PI/2;

	return t;
}



Ogre::ManualObject *createAxis(Ogre::SceneManager* smgr, const gkString& name, float axisLen)
{
	GK_ASSERT(smgr && !name.empty() && axisLen > 0);

	//create axis materials, if not exist.
	Ogre::MaterialManager& mmgr = Ogre::MaterialManager::getSingleton();
	if (!mmgr.resourceExists(MAT_X_AXIS))
	{
		static const gkString group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;
		static const char* names[] = { MAT_X_AXIS, MAT_Y_AXIS, MAT_Z_AXIS };
		static const Ogre::ColourValue colors[] = { Ogre::ColourValue::Red, Ogre::ColourValue::Green, Ogre::ColourValue::Blue };
		
		for (int i = 0; i < 3; i++)
		{
			Ogre::MaterialPtr mat = mmgr.create(names[i], group);
			Ogre::Pass *pass = mat->getTechnique(0)->getPass(0);
			//pass->setLightingEnabled(false);
			pass->setAmbient(colors[i]);
			pass->setDiffuse(colors[i]);
			pass->setSpecular(colors[i]);
		}
	}

	//create axis
	Ogre::ManualObject *mobj = smgr->createManualObject(name);

	gkVector3 p0(0,0,0), px(axisLen,0,0), py(0,axisLen,0), pz(0,0,axisLen);

	float tx = 0.15f, ty = 0.25f, tz = 0.2f;
	float x = px.x + 0.2f, y = py.y + 0.5f, z= pz.z + 0.2f;

	//--

	mobj->begin(MAT_X_AXIS, Ogre::RenderOperation::OT_LINE_LIST); 
	
	//x-axis
	mobj->position(p0); mobj->position(px); 

	//x-char
	mobj->position(x - tx,  ty, 0); mobj->position(x + tx, -ty, 0);
	mobj->position(x - tx, -ty, 0); mobj->position(x + tx,  ty, 0);
	
	mobj->end();

	//--

	mobj->begin(MAT_Y_AXIS, Ogre::RenderOperation::OT_LINE_LIST); 
	
	//y-axis
	mobj->position(p0); mobj->position(py); 

	//y-char
	mobj->position(-tx, y + ty, 0); mobj->position(0, y, 0);
	mobj->position( tx, y + ty, 0); mobj->position(0, y, 0);
	mobj->position(  0, y - ty, 0); mobj->position(0, y, 0);

	mobj->end();

	//--

	mobj->begin(MAT_Z_AXIS, Ogre::RenderOperation::OT_LINE_LIST); 
	
	//z-axis
	mobj->position(p0); mobj->position(pz); 

	//z-char
	mobj->position(-tx,  tz, z); mobj->position(tx,  tz, z);
	mobj->position(-tx, -tz, z); mobj->position(tx,  tz, z);
	mobj->position(-tx, -tz, z); mobj->position(tx, -tz, z);

	mobj->end();

	return mobj;
}

//--

okCamera::okCamera(Ogre::SceneManager* smgr, Ogre::Camera* camera, okCamera::CAMERA_MODE mode) : 
	m_camera(NULL),
	m_sceneMgr(smgr),
	m_axis(NULL),
	m_showAxis(false),
	m_mode(mode),
	m_cameraNode(NULL),
	m_targetNode(NULL),
	m_axisNode(NULL),
	m_axisPos(CENTER),	
	m_spherePos(gkVector3::ZERO),
	m_offsetPos(gkVector3::ZERO),
	m_minRadius(DEFAULT_MIN_RADIUS),
	m_maxRadius(DEFAULT_MAX_RADIUS),
	m_usePitchLimit(true)
{
	GK_ASSERT(smgr);

	static int sCount = 0;

	Ogre::SceneNode* rootNode = smgr->getRootSceneNode();

	m_cameraNode = rootNode->createChildSceneNode();
	m_targetNode = rootNode->createChildSceneNode();
	m_axisNode   = rootNode->createChildSceneNode();

	m_axis = createAxis(m_sceneMgr, utStringFormat(AXIS_OBJ_NAME"_%d", sCount++), AXIS_LENGTH);
	m_axis->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY+1);
	m_axisNode->attachObject(m_axis);

	m_axis->setVisible(m_showAxis);

	m_cameraNode->setFixedYawAxis(true);

	if (camera) 
		attachCamera(camera);
}

okCamera::~okCamera()
{
	if (!m_cameraNode) return; //something wrong!

	m_cameraNode->detachAllObjects();

	if (m_axisNode) 
	{
		m_sceneMgr->destroySceneNode(m_axisNode); m_axisNode = NULL;
	}
	if (m_axis) 
	{
		m_sceneMgr->destroyMovableObject(m_axis); m_axis = NULL;
	}

	m_sceneMgr->destroySceneNode(m_targetNode); m_targetNode = NULL;
	m_sceneMgr->destroySceneNode(m_cameraNode); m_cameraNode = NULL;

	if (m_camera) { m_sceneMgr->destroyCamera(m_camera); m_camera = NULL; }
}

void okCamera::reset(const gkVector3& pos)
{
	if (m_camera) 
	{
		m_camera->setOrientation(gkQuaternion::IDENTITY);
		m_camera->setPosition(gkVector3::ZERO);
	}

	m_cameraNode->setOrientation(gkQuaternion::IDENTITY);
	m_cameraNode->setPosition(pos);

	m_spherePos = gkVector3::ZERO;
	m_offsetPos = gkVector3::ZERO;	
}

bool okCamera::attachCamera(Ogre::Camera* camera)
{
	GK_ASSERT(camera);

	gkVector3 pos = camera->getDerivedPosition();

	if (camera->getParentSceneNode())
		camera->getParentSceneNode()->detachObject(camera);

	detachCamera();

	m_cameraNode->attachObject(camera);
	m_camera = camera;

	reset(pos);

	m_cameraNode->_updateBounds();

	return true;
}

Ogre::Camera* okCamera::detachCamera()
{
	if (m_camera) 
	{		
		Ogre::Camera* camera = m_camera;
		m_cameraNode->detachObject(m_camera);
		m_camera = NULL;
		return camera;
	} 

	return NULL;
}

//--

gkScalar okCamera::getBoundingRadius() 
{ 
	GK_ASSERT(m_camera);

	return m_camera->getBoundingRadius(); 
}

void okCamera::setPosition(const gkVector3& pos)
{
	if (m_mode == MODE_TARGET) 
	{
		gkVector3 tpos = pos - m_targetNode->getPosition();
		m_spherePos = cart2sphere(tpos);
		m_spherePos.x = gkMin(m_maxRadius, gkMax(m_minRadius, m_spherePos.x));
		m_cameraNode->setPosition(pos);
		m_cameraNode->lookAt(m_targetNode->getPosition(), Ogre::Node::TS_WORLD);
	} 
	else 
	{
		m_cameraNode->setPosition(pos);
		m_spherePos = gkVector3(pos.length(), 0, 0);
	}
}

void okCamera::setOrientation(const gkQuaternion& ori)
{
	if (m_mode == MODE_TARGET) 
	{
		//can't change orietation
	} 
	else 
	{
		m_cameraNode->setOrientation(ori);
	}
}

void okCamera::setDirection(const gkVector3& dir)
{
	//lookAt(dir);

	m_cameraNode->setDirection(dir);
}

//--

void okCamera::setTargetPostion(gkVector3 pos, bool relative)
{
	if (m_mode != MODE_TARGET)
		return;

	if (relative) pos += m_targetNode->getPosition();
	m_targetNode->setPosition(pos);
	zoom(0);
}

void okCamera::move(const gkVector3& trans)
{
	if (m_mode == MODE_TARGET) 
	{
		gkVector3 to = getPosition() + trans;
		setPosition(to);
	} 
	else 
	{
		gkVector3 to = getOrientation() * trans;
		//m_cameraNode->translate(to, Ogre::SceneNode::TS_LOCAL);
		m_cameraNode->setPosition(getPosition() + to);
	}
}

gkVector3 okCamera::calcSpherePos(gkRadian yaw, gkRadian pitch, gkScalar zoom)
{
	gkVector3 spherePos = m_spherePos;

	spherePos.x += zoom;
	spherePos.y += yaw.valueRadians();
	spherePos.z += pitch.valueRadians();

	const gkScalar limitPitch = gkPi/2 - 0.05;

	spherePos.x  = gkMin(m_maxRadius, gkMax(m_minRadius, spherePos.x));
	

	if (m_usePitchLimit)
		spherePos.z = gkMax(-limitPitch, gkMin(limitPitch, spherePos.z));
	else
	{
		if (spherePos.z >= gkPi/2)
			spherePos.y -= gkPi/2;
		else if (spherePos.z <= -gkPi/2)
			spherePos.y += gkPi/2;
	}

	spherePos.y -= int(spherePos.y  / (gkPi*2)) * gkPi*2;

	return spherePos;
}

gkVector3 okCamera::calcPos(gkRadian yaw, gkRadian pitch, gkScalar zoom)
{
	if (m_mode == MODE_TARGET) 
	{
		gkVector3 spherePos = calcSpherePos(yaw, pitch, zoom);

		gkVector3 tpos = m_targetNode->getPosition();
		gkVector3 cpos = getPosition();
		gkVector3 t = sphere2cart(spherePos);
		
		return (tpos + t);
	} 
	else 
	{
		gkVector3 tpos = m_cameraNode->getPosition();
		tpos.z += zoom;
		return tpos;
	}
}

void okCamera::rotate(gkRadian yaw, gkRadian pitch, gkScalar zoom)
{
	if (m_mode == MODE_TARGET) 
	{
		m_spherePos = calcSpherePos(yaw, pitch, zoom);

		gkVector3 tpos = m_targetNode->getPosition();
		gkVector3 cpos = getPosition();
		gkVector3 npos = tpos + sphere2cart(m_spherePos);;

		m_cameraNode->setPosition(npos);
		m_cameraNode->lookAt(tpos, Ogre::Node::TS_WORLD);

		if (m_offsetPos != gkVector3::ZERO)
			m_cameraNode->setPosition(npos + m_cameraNode->getOrientation() * m_offsetPos);

	} 
	else 
	{
		m_cameraNode->pitch(pitch);
		m_cameraNode->rotate(gkVector3(0,1,0),yaw, Ogre::Node::TS_WORLD);

		move(gkVector3(0,0,zoom));
	}
}

void okCamera::update(const gkVector3& pos)
{
	if (m_mode == MODE_FREE) return;

	gkVector3 disp = (pos - m_targetNode->getPosition()) * 0.1;
	m_targetNode->translate(disp);
	zoom(0);
	
}

void okCamera::updateHelper()
{
#if 0
	if (m_showAxis)
		m_axisNode->setPosition(getScreenPos(m_axisPos));
#endif
}

void okCamera::lookAt(const gkVector3& pos)
{
	m_cameraNode->lookAt(pos, Ogre::Node::TS_WORLD);
}

gkRadian okCamera::getYaw()
{
	if (m_mode == MODE_FREE)
		return m_cameraNode->getOrientation().getYaw();
	else
		return gkRadian(m_spherePos.y);
}

gkRadian okCamera::getPitch()
{
	if (m_mode == MODE_FREE)
		return m_cameraNode->getOrientation().getPitch();
	else
		return gkRadian(m_spherePos.z);
}

gkScalar okCamera::getRadius()
{
	if (m_mode == MODE_FREE)
		return 0;
	else
		return m_spherePos.x;
}

bool okCamera::setRadius(gkScalar radius, bool adjustRange)
{
	if (m_mode == MODE_FREE)
		return false;

	if (!adjustRange && (m_minRadius > radius || radius > m_maxRadius))
		return false;

	m_spherePos.x = radius;
	if (radius < m_minRadius) m_minRadius = radius;
	if (radius > m_maxRadius) m_maxRadius = radius;

	zoom(0);

	return true;
}

bool okCamera::setRadiusRange(gkScalar minRadius, gkScalar maxRadius) 
{ 
	if (maxRadius < minRadius) 
		return false;

	m_minRadius = minRadius; 
	m_maxRadius = maxRadius;

	return true;
}


void okCamera::setTargetMode(gkVector3 pos, gkScalar radius)
{
	m_mode = MODE_TARGET;
	m_targetNode->setPosition(pos);
	m_spherePos = gkVector3(radius,0,0);
	rotate(gkRadian(0),gkRadian(0),0);
}

void okCamera::setFreeMode()
{
	m_mode = MODE_FREE;
}

void okCamera::setClipDistance(gkScalar nearDist, gkScalar farDist)
{
	GK_ASSERT(m_camera);

	if (nearDist >= 0) m_camera->setNearClipDistance(nearDist);
	if (farDist  >= 0) m_camera->setFarClipDistance(farDist);
}

void okCamera::setAspectRatio(gkScalar ratio)
{
	GK_ASSERT(m_camera);

	m_camera->setAspectRatio(ratio);	
}

void okCamera::setPolygonMode(Ogre::PolygonMode  sd)
{
	GK_ASSERT(m_camera);

	m_camera->setPolygonMode(sd);
}


gkVector3 okCamera::getScreenPos(SCREEN_POS sp)
{			
	gkVector3 pos = getPosition();
	gkQuaternion ori = getOrientation();

	const float x=20, y=13, z=40;

	if (sp == LT)
		return pos +  ori * gkVector3(-x, y,-z); 
	else if (sp == LB)
		return pos +  ori * gkVector3(-x,-y,-z); 
	else if (sp == RT)
		return pos +  ori * gkVector3(x,y,-z); 
	else if (sp == RB)
		return pos +  ori * gkVector3(x,-y,-z); 
	
	//CENTER
	if (m_targetNode)
		return m_targetNode->_getDerivedPosition();
	
	return gkVector3::ZERO;

}

void okCamera::showAxis(bool show, okCamera::SCREEN_POS pos)
{
	m_showAxis = show;
	m_axisPos = pos;

	if (m_axisNode)
		m_axisNode->setVisible(m_showAxis);
}

void okCamera::setViewport(gkScalar left, gkScalar top, gkScalar right, gkScalar bottom)
{
	if (!m_camera) return;

	Ogre::Viewport* vp = getViewport();
	if (!vp) return;
		
	vp->setDimensions(left, top, right-left, bottom-top);

	gkScalar l = vp->getLeft(), t = vp->getTop();
	gkScalar r = l + vp->getWidth(), b = t + vp->getHeight();

}

void okCamera::setOverlaysEnabled(bool enable)
{
	Ogre::Viewport* vp = getViewport();
	if (vp) vp->setOverlaysEnabled(enable);
}
