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
#include "gkEngine.h"
#include "gkScene.h"
#include "gkCamera.h"
#include "gkStreamer.h"
#include "gkSound.h"
#include "gkBuffer.h"
#include "gkTickState.h"


class gkStreamerTick : public gkTickState
{
	// Frame tick state for sound playback
private:

	gkStreamer &m_stream;
public:

	gkStreamerTick(gkStreamer &streamer);
	virtual ~gkStreamerTick() {}

	void tickImpl(gkScalar delta);
	void beginTickImpl(void);
	void endTickImpl(void);

};

// ----------------------------------------------------------------------------
gkStreamer::gkStreamer(const gkString &name)
	:   m_name(name),
	    m_thread(0),
	    m_stop(true)
{
}


// ----------------------------------------------------------------------------
gkStreamer::~gkStreamer()
{
	exit();
}


// ----------------------------------------------------------------------------
void gkStreamer::exit(void)
{
	// exit thread
	if (m_thread)
	{
		stop();
		m_thread->join();

		delete m_thread;
		m_thread = 0;
	}
}

// ----------------------------------------------------------------------------
void gkStreamer::stopAllSounds(void)
{
	while (!m_buffers.empty())
		stopBuffer(m_buffers.back());
}



// ----------------------------------------------------------------------------
void gkStreamer::stop(void)
{
	gkCriticalSection::Lock lock(m_cs);

	m_stop = true;
	m_syncObj.signal();
	m_syncObj.wait();
}

// ----------------------------------------------------------------------------
void gkStreamer::start(void)
{
	gkCriticalSection::Lock lock(m_cs);

	if (!m_thread)
	{
		m_stop = false;
		m_thread = new gkThread(this);
		m_syncObj.signal();
	}
}


// ----------------------------------------------------------------------------
bool gkStreamer::isRunning(void)
{
	return !m_stop;
}

// ----------------------------------------------------------------------------
bool gkStreamer::isEmpty(void)
{
	gkCriticalSection::Lock lock(m_cs);

	return m_buffers.empty();
}

// ----------------------------------------------------------------------------
void gkStreamer::playSound(gkSource *snd)
{
	gkCriticalSection::Lock lock(m_cs);

	// add to queue
	m_buffers.push_back(new gkBuffer(snd));
	m_syncObj.signal();
}


// ----------------------------------------------------------------------------
void gkStreamer::stopSound(gkSource *snd)
{
	if (snd && snd->isBound())
		stopBuffer(snd->_getBuffer());
}

// ----------------------------------------------------------------------------
void gkStreamer::stopBuffer(gkBuffer *buf)
{
	if (buf)
		notify(buf);
}


// ----------------------------------------------------------------------------
void gkStreamer::notify(gkBuffer *buf)
{
	// wait for a signal saying this
	// buffer has exited

	gkCriticalSection::Lock lock(m_cs);

	buf->exit();
	m_syncObj.wait();

}

// ----------------------------------------------------------------------------
void gkStreamer::remove(gkBuffer *buf)
{
	UTsize pos;
	if ((pos = m_finished.find(buf)) != UT_NPOS)
		m_finished.erase(pos);

	if ((pos = m_buffers.find(buf)) != UT_NPOS)
	{
		m_buffers.erase(pos);
		delete buf;
	}

	if (m_buffers.empty())
		m_buffers.clear(true);
}

// ----------------------------------------------------------------------------
void gkStreamer::run(void)
{
	gkStreamerTick stream(*this);


	while (isRunning())
	{
		// catch any exceptions
		try
		{
			stream.tick();
		}
		catch (...)
		{
			printf("%s: unknown error!\n", m_name.c_str());
		}
	}

	m_syncObj.signal();
}


// ----------------------------------------------------------------------------
void gkStreamer::runProtected(void)
{
	// main sound workload
	UTsize i, s;
	Buffers::Pointer b;

	i = 0;
	s = m_buffers.size();
	b = m_buffers.ptr();

	while (i < s)
	{
		gkBuffer *buf = b[i++];

		// stream contents to OpenAL
		if (buf->isValid())
			buf->_stream();

		// notify were done
		if (buf->isDone() || !buf->isValid())
			m_finished.push_back(buf);
	}

	if (!m_finished.empty())
	{
		while (!m_finished.empty())
			remove(m_finished.back());

		m_finished.clear(true);

		// wake up wating sync
		m_syncObj.signal();
	}
}


// ----------------------------------------------------------------------------
void gkStreamer::collect(void)
{
	if (!m_finished.empty())
	{
		while (!m_finished.empty())
			remove(m_finished.back());

		m_finished.clear(true);

		// wake up wating sync
		m_syncObj.signal();
	}
}

// ----------------------------------------------------------------------------
gkStreamerTick::gkStreamerTick(gkStreamer &streamer)
	:   gkTickState(gkEngine::getTickRate()),
	    m_stream(streamer)
{
}

// ----------------------------------------------------------------------------
void gkStreamerTick::tickImpl(gkScalar delta)
{
	m_stream.runProtected();
}


// ----------------------------------------------------------------------------
void gkStreamerTick::beginTickImpl(void)
{
	m_stream.collect();
}

// ----------------------------------------------------------------------------
void gkStreamerTick::endTickImpl(void)
{
	m_stream.collect();
}

