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
#define GK_SND_DEBUG 1

#include "gkCommon.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkSoundManager.h"
#include "gkSound.h"
#include "gkEngine.h"
#include "gkUserDefs.h"


#ifdef GK_SND_DEBUG
#include "gkDebugger.h"
#endif


#define gkSndCtxValid() (m_device != 0 && m_context != 0)


// ----------------------------------------------------------------------------
gkSoundManager::gkSoundManager()
	:   m_stream(0), m_valid(false)
{
	m_device = alcOpenDevice(NULL);
	if (m_device != 0)
	{
		m_context = alcCreateContext(m_device, 0);
		if (m_context)
		{
			alcMakeContextCurrent(m_context);
			m_valid = !alErrorCheck();

			if (m_valid)
			{
				alDistanceModel(AL_NONE);
				alSpeedOfSound(343.3f);
				alDopplerFactor(1.f);

				m_stream = new gkStreamer("Sound Manager Stream");
			}

		}
	}
}



// ----------------------------------------------------------------------------
gkSoundManager::~gkSoundManager()
{
	destroyAll();

	delete m_stream;

	if (gkSndCtxValid())
	{
		alcMakeContextCurrent(0);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}
}

// ----------------------------------------------------------------------------
bool gkSoundManager::isValidContext(void)
{
	return m_valid && gkSndCtxValid();
}


// ----------------------------------------------------------------------------
void gkSoundManager::notifySourceCreated(gkSource *src)
{
	GK_ASSERT(m_sources.find(src) == UT_NPOS);
	m_sources.push_back(src);
}


// ----------------------------------------------------------------------------
void gkSoundManager::notifySourceDestroyed(gkSource *src)
{
	if (src)
	{
		if (src->isPlaying())
		{
			src->loop(false);

			if (m_gcSources.find(src) == UT_NPOS)
				m_gcSources.push_back(src);
			else
				GK_ASSERT(0);
		}
		else
		{
			if (m_gcSources.find(src) == UT_NPOS)
				m_gcSources.push_back(src);

			collectGarbage();
		}
	}
}


// ----------------------------------------------------------------------------
void gkSoundManager::stopAllSounds(void)
{
	if (!gkSndCtxValid())
		return;

	m_stream->stopAllSounds();
	collectGarbage();
}


// ----------------------------------------------------------------------------
void gkSoundManager::updateSoundProperties(void)
{
	if (!gkSndCtxValid())
		return;

	switch (m_props.m_distModel)
	{
	case gkSoundSceneProperties::DM_NONE:            {alDistanceModel(AL_NONE);                      break;}
	case gkSoundSceneProperties::DM_INVERSE:         {alDistanceModel(AL_INVERSE_DISTANCE);          break;}
	case gkSoundSceneProperties::DM_LINEAR:          {alDistanceModel(AL_LINEAR_DISTANCE);           break;}
	case gkSoundSceneProperties::DM_EXPONENT:        {alDistanceModel(AL_EXPONENT_DISTANCE);         break;}
	case gkSoundSceneProperties::DM_INVERSE_CLAMP:   {alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);  break;}
	case gkSoundSceneProperties::DM_LINEAR_CLAMP:    {alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);   break;}
	case gkSoundSceneProperties::DM_EXPONENT_CLAMP:  {alDistanceModel(AL_EXPONENT_DISTANCE_CLAMPED); break;}
	}

	alSpeedOfSound(m_props.m_sndSpeed);
	alDopplerFactor(m_props.m_dopplerFactor);
}



// ----------------------------------------------------------------------------
void gkSoundManager::update(gkScene *scene)
{
	if (!gkSndCtxValid())
		return;

	collectGarbage();


	gkCamera *obj = scene->getMainCamera();

	const gkVector3 pos     = obj->getWorldPosition();
	const gkVector3 vel     = obj->getLinearVelocity();
	const gkQuaternion rot  = obj->getWorldOrientation();


	gkVector3 at = (rot * gkVector3(0, 0, -1));
	gkVector3 up = (rot * gkVector3(0, 1, 0));
	ALfloat ori[6] = {at.x, at.y, at.z, up.x, up.y, up.z};

	alListenerfv(AL_POSITION,       pos.ptr());
	alListenerfv(AL_ORIENTATION,    ori);
	alListenerfv(AL_VELOCITY,       vel.ptr());

#ifdef GK_SND_DEBUG

	if (gkEngine::getSingleton().getUserDefs().debugSounds && !m_sources.empty())
	{
		UTsize i, s;
		Sources::Pointer p;

		i = 0;
		s = m_sources.size();
		p = m_sources.ptr();

		gkDebugger *debug = scene->getDebugger();

		while (i < s)
		{
			gkSource *src = p[i++];
			if (src->isBound() && src->getProperties().m_3dSound)
				debug->draw3dSound(src->getProperties());
		}
	}
#endif
}


// ----------------------------------------------------------------------------
void gkSoundManager::collectGarbage(void)
{
	if (!gkSndCtxValid())
		return;

	UTsize i, s;
	Sources::Pointer p;
	Sources del;

	if (!m_gcSources.empty())
	{
		i = 0;
		s = m_gcSources.size();
		p = m_gcSources.ptr();
		while (i < s)
		{
			gkSource *src = p[i++];
			if (!src->isPlaying())
			{
				del.push_back(src);
				m_sources.erase(src);
			}
		}
	}

	if (!del.empty())
	{
		i = 0;
		s = del.size();
		p = del.ptr();

		while (i < s)
		{
			gkSource *src = p[i++];
			GK_ASSERT(!src->isBound());
			m_gcSources.erase(src);
			m_sources.erase(src);
			delete src;
		}

		del.clear();
	}
}


// ----------------------------------------------------------------------------
void gkSoundManager::removePlayback(gkSound *sndToDelete)
{
	if (!m_sources.empty())
	{
		UTsize i, s;
		Sources::Pointer p;

		i = 0;
		s = m_sources.size();
		p = m_sources.ptr();

		while (i < s)
		{
			gkSource *src = p[i++];

			if (src->getCreator() == sndToDelete)
			{
				src->stop();
				m_gcSources.push_back(src);
			}
		}

		collectGarbage();
	}
}


// ----------------------------------------------------------------------------
bool gkSoundManager::hasSounds(void)
{
	return !m_stream->isEmpty();
}

// ----------------------------------------------------------------------------
void gkSoundManager::playSound(gkSource *snd)
{
	if (!gkSndCtxValid())
		return;

	if (m_valid)
	{
		if (!m_stream->isRunning())
			m_stream->start();
		m_stream->playSound(snd);
	}
}

// ----------------------------------------------------------------------------
void gkSoundManager::stopSound(gkSource *snd)
{
	if (m_valid)
	{
		m_stream->stopSound(snd);
	}
}


// ----------------------------------------------------------------------------
gkSound *gkSoundManager::getSound(const gkHashedString &name)
{
	UTsize pos;
	if ((pos = m_objects.find(name)) == GK_NPOS)
		return 0;
	return m_objects.at(pos);
}


// ----------------------------------------------------------------------------
gkSound *gkSoundManager::createSound(const gkHashedString &name)
{
	if (!gkSndCtxValid())
		return 0;

		
	UTsize pos;
	if ((pos = m_objects.find(name)) != GK_NPOS)
		return 0;

	gkSound *ob = new gkSound(name.str());
	m_objects.insert(name, ob);
	return ob;
}


// ----------------------------------------------------------------------------
void gkSoundManager::destroy(const gkHashedString &name)
{
	UTsize pos;
	if ((pos = m_objects.find(name)) != GK_NPOS)
	{
		gkSound *ob = m_objects.at(pos);
		removePlayback(ob);

		m_objects.remove(name);
		delete ob;
	}
}


// ----------------------------------------------------------------------------
void gkSoundManager::destroy(gkSound *ob)
{
	GK_ASSERT(ob);
	destroy(ob->getName());
}


// ----------------------------------------------------------------------------
void gkSoundManager::destroyAll(void)
{
	stopAllSounds();
	m_stream->exit();


	utHashTableIterator<ObjectMap> iter(m_objects);
	while (iter.hasMoreElements())
	{
		gkSound *ob = iter.peekNextValue();
		removePlayback(ob);

		delete ob;
		iter.next();
	}

	m_objects.clear();
}



// ----------------------------------------------------------------------------
bool gkSoundManager::hasSound(const gkHashedString &name)
{
	return m_objects.find(name) != GK_NPOS;
}


GK_IMPLEMENT_SINGLETON(gkSoundManager);
