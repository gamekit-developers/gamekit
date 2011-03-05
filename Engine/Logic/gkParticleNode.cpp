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

#include "gkParticleNode.h"
#include "gkScene.h"
#include "gkEngine.h"
#include "gkGameObjectManager.h"
#include "gkParticleManager.h"
#include "gkParticleObject.h"
#include "gkUtils.h"
#include "Ogre/gkOgreParticleResource.h"

using namespace Ogre;

gkParticleNode::gkParticleNode(gkLogicTree* parent, size_t id)
	:	gkLogicNode(parent, id),
		m_scene(0)
{
	ADD_ISOCK(CREATE, false);
	ADD_ISOCK(POSITION, gkVector3::ZERO);
	ADD_ISOCK(ORIENTATION, gkQuaternion::IDENTITY);
	ADD_ISOCK(PARTICLE_SYSTEM_NAME, "");
}

gkParticleNode::~gkParticleNode()
{
	ParticleObjectIterator iter(m_particles);

	while (iter.hasMoreElements())
	{
		ParticleObject* p = iter.getNext();
		delete p;	
	}
}

void gkParticleNode::initialize()
{
	m_scene = gkEngine::getSingleton().getActiveScene();

	GK_ASSERT(m_scene);
}


bool gkParticleNode::evaluate(Real tick)
{
	return m_particles.size() || (GET_SOCKET_VALUE(CREATE) && !GET_SOCKET_VALUE(PARTICLE_SYSTEM_NAME).empty());
}

void gkParticleNode::update(Real tick)
{
	if (GET_SOCKET_VALUE(CREATE))
	{
		gkParticleObject* p = m_scene->createParticleObject(gkUtils::getUniqueName("gkParticleNode"));

		p->getParticleProperties().m_settings = GET_SOCKET_VALUE(PARTICLE_SYSTEM_NAME);
		p->createInstance();
		p->setOrientation(GET_SOCKET_VALUE(ORIENTATION));
		p->setPosition(GET_SOCKET_VALUE(POSITION));

		m_particles.push_back(new ParticleObject(p));
	}
	else
	{
		ParticleObjectIterator iter(m_particles);

		while (iter.hasMoreElements())
		{
			ParticleObject* p = iter.getNext();
			if (p->isExpired())
			{
				m_particles.erase(p);
				m_scene->destroyObject(p->m_parObj);
				delete p;
				//TODO: check iterator validation at erase.
			}
		}
	}
}

gkParticleNode::ParticleObject::ParticleObject(gkParticleObject* parObj)
	:	m_parObj(parObj),
		m_timeToLive(0)
{
	GK_ASSERT(m_parObj);

	gkOgreParticleResource* resource = static_cast<gkOgreParticleResource*>(m_parObj->getParticleResource()); //TOOD:change to gkParticleResource

	if (resource)
		m_timeToLive *= resource->getMaxTimeToLiveOfEmitters() * 1000;

	m_timer.reset();
}

gkParticleNode::ParticleObject::~ParticleObject()
{

}
