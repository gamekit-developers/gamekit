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
#include "OgreRoot.h"
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"

using namespace Ogre;

gkParticleNode::gkParticleNode(gkLogicTree *parent, size_t id)
: gkLogicNode(parent, id)
{
	ADD_ISOCK(*getCreate(), this, gkLogicSocket::ST_BOOL);
	ADD_ISOCK(*getPosition(), this, gkLogicSocket::ST_VECTOR);
	ADD_ISOCK(*getParticleSystemName(), this, gkLogicSocket::ST_STRING);
}

gkParticleNode::~gkParticleNode()
{
	ParticleObjectIterator iter(m_particles);

	while (iter.hasMoreElements())
	{
		ParticleSystem* pParticle = iter.getNext();

		delete pParticle;
	}
}

bool gkParticleNode::evaluate(Real tick)
{
	return m_particles.size() || (getCreate()->getValueBool() && !getParticleSystemName()->getValueString().empty());
}

void gkParticleNode::update(Real tick)
{
	if(getCreate()->getValueBool())
	{
		m_particles.push_back(new ParticleSystem(getParticleSystemName()->getValueString(), getPosition()->getValueVector3()));
	}
	else
	{
		ParticleObjectIterator iter(m_particles);
		
		while (iter.hasMoreElements())
		{
			ParticleSystem* pParticle = iter.getNext();

			if(pParticle->HasExpired())
			{
				ParticleObjects::Pointer p = m_particles.find(pParticle);
				
				m_particles.erase(p);

				delete pParticle;

				break;
			}
		}
	}
}

gkParticleNode::ParticleSystem::ParticleSystem(const gkString& name, const gkVector3& position)
: m_node(0),
m_particleSystem(0),
m_time_to_live(0)
{
	gkScene* pScene = gkEngine::getSingleton().getActiveScene();

	m_node = pScene->getManager()->getRootSceneNode()->createChildSceneNode();

	m_node->setPosition(position);

	GK_ASSERT(!m_particleSystem);

	static int idx = 0;

	gkString instanceName = "gkParticleNode" + Ogre::StringConverter::toString(idx++);

	m_particleSystem = pScene->getManager()->createParticleSystem(instanceName, name);

	m_particleSystem->setQueryFlags(0);

	m_node->attachObject(m_particleSystem);

	unsigned short n = m_particleSystem->getNumEmitters();

	m_time_to_live = 0;

	for(unsigned short i=0; i<n; i++)
	{
		m_time_to_live = max(m_time_to_live, m_particleSystem->getEmitter(i)->getTimeToLive());
	}

	m_time_to_live *= 1000;

	m_timer.reset();
}

gkParticleNode::ParticleSystem::~ParticleSystem()
{
	if(m_node)
	{
		GK_ASSERT(m_particleSystem);

		m_node->detachObject(m_particleSystem);

		gkScene* pScene = gkEngine::getSingleton().getActiveScene();

		pScene->getManager()->destroyParticleSystem(m_particleSystem);

		m_particleSystem = 0;

		pScene->getManager()->getRootSceneNode()->removeAndDestroyChild(m_node->getName());

		m_node = 0;
	}
}


