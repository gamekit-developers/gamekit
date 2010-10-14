/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

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
#ifndef _gkSoundActuator_h_
#define _gkSoundActuator_h_
#include "gkCommon.h"


#include "LogicBricks/gkLogicActuator.h"

#ifdef OGREKIT_OPENAL_SOUND
#include "Sound/gkSound.h"
#endif


class gkSoundActuator : public gkLogicActuator
{
public:

	enum Mode
	{
		SA_PLAY_STOP,
		SA_PLAY_END,
		SA_LOOP_STOP,
		SA_LOOP_END,
		SA_LOOP_PPONG,
		SA_LOOP_PPONG_STOP,
	};


private:
	int                 m_mode;         // playback mode
	bool                m_sndInit;      // state chech
	gkString            m_sndRef;       // reference to the sould block name

#ifdef OGREKIT_OPENAL_SOUND

	gkSound*             m_sound;       // the sound to play
	gkSource*            m_player;      // running sound
	gkSoundProperties    m_props;       // properties to attach to the sound stream

	void notifyActiveStatus(void);
	void notifyLinkDestroyed(void);
#endif
public:

	gkSoundActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkSoundActuator();

	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);
#ifdef OGREKIT_OPENAL_SOUND
	GK_INLINE gkSoundProperties&    getProperties(void)                {return m_props;}
#endif

	GK_INLINE void                  setMode(int v)                      {m_mode = v;}
	GK_INLINE void                  setSoundFile(const gkString& v)     {m_sndRef = v;}
	GK_INLINE int                   getMode(void)                       {return m_mode;}
	GK_INLINE const gkString&       getSoundFile(void)                  {return m_sndRef;}

	void execute(void);
};
#endif//_gkSoundActuator_h_
