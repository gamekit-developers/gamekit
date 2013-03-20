/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2013 Charlie C.

    Contributor(s): Thomas Trocha(dertom)
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
#include "gkSoundActuator.h"

#ifdef OGREKIT_OPENAL_SOUND
#include "gkLogger.h"
#include "gkSoundManager.h"
#include "gkSound.h"
#endif

gkSoundActuator::gkSoundActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name),
	    m_mode(SA_PLAY_STOP),
	    m_sndInit(false),
	    m_sndRef(""),
	    m_didPlay(false)
#ifdef OGREKIT_OPENAL_SOUND
	    ,
	    m_sound(0),
	    m_player(0)
#endif
{
}



gkSoundActuator::~gkSoundActuator()
{
#ifdef OGREKIT_OPENAL_SOUND
	notifyLinkDestroyed();
#endif
}



gkLogicBrick* gkSoundActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkSoundActuator* act = new gkSoundActuator(*this);
	act->cloneImpl(link, dest);
#ifdef OGREKIT_OPENAL_SOUND
	act->m_player = 0;
	act->m_sound = 0;
	act->m_sndInit = false;
#endif
	return act;
}

#ifdef OGREKIT_OPENAL_SOUND


void gkSoundActuator::notifyLinkDestroyed(void)
{
	if (m_player && m_sound)
		m_sound->destroySource(m_player);
	m_player = 0;
	m_sound = 0;
	m_sndInit = false;

}



void gkSoundActuator::notifyActiveStatus(void)
{
	// give it a chance to shut off
	if (!m_isActive && m_player)
	{
		m_player->loop(false);

		if (m_mode == SA_PLAY_STOP || m_mode == SA_LOOP_STOP)
			m_player->stop();
	}
}

#endif

void gkSoundActuator::execute(void)
{

#ifdef OGREKIT_OPENAL_SOUND
	if (!m_sndInit)
	{
		m_sndInit = true;
		m_sound = gkSoundManager::getSingleton().getByName<gkSound>(m_sndRef);

		if (m_sound)
		{
			m_player = m_sound->createSource();
			if (m_player)
				m_player->setProperties(m_props);
		}
	}


	if (m_player != 0)
	{
		int tmode = m_mode;

		if (m_mode == SA_LOOP_END)
		{
			if (!m_player->isLooped())
				m_player->loop(true);

			tmode = SA_PLAY_END;
		}
		else if (m_mode == SA_LOOP_STOP)
		{
			if (!m_player->isLooped())
				m_player->loop(true);

			tmode = SA_PLAY_STOP;
		}
		else if (m_player->isLooped())
			m_player->loop(false);


		if (tmode == SA_PLAY_STOP)
		{
			// play while there is a positive pulse
			if (isPulseOff())
			{
				if (m_player->isPlaying())
				{
					m_player->stop();
					m_didPlay = false;
				}
			}
			else
			{
				if (!m_didPlay || (!m_player->isPlaying() && m_player->isLooped()) )
				{
					m_player->play();
					m_didPlay = true;
				}
			}
		}
		else if (tmode == SA_PLAY_END)
		{
			// play until finished
			if (isPulseOff())
			{
				// reset the flag that the sound started to be
				// ready for the next positive pulse
				m_didPlay = false;
				return;
			}
			else
			{
				if (!m_didPlay || (!m_player->isPlaying() && m_player->isLooped()) )
				{
					// TODO this should be done automatically in the streamer/buffer
					m_player->stop();
					m_player->play();
					m_didPlay = true;
				}
			}
		}


		if (m_props.m_3dSound)
			m_player->updatePropsForObject(m_object);
	}
#endif
}

void gkSoundActuator::updateProperties()
{
#ifdef OGREKIT_OPENAL_SOUND
	if (m_player)
	{
		m_player->getProperties().m_pitch=m_props.m_pitch;
		m_player->getProperties().m_volume=m_props.m_volume;
		m_player->requestUpdateProperties();
	}
#endif
}
