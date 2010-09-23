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
#include "gkBuffer.h"
#include "gkSoundStream.h"
#include "gkSound.h"
#include "gkSoundManager.h"



gkBuffer::gkBuffer(gkSource* obj)
	:   m_sound(obj),
	    m_stream(obj->getStream()),
	    m_loop(false),
	    m_ok(false),
	    m_exit(false),
	    m_initial(true),
	    m_isInit(false),
	    m_suspend(false),
	    m_doSuspend(false),
	    m_do3D(false),
	    m_pos(0),
	    m_eos(false)
{
	if (m_stream != 0)
	{
		m_fmt = m_stream->getFormat();
		if (m_fmt != -1)
		{
			m_smp = m_stream->getSampleRate();
			m_bps = m_stream->getBitsPerSecond();
			obj->bind(this);
		}
	}
}



gkBuffer::~gkBuffer()
{
	finalize();
}




void gkBuffer::doSuspend(void)
{
	if (m_doSuspend)
	{
		m_doSuspend = false;

		if (alIsPlaying(m_source) && m_suspend)
			alSourceStop(m_source);
		else if (!m_suspend)
			alSourcePlay(m_source);

		m_suspend = !m_suspend;
		m_ok = !alErrorThrow("suspend buffers");
	}
}



void gkBuffer::do3D(void)
{
	if (m_do3D)
	{
		m_do3D = false;

		alSourcefv(m_source, AL_VELOCITY, m_props.m_velocity.ptr());
		alSourcefv(m_source, AL_POSITION, m_props.m_position.ptr());
		alSourcefv(m_source, AL_DIRECTION, m_props.m_direction.ptr());
	}
}



void gkBuffer::doProperties(void)
{
	alSourcei(m_source,  AL_LOOPING, AL_FALSE);
	if (m_props.m_3dSound)
	{
		alSourcei(m_source,     AL_SOURCE_RELATIVE,     AL_FALSE);
		alSourcef(m_source,     AL_MIN_GAIN,            gkClampf(m_props.m_gainClamp.x, 0.f, 1.f));
		alSourcef(m_source,     AL_MAX_GAIN,            gkClampf(m_props.m_gainClamp.y, 0.f, 1.f));
		alSourcef(m_source,     AL_REFERENCE_DISTANCE,  m_props.m_refDistance);
		alSourcef(m_source,     AL_MAX_DISTANCE,        m_props.m_maxDistance);
		alSourcef(m_source,     AL_ROLLOFF_FACTOR,      m_props.m_rolloff);
		alSourcef(m_source,     AL_CONE_INNER_ANGLE,    m_props.m_coneAngle.x);
		alSourcef(m_source,     AL_CONE_OUTER_ANGLE,    m_props.m_coneAngle.y);
		alSourcef(m_source,     AL_CONE_OUTER_GAIN,     m_props.m_coneOuterGain);

		alSourcefv(m_source,    AL_POSITION,            m_props.m_position.ptr());
		alSourcefv(m_source,    AL_DIRECTION,           m_props.m_direction.ptr());
		alSourcefv(m_source,    AL_VELOCITY,            m_props.m_velocity.ptr());
	}
	else
		alSourcei(m_source,  AL_SOURCE_RELATIVE, AL_TRUE);

	if (m_props.m_pitch > 0)
		alSourcef(m_source, AL_PITCH, m_props.m_pitch);

	alSourcef(m_source, AL_GAIN, m_props.m_volume);

	m_ok = !alErrorThrow("Sound properties");

}


void gkBuffer::suspend(bool v)
{
	if (!m_ok) return;

	m_doSuspend = m_suspend != v;
}


void gkBuffer::setLoop(bool v)
{
	if (!m_ok) return;

	m_loop = v;
}



void gkBuffer::setPosition(const gkVector3& v)
{
	if (!m_ok) return;

	m_props.m_position = v;
	m_do3D = true;
}


void gkBuffer::setDirection(const gkVector3& v)
{
	if (!m_ok) return;

	m_props.m_direction = v;
	m_do3D = true;
}



void gkBuffer::setVelocity(const gkVector3& v)
{
	if (!m_ok) return;

	m_props.m_velocity = v;
	m_do3D = true;
}




bool gkBuffer::initialize(void)
{
	if (m_isInit)
		return false;

	alGetError();

	alGenBuffers(GK_SND_SAMPLES, m_buffer);
	if (alErrorThrow("opening buffers"))
	{
		m_ok = false;
		return false;
	}

	alGenSources(1, &m_source);
	if (alErrorThrow("opening source"))
	{
		m_ok = false;
		return false;
	}
	m_ok = true;
	m_isInit = true;
	return true;
}



void gkBuffer::queue(bool play)
{
	if (!m_ok)
		return;

	// queue initial buffers
	int blk = 0;
	for (blk = 0; blk < GK_SND_SAMPLES; ++blk)
	{
		UTsize br = 0;
		const char* db = read(m_bps, br);
		if (br != 0 && db)
		{
			//printf ("Block Read: %i (%p)[%i];\n", m_smp, db, br);

			alBufferData(m_buffer[blk], m_fmt, db, br, m_smp);
			m_ok = !alErrorThrow("queue buffers");
		}
		else
			break;
	}

	if (blk > 0)
	{
		alSourceQueueBuffers(m_source, blk, m_buffer);
		if (play)
		{
			alSourcePlay(m_source);
			m_ok = !alErrorThrow("source playback");
			doProperties();
		}
	}
	m_initial = false;
}



void gkBuffer::finalize(void)
{
	m_exit = true;
	if (m_sound)
	{
		m_stream = 0;
		m_sound->bind(0);
		m_sound = 0;
	}

	if (!m_ok || !m_isInit)
		return;

	reset();
	alDeleteBuffers(GK_SND_SAMPLES, m_buffer);
	alErrorThrow("closing buffers");
	alDeleteSources(1, &m_source);
	alErrorThrow("closing sources");
}



void gkBuffer::setProperties(const gkSoundProperties& props)
{
	m_props = props;
}



void gkBuffer::reset(void)
{
	if (!m_ok)
		return;

	ALuint buf;
	ALint nr;


	if (alIsPlaying(m_source))
	{
		alSourceStop(m_source);
		m_ok = !alErrorThrow("Stop playback");
	}

	alGetSourcei(m_source, AL_BUFFERS_QUEUED, &nr);
	if (nr > GK_SND_SAMPLES)
		printf("More Queued that expected!\n");

	while (nr--)
	{
		alSourceUnqueueBuffers(m_source, 1, &buf);
		m_ok = !alErrorThrow("reset:unqueue buffer");
	}

	m_pos = 0;
	m_eos = false;
	m_initial = true;
}


const char* gkBuffer::read(UTsize len, UTsize& br)
{
	if (!m_stream || m_eos)
	{
		m_eos = true;
		br = 0;
		return 0;
	}

	// read contents from open stream
	const char* blk = m_stream->read(m_pos, len, br);
	m_pos += br;
	m_eos = br == 0;
	return blk;
}



bool gkBuffer::stream(void)
{
	// stream contents to OpenAL buffers
	ALuint buf;
	ALint nr;

	if (!m_isInit)
		initialize();

	if (!m_initial)
	{
		if (!m_ok)
			return false;

		// update pause
		if (m_doSuspend)
			doSuspend();


		if (m_suspend)
			return false;


		// update 3D properties
		if (m_do3D) do3D();
	}
	else
	{
		queue(true);

		if (!m_ok)
			return false;
	}


	if (m_exit)
		return false;

	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &nr);
	if (nr <= 0)
		return false;


	if (!m_eos)
	{
		UTsize br = -1;
		int totQue = 0;

		while (nr--)
		{
			// read another block
			alSourceUnqueueBuffers(m_source, 1, &buf);
			m_ok = !alErrorThrow("stream:unqueue buffer");

			if (br != 0)
			{
				const char* db = read(m_bps, br);
				if (br != 0 && db)
				{
					//printf ("Block Read: %i (%p)[%i];\n", m_smp, db, br);

					alBufferData(buf, m_fmt, db, br, m_smp);
					alSourceQueueBuffers(m_source, 1, &buf);
					m_ok = !alErrorThrow("stream:requeue buffer");
					totQue++;
				}
			}
		}

		if (totQue > 0)
		{
			if (!alIsPlaying(m_source))
			{
				alSourcePlay(m_source);
				m_ok = !alErrorThrow("stream:source playback");
			}

			return true;
		}
	}

	if (!alIsPlaying(m_source))
	{
		if (m_loop)
		{
			reset();
			return false;
		}
		else
			m_exit = true;
	}

	return true;
}
