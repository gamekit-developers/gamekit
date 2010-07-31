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
#ifndef _gkBuffer_h_
#define _gkBuffer_h_

#include "gkCommon.h"
#include "gkSoundUtil.h"
#include "gkSound.h"
#include "Thread/gkCriticalSection.h"


class gkBuffer
{
	// Main playback buffer.

public:

	gkBuffer(gkSource *obj);
	virtual ~gkBuffer();

	bool stream(void);

	void suspend(bool v);
	void setLoop(bool v);

	bool isSuspended(void)  {return m_suspend;}
	bool isDone(void)       {return m_exit;}
	bool isValid(void)      {return m_ok;}
	bool isLooped(void)     {return m_loop;}
	void exit(void)         {m_exit = true;}

	void setPosition(const gkVector3 &v);
	void setDirection(const gkVector3 &v);
	void setVelocity(const gkVector3 &v);

	void setProperties(const gkSoundProperties &props);

	void queue(bool play=false);

private:
	bool initialize(void);
	void finalize(void);
	void reset(void);


	// stream reading
	const char *read(UTsize len, UTsize &br);
	void seek(void);

	gkCriticalSection   m_cs;
	gkSource            *m_sound;
	gkSoundStream       *m_stream;
	ALuint              m_buffer[GK_SND_SAMPLES];
	ALuint              m_source;
	bool                m_loop, m_ok, m_exit, m_initial;
	bool                m_suspend;
	gkSoundProperties   m_props;
	UTsize              m_pos;
	bool                m_eos;
	int                 m_fmt, m_smp, m_bps;
};


#endif//_gkBuffer_h_
