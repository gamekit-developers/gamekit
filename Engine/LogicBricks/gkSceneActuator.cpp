/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Xavier T.

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

#include "gkSceneActuator.h"
#include "gkGameObject.h"
#include "gkScene.h"
#include "gkSceneManager.h"
#include "gkEngine.h"


gkSceneActuator::gkSceneActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name),
		m_mode(0),
		m_sceneName(gkStringUtils::BLANK),
		m_camera(gkStringUtils::BLANK)
{
}


gkLogicBrick* gkSceneActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkSceneActuator* act = new gkSceneActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkSceneActuator::execute(void)
{
	gkScene* scene = 0;
	gkGameObject* obj = 0;

	if (isPulseOff())
		return;

	if (!m_object->isInstanced())
		return;

	switch (m_mode)
	{
	case SC_RESTART:
		m_object->getOwner()->reinstance(true);
		break;
	case SC_SET_SCENE:
		scene = (gkScene*)gkSceneManager::getSingleton().getByName(m_sceneName);
		if (scene && scene != m_object->getOwner())
		{
			scene->destroyInstance(true);
			m_object->getOwner()->destroyInstance(true);
			scene->createInstance(true);
		}
		break;
	case SC_SET_CAMERA:
		obj = m_object->getOwner()->getObject(m_camera);
		if (obj && obj->getType() == GK_CAMERA)
			m_object->getOwner()->setMainCamera((gkCamera*)obj);
		break;
	case SC_ADD_FRONT:
		break;
	case SC_ADD_BACK:
		break;
	case SC_REMOVE:
		break;
	case SC_SUSPEND:
		break;
	case SC_RESUME:
		break;
	}
	setPulse(BM_OFF);

}
