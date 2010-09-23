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
#include "gkMouseSensor.h"
#include "gkLogicManager.h"
#include "gkCamera.h"
#include "gkScene.h"

#include "OgreCamera.h"
#include "OgreSceneManager.h"


gkMouseDispatch::gkMouseDispatch()
{
}

gkMouseDispatch::~gkMouseDispatch()
{
}


void gkMouseDispatch::dispatch(void)
{
	// TODO need to sort.
	doDispatch(m_sensors);
}



gkMouseSensor::gkMouseSensor(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:       gkLogicSensor(object, link, name), m_type(MOUSE_NILL), m_rayQuery(0), m_last(false)
{
	m_dispatchType = DIS_MOUSE;
	connect();
}


gkMouseSensor::~gkMouseSensor()
{
	if (m_rayQuery && m_scene->isInstanced())
		m_scene->getManager()->destroyQuery(m_rayQuery);
}



gkLogicBrick* gkMouseSensor::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkMouseSensor* sens = new gkMouseSensor(*this);
	sens->m_rayQuery = 0;
	sens->cloneImpl(link, dest);
	return sens;
}


bool gkMouseSensor::query(void)
{
	if (m_type == MOUSE_NILL)
		return false;


	gkMouse* mse = gkWindowSystem::getSingleton().getMouse();
	switch (m_type)
	{
	case MOUSE_LEFT:
		return mse->isButtonDown(gkMouse::Left);
	case MOUSE_MIDDLE:
		return mse->isButtonDown(gkMouse::Middle);
	case MOUSE_RIGHT:
		return mse->isButtonDown(gkMouse::Right);
	case MOUSE_MOTION:
		return mse->moved;
	case MOUSE_WHEEL_UP:
		return mse->wheelDelta > 0;
	case MOUSE_WHEEL_DOWN:
		return mse->wheelDelta < 0;
	case MOUSE_MOUSE_OVER:
	case MOUSE_MOUSE_OVER_ANY:
		// use Ogre viewport to ray query
		if (m_last && !mse->moved)
			return m_last;
		m_last = rayTest();
		return m_last;
	}
	return false;
}


bool gkMouseSensor::rayTest(void)
{
	// cannot test no movable data,
	if (m_type == MOUSE_MOUSE_OVER && (m_object->getType() == GK_OBJECT || m_object->getType() == GK_SKELETON))
		return false;


	GK_ASSERT(m_object);
	gkCamera* cam = m_object->getOwner()->getMainCamera();
	Ogre::Camera* oc = cam->getCamera();


	gkMouse* mse = gkWindowSystem::getSingleton().getMouse();

	gkScalar ncx = mse->position.x / mse->winsize.x;
	gkScalar ncy = mse->position.y / mse->winsize.y;


	Ogre::Ray dest;
	oc->getCameraToViewportRay(ncx, ncy, &dest);

	if (m_rayQuery == 0)
	{
		Ogre::SceneManager* mgr = m_object->getOwner()->getManager();
		m_rayQuery = mgr->createRayQuery(dest);
	}
	else m_rayQuery->setRay(dest);


	// do the test
	Ogre::RaySceneQueryResult& res = m_rayQuery->execute();

	bool result = false;
	for (Ogre::RaySceneQueryResult::iterator it = res.begin(); it != res.end(); ++it)
	{
		Ogre::RaySceneQueryResultEntry ent = (*it);

		if (ent.movable == oc)
			continue;


		if (ent.movable)
		{
			if (m_type == MOUSE_MOUSE_OVER)
			{
				if (ent.movable == m_object->getMovable())
				{
					result = true;
					break;
				}
			}
			else
			{
				result = true;
				break;
			}
		}
	}
	return result;
}
