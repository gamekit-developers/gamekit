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
#include "gkCommon.h"
#include "gkCamera.h"
#include "gkScene.h"
#include "gkSoundManager.h"
#include "gkSound.h"
#include "gkEngine.h"
#include "gkUserDefs.h"
#include "gkDebugger.h"
#include "gkLogger.h"


#define gkSndCtxValid() (m_device != 0 && m_context != 0 && !m_disabled)




gkSoundManager::gkSoundManager()
	:   gkResourceManager("SoundManager", "Sound"),
		m_stream(0),
	    m_valid(false),
	    m_device(0),
	    m_context(0),
	    m_disabled(false)
{
	m_disabled = gkEngine::getSingleton().getUserDefs().disableSound;

	if (!m_disabled)
	{
		m_disabled = !alOpenLibrary();

		if (!m_disabled)
			initialize();
	}
}




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

	alCloseLibrary();
}


gkResource* gkSoundManager::createImpl(const gkResourceName& name, const gkResourceHandle& handle)
{
	// Always test the return value, no guarantee that this manager can create playback.
	if (!gkSndCtxValid())
		return 0;

	return new gkSound(this, name, handle);
}


void gkSoundManager::initialize(void)
{
	// Load start up params


	UT_ASSERT(!m_disabled && "Sound system disabled!");


	ALCcontext* ctx = alcGetCurrentContext();
	if (ctx)
		gkLogMessage("Warning: ALC context already present!");

	// Don't get/clear the error here, because alGetError requires a valid context.
	// alGetError();

	/// \todo, need to be able to specify a device
	m_device = alcOpenDevice(NULL);
	if (!m_device)
	{
		alErrorThrow("gkSoundManager::initialize alcOpenDevice");
		return;
	}


	m_context = alcCreateContext(m_device, 0);
	if (!m_context)
	{
		alcCloseDevice(m_device);

		m_device = 0;
		m_context = 0;
		return;
	}

	alcMakeContextCurrent(m_context);
	m_valid = !alErrorThrow("gkSoundManager::initialize alcMakeContextCurrent");
	if (!m_valid)
	{
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);

		m_context = 0;
		m_device = 0;

		return;
	}


	// Apply initial parameters.
	alDistanceModel(AL_NONE);
	alSpeedOfSound(343.3f);
	alDopplerFactor(1.f);


	// Create the playback stream.
	m_stream = new gkStreamer("Sound Manager Stream");
}



bool gkSoundManager::isValidContext(void)
{
	return m_valid && gkSndCtxValid();
}



void gkSoundManager::notifySourceCreated(gkSource* src)
{
	if (!gkSndCtxValid())
		return;


	// Add this source to the play list.
	GK_ASSERT(m_playingSources.find(src) == UT_NPOS);

	m_playingSources.push_back(src);
}



void gkSoundManager::notifySourceDestroyed(gkSource* src)
{
	if (!gkSndCtxValid())
		return;


	// This orphans the source from gkSound,
	// The manager is now the owner. This source will be deleted
	// when the sound has stopped playing.


	if (src)
	{
		if (src->isPlaying())
		{
			// Clear any loop state.
			src->loop(false);


			if (m_gcSources.find(src) == UT_NPOS)
				m_gcSources.push_back(src);
			else
			{
				GK_ASSERT(0 && "Duplicate orphaned source");
			}

		}
		else
		{
			// Just destroy
			UTsize fnd = m_playingSources.find(src);

			GK_ASSERT(!src->isBound() && "Attempting to delete a bound source!");
			if (fnd != UT_NPOS)
			{

				// paranoia
				UTsize gcFnd = m_gcSources.find(src);
				if (gcFnd != UT_NPOS)
					m_gcSources.erase(gcFnd);

				m_playingSources.erase(fnd);
			}

			delete src;
		}
	}
}



void gkSoundManager::stopAllSounds(void)
{
	if (!gkSndCtxValid())
		return;

	if (!m_stream->isEmpty())
		m_stream->stopAllSounds();

	collectGarbage();


	if (!m_playingSources.empty())
	{
		// Free any other sources.
		m_playingSources.clear(true);
	}


	if (!m_gcSources.empty())
	{
		// stop any pending gc
		m_gcSources.clear(true);
	}
}



void gkSoundManager::updateSoundProperties(void)
{
	if (!gkSndCtxValid())
		return;

	// Apply global properties.

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




void gkSoundManager::update(gkScene* scene)
{
	if (!gkSndCtxValid())
		return;

	collectGarbage();


	gkCamera* obj = scene->getMainCamera();

	const gkVector3 pos     = obj->getWorldPosition();
	const gkVector3 vel     = obj->getLinearVelocity();
	const gkQuaternion rot  = obj->getWorldOrientation();


	gkVector3 at = (rot * gkVector3(0, 0, -1));
	gkVector3 up = (rot * gkVector3(0, 1, 0));
	ALfloat ori[6] = {at.x, at.y, at.z, up.x, up.y, up.z};

	alListenerfv(AL_POSITION,       pos.ptr());
	alListenerfv(AL_ORIENTATION,    ori);
	alListenerfv(AL_VELOCITY,       vel.ptr());


	// Apply debug information.
	if (gkEngine::getSingleton().getUserDefs().debugSounds && !m_playingSources.empty())
	{
		UTsize i, s;
		Sources::Pointer p;

		i = 0;
		s = m_playingSources.size();
		p = m_playingSources.ptr();

		gkDebugger* debug = scene->getDebugger();

		while (i < s)
		{
			gkSource* src = p[i++];

			// Draw 3D only.

			if (src->isBound() && src->getProperties().m_3dSound)
				debug->draw3dSound(src->getProperties());
		}
	}
}



void gkSoundManager::collectGarbage(void)
{
	if (!gkSndCtxValid())
		return;

	// Test for finished sounds,
	// and delete when it has stopped playing.


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
			gkSource* src = p[i++];

			if (!src->isPlaying())
			{
				del.push_back(src);

				// erase from playing
				m_playingSources.erase(src);
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
			gkSource* src = p[i++];
			GK_ASSERT(!src->isBound());

			// Free

			m_gcSources.erase(src);
			m_playingSources.erase(src);


			delete src;
		}

		del.clear();
	}

	if (m_gcSources.empty())
		m_gcSources.clear(true);


	if (m_playingSources.empty())
		m_playingSources.clear(true);
}



void gkSoundManager::removePlayback(gkSound* sndToDelete)
{
	if (!gkSndCtxValid())
		return;

	// Force the playback to stop (the parent sound is about to become invalid)


	if (!m_playingSources.empty())
	{
		UTsize i, s, f;
		Sources::Pointer p;

		i = 0;
		s = m_playingSources.size();
		p = m_playingSources.ptr();

		Sources freeNow;
		freeNow.reserve(s);


		while (i < s)
		{
			gkSource* src = p[i++];

			if (src->getCreator() == sndToDelete)
			{
				src->stop();

				GK_ASSERT(!src->isBound());

				// Remove from collection.

				f = m_gcSources.find(src);
				if (f != UT_NPOS)
					m_gcSources.erase(f);


				freeNow.push_back(src);
			}
		}

		// Now destroy them all

		i = 0;
		s = freeNow.size();
		p = freeNow.ptr();

		while (i < s)
		{
			gkSource* src = p[i++];

			f = m_playingSources.find(src);


			UT_ASSERT(f != UT_NPOS);

			m_playingSources.erase(f);
			delete src;
		}


		if (m_playingSources.empty())
			m_playingSources.clear(true);
	}
}



bool gkSoundManager::hasSounds(void)
{
	return !m_stream->isEmpty();
}


void gkSoundManager::playSound(gkSource* snd)
{
	if (!gkSndCtxValid())
		return;

	if (m_valid)
	{
		if (!m_stream->isRunning())
		{
			// Start on demand.
			m_stream->start();
		}


		// Add it to the playlist.
		m_stream->playSound(snd);
	}
}


void gkSoundManager::stopSound(gkSource* snd)
{
	if (!gkSndCtxValid())
		return;

	if (m_valid)
	{
		// Remove it from the playlist.
		m_stream->stopSound(snd);
	}
}


void gkSoundManager::notifyResourceCreatedImpl(gkResource* res)
{

}

void gkSoundManager::notifyResourceDestroyedImpl(gkResource* res)
{
	if (!gkSndCtxValid())
		return;

	gkSound* ob = (gkSound*)res;
	// Force stop.
	removePlayback(ob);
}


void gkSoundManager::notifyDestroyAllImpl(void)
{
	if (!gkSndCtxValid())
		return;

	stopAllSounds();

	// Destroy thread.
	if (m_stream) m_stream->exit();
}


UT_IMPLEMENT_SINGLETON(gkSoundManager);
