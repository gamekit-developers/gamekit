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

#include "gkCollisionNode.h"
#include "gkRigidBody.h"
#include "gkScene.h"
#include "gkDynamicsWorld.h"
#include "gkEngine.h"

using namespace Ogre;

gkCollisionNode::gkCollisionNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id),
m_bBody(0)
{
	ADD_ISOCK(*getEnable(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getTarget(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_ISOCK(*getCollidesWith(), this, gkLogicSocket::ST_STRING);
	ADD_OSOCK(*getHasCollided(), this, gkLogicSocket::ST_BOOL);
	ADD_OSOCK(*getCollided(), this, gkLogicSocket::ST_GAME_OBJECT);
	ADD_OSOCK(*getContactPosition(), this, gkLogicSocket::ST_VECTOR);
	
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	pScene->getDynamicsWorld()->EnableContacts(true);
}

gkCollisionNode::~gkCollisionNode()
{
}

bool gkCollisionNode::evaluate(Real tick)
{
	gkGameObject* pObj = getTarget()->getValueGameObject();

	m_bBody = 0;

	if(pObj && pObj->isLoaded())
	{
		m_bBody = pObj->getAttachedBody();
	}

	return m_bBody != 0;
}

void gkCollisionNode::update(Real tick)
{
	getHasCollided()->setValue(false);

	if(getEnable()->getValueBool())
	{
		m_bBody->setFlags(m_bBody->getFlags() | gkRigidBody::RBF_CONTACT_INFO);

		const gkRigidBody::ContactArray& contacts = m_bBody->getContacts();

		for (size_t i = 0; i < contacts.size(); i++)
		{
			const gkRigidBody::ContactInfo& c = contacts[i];

			if(getCollidesWith()->getValueString().empty() || c.collider->getObject()->getName().find(getCollidesWith()->getValueString()) != -1)
			{
				getContactPosition()->setValue(gkVector3(c.point.getPositionWorldOnA()));
				getCollided()->setValue(c.collider->getObject());
				getHasCollided()->setValue(true);
				break;
			}
		}
	}
	else
	{
		m_bBody->setFlags(m_bBody->getFlags() ^ gkRigidBody::RBF_CONTACT_INFO);
	}
}



