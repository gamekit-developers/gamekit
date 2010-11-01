/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Nestor Silveira & Charlie C.

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
#include "gkSource.h"
#include "gkGameObject.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkBuffer.h"
#include "gkSound.h"
#include "gkSoundManager.h"
#include "gkEngine.h"
#include "gkLogger.h"
#include "gkUserDefs.h"




gkSource::gkSource(gkSound* sound)
	:   m_playback(0),
	    m_props(),
	    m_reference(sound)
{
}


gkSource::~gkSource()
{
	GK_ASSERT(!m_playback && "Playback not unbound");
}



void gkSource::bind(gkBuffer* buf)
{
	gkCriticalSection::Lock lock(m_cs);

	m_playback = buf;
	if (m_playback)
		m_playback->setProperties(m_props);
}



void gkSource::updatePropsForObject(gkGameObject* obj)
{
	gkCriticalSection::Lock lock(m_cs);

	if (obj)
	{
		gkVector3 pos = obj->getWorldPosition();
		gkVector3 vel = obj->getLinearVelocity();
		gkQuaternion ori = obj->getWorldOrientation();
		gkVector3 dir = (ori * gkVector3(0, 1, 0)).normalisedCopy();

		m_props.m_height = 2.f * obj->getAabb().getHalfSize().z;
		m_props.m_orientation = ori;
		m_props.m_position = pos;
		m_props.m_direction = dir;
		m_props.m_velocity = vel;

		if (m_playback)
		{
			m_playback->setPosition(m_props.m_position);
			m_playback->setDirection(m_props.m_direction);
			m_playback->setVelocity(m_props.m_velocity);
		}
	}

}



bool gkSource::isPaused(void) const
{
	gkCriticalSection::Lock lock(m_cs);
	return m_playback ? m_playback->isSuspended() : false;
}


gkSoundStream* gkSource::getStream(void)
{
	return m_reference ? m_reference->getStream() : 0;
}



void gkSource::play(void)
{
	bool addPlay = false;
	{
		gkCriticalSection::Lock lock(m_cs);

		if (m_reference && !m_playback)
		{
			gkSoundStream* stream = m_reference->getStream();
			if (stream) addPlay = true;
		}
	}


	if (addPlay)
		gkSoundManager::getSingleton().playSound(this);

}


void gkSource::pause(void)
{
	gkCriticalSection::Lock lock(m_cs);

	// Suspend active buffer


	if (isPlaying()) m_playback->suspend(!isPaused());
}


void gkSource::stop(void)
{
	bool stopPlay = false;
	{
		gkCriticalSection::Lock lock(m_cs);

		if (m_reference && m_playback)
		{
			gkSoundStream* stream = m_reference->getStream();
			if (stream) stopPlay = true;
		}
	}

	if (stopPlay)
		gkSoundManager::getSingleton().stopSound(this);
}



void gkSource::loop(bool v)
{
	gkCriticalSection::Lock lock(m_cs);

	// Loop active buffer

	m_props.m_loop = v;
	if (m_playback)
		m_playback->setLoop(m_props.m_loop);
}
