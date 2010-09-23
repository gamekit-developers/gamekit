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
#include "gkShowPhysicsNode.h"
#include "gkEngine.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkLogger.h"

gkShowPhysicsNode::gkShowPhysicsNode(gkLogicTree* parent, size_t id)
	: gkLogicNode(parent, id),
	  m_scene(0)
{
	ADD_ISOCK(ENABLE, false);
	ADD_ISOCK(SHOW_AABB, false);
}

void gkShowPhysicsNode::initialize()
{
	m_scene = gkEngine::getSingleton().getActiveScene();
}

bool gkShowPhysicsNode::evaluate(gkScalar tick)
{
	m_scene->getDynamicsWorld()->enableDebugPhysics(GET_SOCKET_VALUE(ENABLE), GET_SOCKET_VALUE(SHOW_AABB));

	return false;
}
