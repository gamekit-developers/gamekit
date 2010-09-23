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
#include "gkStreamer.h"
#include "gkSound.h"
#include "gkBuffer.h"




gkStreamer::gkStreamer(const gkString& name)
	:   m_name(name),
	    m_thread(0),
	    m_stop(true),
	    m_finish(false),
	    m_wantsQSync(false),
	    m_wantsSQSync(false)
{
}



gkStreamer::~gkStreamer()
{
	exit();
}



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


void gkStreamer::stopAllSounds(void)
{
	gkCriticalSection::Lock lock(m_cs);
	if (!m_updateBuffers.empty())
	{
		m_finish = true;

		// Block until loop is finished.
		m_fsync.wait();

		// Paranoia checks.
		UT_ASSERT(m_updateBuffers.empty());
		UT_ASSERT(m_queueBuffers.empty());
		UT_ASSERT(m_finishedBuffers.empty());
		UT_ASSERT(m_queueSources.empty());
	}
}



void gkStreamer::stop(void)
{
	gkCriticalSection::Lock lock(m_cs);
	if (isRunning())
	{
		m_finish = m_stop = true;

		// Block until loop is finished.
		m_fsync.wait();

		// Paranoia checks.
		UT_ASSERT(m_updateBuffers.empty());
		UT_ASSERT(m_queueBuffers.empty());
		UT_ASSERT(m_finishedBuffers.empty());
		UT_ASSERT(m_queueSources.empty());
	}
}


void gkStreamer::start(void)
{
	gkCriticalSection::Lock lock(m_cs);
	if (!isRunning())
	{
		if (!m_thread)
		{
			m_finish = m_stop = false;
			m_thread = new gkThread(this);
		}
	}
}



bool gkStreamer::isRunning(void)
{
	return !m_stop;
}


bool gkStreamer::isEmpty(void)
{
	gkCriticalSection::Lock lock(m_cs);
	return m_updateBuffers.empty();
}



void gkStreamer::playSound(gkSource* snd)
{
	gkCriticalSection::Lock lock(m_cs);
	if (snd && !snd->isBound())
	{
		m_wantsQSync = true;

		// Assert a queue update is not in progress.
		// Stop all traffic.
		m_qsync.wait();

		// Add to queue.
		m_queueBuffers.push_back(new gkBuffer(snd));
	}
}


void gkStreamer::stopSound(gkSource* snd)
{
	gkCriticalSection::Lock lock(m_cs);
	if (snd && snd->isBound())
	{
		m_wantsSQSync = true;

		// Assert a queue update is not in progress.
		// Stop all traffic.
		m_sqsync.wait();

		// Add to queue.
		m_queueSources.push_back(snd);
	}

}



void gkStreamer::freeBuffers(gkStreamer::Buffers& buffers)
{
	// Remove temporary buffers, from the update list

	UT_ASSERT(!buffers.empty());

	UTsize i, s, p;
	Buffers::Pointer b;
	gkBuffer* tmp;

	i = 0;
	s = buffers.size();
	b = buffers.ptr();

	while (i < s)
	{
		tmp = b[i++];
		p = m_updateBuffers.find(tmp);

		if (p != UT_NPOS)
		{
			m_updateBuffers.erase(p);
			delete tmp;
		}
	}

	buffers.clear(true);
	if (m_updateBuffers.empty())
		m_updateBuffers.clear(true);
}



void gkStreamer::processBuffers(void)
{
	UTsize i, s;
	Buffers::Pointer qb;
	Sources::Pointer qs;
	gkBuffer* tmp;


	if (!m_queueBuffers.empty())
	{
		// Copy buffers

		i  = 0;
		s  = m_queueBuffers.size();
		qb = m_queueBuffers.ptr();

		m_updateBuffers.reserve(s);

		while (i < s)
			m_updateBuffers.push_back(qb[i++]);


		m_queueBuffers.clear(true);
	}

	if (m_wantsQSync)
	{
		m_wantsQSync = false;

		// Notify waiting traffic.
		m_qsync.signal();
	}

	if (!m_queueSources.empty())
	{
		// Free stopped sources.

		i  = 0;
		s  = m_queueSources.size();
		qs = m_queueSources.ptr();

		while (i < s)
		{
			tmp = qs[i++]->getBuffer();

			if (tmp)
			{
				UTsize pos = m_updateBuffers.find(tmp);
				UT_ASSERT(pos != UT_NPOS);

				if (pos != UT_NPOS)
				{
					delete tmp;
					m_updateBuffers.erase(pos);
				}
			}
		}


		// Free buffer cache.
		if (m_updateBuffers.empty())
			m_updateBuffers.clear(true);

		m_queueSources.clear(true);
	}

	if (m_wantsSQSync)
	{
		m_wantsSQSync = false;

		// Notify waiting traffic.
		m_sqsync.signal();
	}
}




void gkStreamer::finishBuffers(void)
{
	// Remove all buffers from the system.

	if (!m_updateBuffers.empty())
	{
		UTsize i, s;
		Buffers::Pointer b;
		gkBuffer* tmp;


		i = 0;
		s = m_updateBuffers.size();
		b = m_updateBuffers.ptr();

		while (i < s)
		{
			tmp = b[i++];

			// Make sure queue buffers, do not contain this.
			UT_ASSERT(m_queueBuffers.find(tmp) == UT_NPOS);

			delete tmp;
		}

		m_updateBuffers.clear(true);
	}

	// Free buffer cache.

	if (!m_finishedBuffers.empty())
		m_finishedBuffers.clear(true);

	if (!m_queueSources.empty())
		m_queueSources.clear(true);

	if (!m_queueBuffers.empty())
		m_queueBuffers.clear(true);
}


void gkStreamer::run(void)
{
	/// Main sound workload.
	/// \note This is designed to be a long running background thread.

	while (isRunning())
	{
		// catch any exceptions
		try
		{
			processBuffers();

			if (!m_updateBuffers.empty())
			{
				// Process update buffers.
				UTsize i, s;
				Buffers::Pointer b;


				i = 0;
				s = m_updateBuffers.size();
				b = m_updateBuffers.ptr();

				while (i < s)
				{
					gkBuffer* buf = b[i++];

					buf->stream();

					// Local remove
					if (!buf->isValid() || buf->isDone())
						m_finishedBuffers.push_back(buf);
				}


				if (!m_finishedBuffers.empty())
					freeBuffers(m_finishedBuffers);
			}

			if (m_finish || m_stop)
			{
				m_finish = false;

				// Clear all buffers.
				finishBuffers();

				if (m_stop)
				{
					// exit hook
					break;
				}
				else
				{
					// continue
					m_fsync.signal();
				}
			}
		}

		catch (...)
		{
			printf("%s: unknown error!\n", m_name.c_str());
		}
	}


	// notify we have stopped.
	m_fsync.signal();
}
