/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2012 Thomas Trocha

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
#include "Sound/gkSoundManager.h"

gkSoundProcess::gkSoundProcess(const gkString& soundName)
	:	m_soundName(soundName),  m_source(0), m_sound(0), m_soundPlayed(false)
{}

gkSoundProcess::~gkSoundProcess()
{
	// stuff is deleted by soundmanager
}


bool gkSoundProcess::isFinished()
{
	return m_soundPlayed && !m_source->isPlaying();
}

void gkSoundProcess::init()
{
	if (!m_source) {
		m_sound = static_cast<gkSound*>(gkSoundManager::getSingleton().getByName(m_soundName));
		if (m_sound)
		{
			m_source = m_sound->createSource();
		}
	}

	m_soundPlayed = false;

}
void gkSoundProcess::update(gkScalar delta)
{
	if (m_source && !m_soundPlayed)
	{
		m_source->play();
		m_soundPlayed = true;
	}
}


