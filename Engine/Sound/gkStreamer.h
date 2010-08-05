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
#ifndef _gkStreamer_h_
#define _gkStreamer_h_

#include "gkCommon.h"
#include "gkSoundUtil.h"

#include "Thread/gkCriticalSection.h"
#include "Thread/gkThread.h"

class gkBuffer;
class gkSound;
class gkSource;


class gkStreamer : public gkCall
{
	// Background thread for streaming Waveform/Ogg sounds
public:

	gkStreamer(const gkString &name);
	virtual ~gkStreamer();


	void playSound(gkSource *snd);
	void stopSound(gkSource *snd);


	void stopAllSounds(void);


	bool isRunning(void);
	bool isEmpty(void);

	void stop(void);
	void start(void);
	void exit(void);

private:
	friend class gkStreamerTick;

	void run(void);
	void runProtected(void);
	void collect(void);

	void remove(gkBuffer *buf);
	void stopBuffer(gkBuffer *snd);
	void notify(gkBuffer *buf);

	typedef utArray<gkBuffer *> Buffers;

	gkCriticalSection   m_cs;
	const gkString      m_name;
	gkThread           *m_thread;
	bool                m_stop;

	Buffers             m_buffers, m_finished;
	gkSyncObj           m_syncObj;
};


#endif//_gkStreamer_h_
