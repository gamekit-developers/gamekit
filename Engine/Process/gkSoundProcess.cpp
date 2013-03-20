/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Thomas Trocha

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


#include "Process/gkSoundProcess.h"
#include "gkLogger.h"

#ifdef OGREKIT_OPENAL_SOUND
#include "Sound/gkSoundManager.h"
#endif

gkSoundProcess::gkSoundProcess(const gkString& soundName)
	:	m_soundName(soundName), m_soundPlayed(false)
#ifdef OGREKIT_OPENAL_SOUND
	,  m_source(0), m_sound(0)
#endif
{}

gkSoundProcess::~gkSoundProcess()
{
	// stuff is deleted by soundmanager
}


bool gkSoundProcess::isFinished()
{
#ifdef OGREKIT_OPENAL_SOUND
	return m_soundPlayed && !m_source->isPlaying();
#else
	return true;
#endif
}

void gkSoundProcess::init()
{
#ifdef OGREKIT_OPENAL_SOUND
	if (!m_source) {
		m_sound = static_cast<gkSound*>(gkSoundManager::getSingleton().getByName(m_soundName));
		if (m_sound)
		{
			m_source = m_sound->createSource();
		}
	}

	m_soundPlayed = false;
#endif
}
void gkSoundProcess::update(gkScalar delta)
{
#ifdef OGREKIT_OPENAL_SOUND
	if (m_sound && m_source && !m_soundPlayed)
	{
		m_source->play();
		m_soundPlayed = true;
	}
#endif
}

bool gkSoundProcess::hasValidSound()
{
#ifdef OGREKIT_OPENAL_SOUND
	return m_sound!=0;
#else
	return false;
#endif
}
