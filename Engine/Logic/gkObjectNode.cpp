/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): Nestor Silveira.
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
#include "gkObjectNode.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkScene.h"


using namespace Ogre;



gkObjectNode::gkObjectNode(gkLogicTree* parent, size_t id) :
	gkLogicNode(parent, id), m_otherName(""), m_func(OB_FUNC_NONE)
{
	ADD_ISOCK(INPUT, true);
	ADD_ISOCK(IN_POSITION, gkVector3::ZERO);
	ADD_ISOCK(IN_ROTATION, gkVector3::ZERO);

	ADD_OSOCK(OUTPUT, false);
	ADD_OSOCK(OUT_POSITION, gkVector3::ZERO);
	ADD_OSOCK(OUT_ROTATION, gkVector3::ZERO);
}


bool gkObjectNode::evaluate(gkScalar tick)
{
	bool update = GET_SOCKET_VALUE(INPUT);

	SET_SOCKET_VALUE(OUTPUT, update);

	return update;
}

void gkObjectNode::initialize()
{
	if (!m_otherName.empty())
	{
		// look at global scope
		if (!m_other)
		{
			if (m_object)
			{
				gkScene* scene = m_object->getOwner();
				GK_ASSERT(scene);
				m_other = scene->getObject(m_otherName);
			}
		}
	}
}


void gkObjectNode::update(gkScalar tick)
{
	gkGameObject* ob = m_other ? m_other : m_object;
	if (!ob)
		return;

	if (GET_SOCKET(IN_POSITION)->isLinked())
		ob->setPosition(GET_SOCKET_VALUE(IN_POSITION));
	else
		SET_SOCKET_VALUE(OUT_POSITION, ob->getPosition());

	if (GET_SOCKET(IN_ROTATION)->isLinked())
	{
		gkQuaternion q = gkMathUtils::getQuatFromEuler(GET_SOCKET_VALUE(IN_ROTATION), true);
		ob->setOrientation(q);
		SET_SOCKET_VALUE(OUT_ROTATION, GET_SOCKET_VALUE(IN_ROTATION));
	}
	else
		SET_SOCKET_VALUE(OUT_ROTATION, gkMathUtils::getEulerFromQuat(ob->getOrientation(), true));
}
