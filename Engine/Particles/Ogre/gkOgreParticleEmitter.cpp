/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 harkon.kr.

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

#include "gkCommon.h"
#include "gkSerialize.h"
#include "gkOgreParticleResource.h"
#include "gkOgreParticleEmitter.h"
#include "gkOgreParticleObject.h"
#include "gkOgreParticleRenderer.h"
#include "gkMesh.h"

//#include "OgreAny.h"
//#include "OgreParticleSystem.h"
#include "Ogre.h"


const Ogre::String gkOgreParticleEmitter::NAME = "gkEmitter";

gkOgreParticleEmitter::gkOgreParticleEmitter(Ogre::ParticleSystem* psys)
	:	Ogre::ParticleEmitter(psys),
		m_creator(0)
{
	mType = NAME;
	mEmissionRate = 50;
}

gkOgreParticleEmitter::~gkOgreParticleEmitter()
{
}

unsigned short gkOgreParticleEmitter::_getEmissionCount(Ogre::Real timeElapsed)
{
	return genConstantEmissionCount(timeElapsed);
}

void gkOgreParticleEmitter::copyParametersTo(Ogre::StringInterface* dest) const
{
	Ogre::ParticleEmitter::copyParametersTo(dest);
	static_cast<gkOgreParticleEmitter*>(dest)->m_creator = m_creator;
}

gkVector3 randomInTri(gkVector3 p[3])
{
	gkScalar s = gkMath::RangeRandom(0, 1);
	gkScalar t = gkMath::RangeRandom(0, 1-s);
	gkScalar u = 1 - (s+t);

	return p[0]*s + p[1]*t + p[2]*u;
}

void gkOgreParticleEmitter::_initParticle(Particle* pParticle)
{
	GK_ASSERT(m_creator);

	gkParticleSettingsProperties& props = m_creator->getParticleProperties();

	ParticleEmitter::_initParticle(pParticle);

	pParticle->direction = props.m_velocity;

	if (mParent)
	{
		Ogre::Any any = mParent->getUserAny();
		if (!any.isEmpty())
		{
			gkOgreParticleObject* pobj = Ogre::any_cast<gkOgreParticleObject*>(any);
			gkMesh* mesh = pobj->getParticleProperties().m_mesh;
			if (mesh)
			{

				if (props.m_emitfrom == gkParticleSettingsProperties::EF_VERTS)
				{
					UTsize count = mesh->getMeshVertexCount();
					int i = rand() % count;
					const gkVertex& v = mesh->getMeshVertex(i);

					pParticle->position = v.co;
					
					if (props.m_velNormal != 0)
						pParticle->direction = (pParticle->direction + v.no * props.m_velNormal)/2; 
				}
				else // if (props.m_emitfrom == gkParticleSettingsProperties::EF_FACES)
				{
					UTsize count = mesh->getMeshTriFaceCount();
					int i = rand() % count;
					gkTriFace face = mesh->getMeshTriFace(i);

					pParticle->position = randomInTri(face.p);

					if (props.m_velNormal != 0)
						pParticle->direction = (pParticle->direction + face.normal() * props.m_velNormal)/2; 
				}
			}
		}
	}


	//setStartTime(props.m_start);
			
	//setTimeToLive(props.m_lifetime);
	//setDirection(props.m_velocity);
	//setParticleVelocity(props.m_velocity.length());

	//pParticle->position = getPosition();
	//pParticle->setDimensions(props.m_size, props.m_size);
	
	//genEmissionColour(pParticle->colour);
    //genEmissionDirection(pParticle->direction);
	//genEmissionVelocity(pParticle->direction); //props.m_velocity

	gkParticleVisualData* data = static_cast<gkParticleVisualData*>(pParticle->getVisualData());
	if (data)
		data->m_initDir = pParticle->direction;

	gkScalar size = gkMax(0.01f, Ogre::Math::RangeRandom(props.m_size - props.m_sizeRandom, props.m_size + props.m_sizeRandom));
	pParticle->setDimensions(size, size);
	pParticle->timeToLive = pParticle->totalTimeToLive = props.m_lifetime; //genEmissionTTL();
}


//--

	
Ogre::ParticleEmitter* gkOgreEmitterFactory::createEmitter(Ogre::ParticleSystem* psys) 
{
	gkOgreParticleEmitter* emit = new gkOgreParticleEmitter(psys);
	mEmitters.push_back(emit);
	return emit;
}