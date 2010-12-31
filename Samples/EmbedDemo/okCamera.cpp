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
#define AXIS_OBJ_NAME		"AxisObj"
#define AXIS_LENGTH			2

#define MAT_X_AXIS			"axis/red"	
#define MAT_Y_AXIS			"axis/green"
#define MAT_Z_AXIS			"axis/blue"


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

okCamera::okCamera(Ogre::SceneManager* smgr, Ogre::Camera* camera, okCamera::CAMERA_MODE mode) 
	:	m_camera(NULL),
		m_sceneMgr(smgr),
		m_axis(NULL),
		m_showAxis(false),
		m_mode(mode),
		m_cameraNode(NULL),
		m_targetNode(NULL),
		m_axisNode(NULL),
		m_axisPos(CENTER)
{
	GK_ASSERT(smgr);

	static int sCount = 0;

	Ogre::SceneNode* rootNode = smgr->getRootSceneNode();

	m_targetNode = rootNode->createChildSceneNode();
	m_cameraNode = m_targetNode->createChildSceneNode();
	
	m_axisNode   = rootNode->createChildSceneNode();
	m_axis = createAxis(m_sceneMgr, utStringFormat(AXIS_OBJ_NAME"_%d", sCount++), AXIS_LENGTH);
	m_axis->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY+1);
	m_axisNode->attachObject(m_axis);
	m_axis->setVisible(m_showAxis);

	m_targetNode->setFixedYawAxis(true, Ogre::Vector3::UNIT_Y);
	m_cameraNode->setPosition(0,0,0);


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

	m_sceneMgr->destroySceneNode(m_cameraNode); m_cameraNode = NULL;
	m_sceneMgr->destroySceneNode(m_targetNode); m_targetNode = NULL;
	

	if (m_camera) { m_sceneMgr->destroyCamera(m_camera); m_camera = NULL; }
}

void okCamera::reset(const gkVector3& pos, CAMERA_DIR dir)
{
	if (m_camera) 
	{
		m_camera->setOrientation(gkQuaternion::IDENTITY);
		m_camera->setPosition(gkVector3::ZERO);
	}

	m_targetNode->setOrientation(gkQuaternion::IDENTITY);
	m_targetNode->setPosition(gkVector3::ZERO);

	m_cameraNode->setOrientation(gkQuaternion::IDENTITY);


	m_cameraNode->setPosition(pos);
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

//--

void okCamera::setTargetPostion(gkVector3 pos, bool relative)
{
	if (m_mode != MODE_TARGET)
		return;

	if (relative) pos += m_targetNode->getPosition();
	m_targetNode->setPosition(pos);
	zoom(0);
}


void okCamera::rotate(gkRadian yaw, gkRadian pitch, gkScalar zoom)
{

	if (m_mode == MODE_TARGET)
	{
		m_targetNode->yaw(yaw, Ogre::Node::TS_WORLD);
		m_targetNode->pitch(pitch, Ogre::Node::TS_LOCAL);

		m_cameraNode->translate(0, 0, zoom);
	}
	else
	{
		m_cameraNode->yaw(yaw, Ogre::Node::TS_WORLD);
		m_cameraNode->pitch(pitch, Ogre::Node::TS_LOCAL);

		m_cameraNode->translate(0, 0, zoom);
	}
}

void okCamera::update(const gkVector3& pos)
{
	
}

void okCamera::updateHelper()
{
#if 0
	if (m_showAxis)
		m_axisNode->setPosition(getScreenPos(m_axisPos));
#endif
}


void okCamera::setTargetMode(gkVector3 pos, gkScalar radius)
{
	m_mode = MODE_TARGET;
	m_targetNode->setPosition(pos);	
	rotate(gkRadian(0),gkRadian(0),radius);
}

void okCamera::setFreeMode()
{
	m_mode = MODE_FREE;
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
