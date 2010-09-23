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
#ifndef _gkParticleNode_h_
#define _gkParticleNode_h_

#include "gkLogicNode.h"
#include "LinearMath/btQuickprof.h"

class gkParticleNode : public gkLogicNode
{
public:
	enum
	{
		CREATE,
		POSITION,
		ORIENTATION,
		PARTICLE_SYSTEM_NAME
	};

	DECLARE_SOCKET_TYPE(CREATE, bool);
	DECLARE_SOCKET_TYPE(POSITION, gkVector3);
	DECLARE_SOCKET_TYPE(ORIENTATION, gkQuaternion);
	DECLARE_SOCKET_TYPE(PARTICLE_SYSTEM_NAME, gkString);

	gkParticleNode(gkLogicTree* parent, size_t id);

	~gkParticleNode();

	void update(Ogre::Real tick);
	bool evaluate(Ogre::Real tick);

private:

	class ParticleSystem
	{
	public:

		ParticleSystem(const gkString& name, const gkVector3& position, const gkQuaternion& q);
		~ParticleSystem();

		bool HasExpired() { return m_timer.getTimeMilliseconds() > m_time_to_live; }

	private:

		Ogre::SceneNode* m_node;

		Ogre::ParticleSystem* m_particleSystem;

		gkScalar m_time_to_live;

		btClock m_timer;
	};

	typedef utList<ParticleSystem*> ParticleObjects;
	typedef utListIterator<ParticleObjects> ParticleObjectIterator;

	ParticleObjects    m_particles;

};

#endif//_gkParticleNode_h_
