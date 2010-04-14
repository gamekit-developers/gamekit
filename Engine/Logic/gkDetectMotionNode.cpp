/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira.

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
#include "gkDetectMotionNode.h"
#include "gkLogger.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkGameObject.h"

gkDetectMotionNode::gkDetectMotionNode(gkLogicTree *parent, size_t id) 
: gkLogicNode(parent, id),
m_detected(true),
m_detections(-1)
{
	ADD_ISOCK(UPDATE, false);
	ADD_OSOCK(DETECTED, true);
	ADD_OSOCK(NOT_DETECTED, false);
}

bool gkDetectMotionNode::evaluate(gkScalar tick)
{
	return GET_SOCKET_VALUE(UPDATE) && m_detections;
}

void gkDetectMotionNode::update(gkScalar tick)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(pScene);

	if(pScene->hasChanged())
	{
		if(!m_detected)
		{
			m_detected = true;

			gkPrintf("Motion detected in scene");
			SET_SOCKET_VALUE(DETECTED, true);
			SET_SOCKET_VALUE(NOT_DETECTED, false);
			updateDetecttions();
		}
	}
	else if(m_detected)
	{
		m_detected = false;

		gkPrintf("Not motion detected in scene");
		SET_SOCKET_VALUE(DETECTED, false);
		SET_SOCKET_VALUE(NOT_DETECTED, true);
		updateDetecttions();
	}	
}

void gkDetectMotionNode::updateDetecttions()
{
	if(m_detections != -1)
	{
		--m_detections;
	}
}
