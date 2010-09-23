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


///Background thread for streaming Waveform/Ogg sounds
class gkStreamer : public gkCall
{
public:

	gkStreamer(const gkString& name);
	virtual ~gkStreamer();


	void playSound(gkSource* snd);
	void stopSound(gkSource* snd);


	void stopAllSounds(void);


	bool isRunning(void);
	bool isEmpty(void);

	void stop(void);
	void start(void);
	void exit(void);

private:
	friend class gkStreamerTick;
	void run(void);
	void runTick(void);
	void beginTick(void);
	void endTick(void);

	typedef utArray<gkBuffer*> Buffers;
	typedef utArray<gkSource*> Sources;


	gkCriticalSection   m_cs;
	const gkString      m_name;
	gkThread*           m_thread;
	bool                m_stop;
	bool                m_finish;

	void freeBuffers(Buffers& bufs);
	void finishBuffers(void);
	void processBuffers(void);

	Buffers m_queueBuffers;
	Buffers m_updateBuffers, m_finishedBuffers;
	Sources m_queueSources;

	bool m_wantsQSync, m_wantsSQSync;
	gkSyncObj m_fsync, m_sqsync, m_qsync;
};


#endif//_gkStreamer_h_
